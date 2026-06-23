#include "repository.h"

// 1. READ (PAGINATION)
QVector<Patient> Repository::fetchPage(int pageIndex, int pageSize, const QString &searchQuery)
{
    QVector<Patient> patients;
    QSqlQuery query;

    int offset = pageIndex * pageSize;

    // Fix: Use the correct column names from your main.cpp schema!
    QString sql = "SELECT CODEA, TRAV, NAME, AGE, ADDR, DATEA FROM UD1";


    if (!searchQuery.isEmpty()) {
        // All schema columns capitalized to match your system queries exactly
        QStringList columns = {
            "CODEA", "TRAV", "NAME", "AGE", "ADDR", "DATEA"
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
                query.value(0).toString(), // id
                query.value(2).toString(), // name
                query.value(3).toString(), // age
                query.value(1).toString(), // trav (mapped to job)
                query.value(4).toString(), // Address
                query.value(5).toString()  // datea
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
    query.prepare("UPDATE UD1 SET TRAV = :job, NAME = :name, AGE = :age, ADDR = :address ,DATEA = :date WHERE CODEA = :id");

    query.bindValue(":job",  patient.job());
    query.bindValue(":name", patient.name());
    query.bindValue(":age",  patient.age());
    query.bindValue(":date", patient.date());
    query.bindValue(":id",   patient.id());
    query.bindValue(":address", patient.address());
    if (!query.exec()) {
        qCritical() << "SQLite Update Failed:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Repository::addPatient(const Patient &patient)
{
    QSqlQuery query;
    // Map job to 'trav' instead of 'codea'
    query.prepare("INSERT INTO UD1 (TRAV, NAME, AGE, DATEA, ADDR) VALUES (:job, :name, :age, :date, :address)");

    query.bindValue(":job",  patient.job());  // e.g., "موظف" goes into trav
    query.bindValue(":name", patient.name());
    query.bindValue(":age",  patient.age());
    query.bindValue(":date", patient.date());
    query.bindValue(":daddress", patient.date());
    if (!query.exec()) {
        qCritical() << "SQLite Partial Insert Failed:" << query.lastError().text();
        return false;
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