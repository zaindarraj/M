#include "patientrecored.h"

PatientRecord::PatientRecord(QString id, QString bloodPressure, QString pulseRate, QString bodyTemperature,
                             QString detailedHistory, QString labResultsRaw, QString differentialDiagnosis,
                             QString treatmentProcedure, QString admissionDate,
                             QString symptomDuration, QString chiefComplaint)
    : m_id(id),
    m_admissionDate(admissionDate),
    m_bloodPressure(bloodPressure),
    m_pulseRate(pulseRate),
    m_bodyTemperature(bodyTemperature),
    m_chiefComplaint(chiefComplaint),
    m_symptomDuration(symptomDuration),
    m_detailedHistory(detailedHistory),
    m_labResultsRaw(labResultsRaw),
    m_differentialDiagnosis(differentialDiagnosis),
    m_treatmentProcedure(treatmentProcedure)
{
}

void PatientRecord::bindToQuery(QSqlQuery &query) const {
    query.bindValue(":codea", m_id);
    query.bindValue(":dz1", m_bloodPressure);
    query.bindValue(":dz2", m_pulseRate);
    query.bindValue(":dz3", m_bodyTemperature);
    query.bindValue(":dz6", m_detailedHistory);
    query.bindValue(":dz17", m_labResultsRaw);
    query.bindValue(":dz19", m_differentialDiagnosis);
    query.bindValue(":dz20", m_treatmentProcedure);
    query.bindValue(":datea", m_admissionDate);
    query.bindValue(":dz5", m_symptomDuration);
    query.bindValue(":dz4", m_chiefComplaint);
}