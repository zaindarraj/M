#include "repository.h"

// 1. READ (PAGINATION)
QVector<Patient> Repository::fetchPage(int pageIndex, int pageSize, const QString &searchQuery)
{
    QVector<Patient> patients;
    QSqlQuery query;

    int offset = pageIndex * pageSize;

    // Fix: Use the correct column names from your main.cpp schema!
    QString sql = "SELECT CODEA, TRAV, NAME, AGE, ADDR, DATEA, TEL FROM UD1";


    if (!searchQuery.isEmpty()) {
        // All schema columns capitalized to match your system queries exactly
        QStringList columns = {
            "CODEA", "TRAV", "NAME", "AGE", "ADDR", "DATEA", "TEL"
        };

        QStringList conditions;
        for (const QString &col : columns) {
            conditions.append(QString("%1 LIKE :search").arg(col));
        }

        // Generates: WHERE (TRAV LIKE :search OR TEL LIKE :search OR NAME LIKE :search ...)
        sql += " WHERE (" + conditions.join(" OR ") + ")";
    }

    if (searchQuery.isEmpty()) {
        sql = sql + " ORDER BY CODEA DESC LIMIT :limit OFFSET :offset";
    }



    query.prepare(sql);
    if (!searchQuery.isEmpty()) {
        query.bindValue(":search", "%" + searchQuery.trimmed() + "%");
    }

    if (searchQuery.isEmpty()) {
        query.bindValue(":limit", pageSize);
        query.bindValue(":offset", offset);
    }



    if (query.exec()) {
        while (query.next()) {
            Patient p(
                query.value("CODEA").toString(), // id
                query.value("NAME").toString(), // name
                query.value("AGE").toString(), // age
                query.value("TRAV").toString(), // trav (mapped to job)
                query.value("ADDR").toString(), // Address
                query.value("TEL").toString(), // Address
                query.value("DATEA").toString()  // datea
                );
            patients.append(p);
        }
    } else {
        qCritical() << "SQLite Fetch Failed:" << query.lastError().text();
    }
    return patients;
}

