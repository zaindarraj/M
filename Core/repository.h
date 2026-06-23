#ifndef PATIENTREPOSITORY_H
#define PATIENTREPOSITORY_H

#include <QVector>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include "patient.h"

class Repository
{
public:
    Repository() = default;

    // Fetches a specific page of data (e.g., Page 0 gets items 0-9)
    QVector<Patient> fetchPage(int pageIndex, int pageSize = 10, const QString &searchQuery = "");

    // Updates an existing patient row based on its unique ID
    bool savePatient(const Patient &patient);

    // Inserts a brand new patient row into the database
    bool addPatient(const Patient &patient);

    // Deletes a patient row from the database using its ID
    bool deletePatient(int patientId);
};

#endif // PATIENTREPOSITORY_H