bool Repository::savePatient(const Patient &patient)
{
    QSqlQuery query;
    // Update 'trav' instead of 'codea'
    query.prepare("UPDATE UD1 SET TRAV = :job, NAME = :name, AGE = :age, ADDR = :address ,DATEA = :date, TEL = :phone  WHERE CODEA = :id");

    query.bindValue(":job",  patient.job());
    query.bindValue(":name", patient.name());
    query.bindValue(":age",  patient.age());
    query.bindValue(":date", patient.date());
    query.bindValue(":id",   patient.id());
    query.bindValue(":phone",patient.phone());

    query.bindValue(":address", patient.address());
    if (!query.exec()) {
        qCritical() << "SQLite Update Failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Repository::addPatient(const Patient &patient)
{
    QSqlQuery idQuery;
    int nextId = 1; // Default fallback if table is empty

    // 1. Fetch the largest current CODEA safely converted to an integer
    if (idQuery.exec("SELECT MAX(CAST(CODEA AS INTEGER)) FROM UD1")) {
        if (idQuery.next() && !idQuery.value(0).isNull()) {
            nextId = idQuery.value(0).toInt() + 1;
        }
    } else {
        qCritical() << "Failed to calculate next CODEA:" << idQuery.lastError().text();
        return false;
    }

    QString generatedCodeA = QString::number(nextId);

    // 2. Insert into UD1 including our new generated CODEA
    QSqlQuery query;
    query.prepare("INSERT INTO UD1 (CODEA, TRAV, NAME, AGE, DATEA, ADDR, TEL) "
                  "VALUES (:codea, :job, :name, :age, :date, :address, :phone)");

    query.bindValue(":codea",   generatedCodeA);
    query.bindValue(":job",     patient.job());
    query.bindValue(":name",    patient.name());
    query.bindValue(":age",     patient.age());
    query.bindValue(":date",    patient.date());
    query.bindValue(":phone",    patient.phone());
    query.bindValue(":address", patient.address()); // Fixed typo from ":daddress" / patient.date()

    if (!query.exec()) {
        qCritical() << "SQLite UD1 Insert Failed:" << query.lastError().text();
        return false;
    }

    // 3. Initialize a clean record entry in UD2 with the matching CODEA
    PatientRecord defaultRecord = PatientRecord();
    defaultRecord.setId(generatedCodeA);
    defaultRecord.setAdmissionDate(patient.date()); // Seed default date from entry profile

    QSqlQuery insertQuery;
    insertQuery.prepare(R"(
        INSERT INTO "main"."UD2" (
            "CODEA", "DZ1", "DZ2", "DZ3", "DZ6", "DZ17",
            "DZ19", "DZ20", "DATEA", "codep", "DZ5", "DZ4"
        ) VALUES (
            :codea, :dz1, :dz2, :dz3, :dz6, :dz17,
            :dz19, :dz20, :datea, :codep, :dz5, :dz4
        );
    )");

    defaultRecord.bindToQuery(insertQuery);

    if (!insertQuery.exec()) {
        qWarning() << "Failed initializing secondary UD2 data table slot:" << insertQuery.lastError().text();
        // Return true anyway because the patient was successfully registered to the main system registry UD1
    }

    return true;
}

// 4. DELETE
bool Repository::deletePatient(int patientId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM patients WHERE CODEA = :id");
    query.bindValue(":id", patientId);

    if (!query.exec()) {
        qCritical() << "SQLite Delete Failed:" << query.lastError().text();
        return false;
    }
    return true;
}



PatientRecord Repository::fetchPatientRecored(QString id){
    // 1. Prepare the statement with a positional '?' placeholder
    QSqlQuery query;
    // 1. Try to SELECT the existing record
    query.prepare(R"(
        SELECT "CODEA", "codep", "DATEA", "DZ1", "DZ2", "DZ3", "DZ4", "DZ5", "DZ6", "DZ17", "DZ19", "DZ20"
        FROM "UD2"
        WHERE "CODEA" = ?;
    )");
    query.bindValue(0, id);

    if (query.exec() && query.next()) {
        qDebug() << "Patient found. Returning existing record for ID:" << id;

        // Populate and return the found record
        return PatientRecord(
            query.value("CODEA").toString(),
            query.value("DZ1").toString(),
            query.value("DZ2").toString(),
            query.value("DZ3").toString(),
            query.value("DZ6").toString(),
            query.value("DZ17").toString(),
            query.value("DZ19").toString(),
            query.value("DZ20").toString(),
            query.value("DATEA").toString(),
            query.value("DZ5").toString(),
            query.value("DZ4").toString()
            );
    }

    // 2. If it does not exist, prepare the CREATE (INSERT) routine
    qDebug() << "Patient not found. Creating record for ID:" << id;

    PatientRecord defaultRecord =  PatientRecord();
    defaultRecord.setId(id) ;

    QSqlQuery insertQuery;
    insertQuery.prepare(R"(
        INSERT INTO "main"."UD2" (
            "CODEA", "DZ1", "DZ2", "DZ3", "DZ6", "DZ17",
            "DZ19", "DZ20", "DATEA", "codep", "DZ5", "DZ4"
        ) VALUES (
            :codea, :dz1, :dz2, :dz3, :dz6, :dz17,
            :dz19, :dz20, :datea, :codep, :dz5, :dz4
        );
    )");

    // Bind using the default backup record template passed to the function
    defaultRecord.bindToQuery(insertQuery);

    if (!insertQuery.exec()) {
        qWarning() << "Critical Error creating record inside getOrCreate:" << insertQuery.lastError().text();
        // Return an empty record or handle database connectivity crashes
        return PatientRecord();
    }

    // Return the newly tracked record back to the execution line
    return defaultRecord;


}

// Updates an existing patient record row based on its unique ID
bool Repository::savePatientRecored(const PatientRecord &patient)
{
    QSqlQuery query;

    // We target the "UD2" table and update fields matching the fetch/insert logic
    query.prepare(R"(
        UPDATE "UD2"
        SET
            "DZ1"   = :dz1,
            "DZ2"   = :dz2,
            "DZ3"   = :dz3,
            "DZ4"   = :dz4,
            "DZ5"   = :dz5,
            "DZ6"   = :dz6,
            "DZ17"  = :dz17,
            "DZ19"  = :dz19,
            "DZ20"  = :dz20,
            "DATEA" = :datea
        WHERE "CODEA" = :codea;
    )");

    patient.bindToQuery(query);

    if (!query.exec()) {
        qCritical() << "SQLite PatientRecord Update Failed:" << query.lastError().text();
        return false;
    }

    return true;
}