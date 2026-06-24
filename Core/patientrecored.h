#ifndef PATIENTRECORD_H
#define PATIENTRECORD_H

#pragma once

#include <QString>
#include <QSqlQuery>
#include <QObject>
#include <QtQml/qqmlregistration.h>


class PatientRecord {
    Q_GADGET
    QML_ELEMENT

    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(QString admissionDate READ admissionDate WRITE setAdmissionDate)
    Q_PROPERTY(QString bloodPressure READ bloodPressure WRITE setBloodPressure)
    Q_PROPERTY(QString pulseRate READ pulseRate WRITE setPulseRate)
    Q_PROPERTY(QString bodyTemperature READ bodyTemperature WRITE setBodyTemperature)
    Q_PROPERTY(QString chiefComplaint READ chiefComplaint WRITE setChiefComplaint)
    Q_PROPERTY(QString symptomDuration READ symptomDuration WRITE setSymptomDuration)
    Q_PROPERTY(QString detailedHistory READ detailedHistory WRITE setDetailedHistory)
    Q_PROPERTY(QString labResultsRaw READ labResultsRaw WRITE setLabResultsRaw)
    Q_PROPERTY(QString differentialDiagnosis READ differentialDiagnosis WRITE setDifferentialDiagnosis)
    Q_PROPERTY(QString treatmentProcedure READ treatmentProcedure WRITE setTreatmentProcedure)

public:
    QString m_id;
    QString m_admissionDate;
    QString m_bloodPressure;
    QString m_pulseRate;
    QString m_bodyTemperature;
    QString m_chiefComplaint;
    QString m_symptomDuration;
    QString m_detailedHistory;
    QString m_labResultsRaw;
    QString m_differentialDiagnosis;
    QString m_treatmentProcedure;

public:
    PatientRecord() = default;

    PatientRecord(QString id, QString bloodPressure, QString pulseRate, QString bodyTemperature,
                  QString detailedHistory, QString labResultsRaw, QString differentialDiagnosis,
                  QString treatmentProcedure, QString admissionDate,
                  QString symptomDuration, QString chiefComplaint);

    void bindToQuery(QSqlQuery &query) const;

    QString id() const { return m_id; }
    void setId(const QString &val) { m_id = val; }

    QString admissionDate() const { return m_admissionDate; }
    void setAdmissionDate(const QString &val) { m_admissionDate = val; }

    QString bloodPressure() const { return m_bloodPressure; }
    void setBloodPressure(const QString &val) { m_bloodPressure = val; }

    QString pulseRate() const { return m_pulseRate; }
    void setPulseRate(const QString &val) { m_pulseRate = val; }

    QString bodyTemperature() const { return m_bodyTemperature; }
    void setBodyTemperature(const QString &val) { m_bodyTemperature = val; }

    QString chiefComplaint() const { return m_chiefComplaint; }
    void setChiefComplaint(const QString &val) { m_chiefComplaint = val; }

    QString symptomDuration() const { return m_symptomDuration; }
    void setSymptomDuration(const QString &val) { m_symptomDuration = val; }

    QString detailedHistory() const { return m_detailedHistory; }
    void setDetailedHistory(const QString &val) { m_detailedHistory = val; }

    QString labResultsRaw() const { return m_labResultsRaw; }
    void setLabResultsRaw(const QString &val) { m_labResultsRaw = val; }

    QString differentialDiagnosis() const { return m_differentialDiagnosis; }
    void setDifferentialDiagnosis(const QString &val) { m_differentialDiagnosis = val; }

    QString treatmentProcedure() const { return m_treatmentProcedure; }
    void setTreatmentProcedure(const QString &val) { m_treatmentProcedure = val; }
};

Q_DECLARE_METATYPE(PatientRecord)

#endif // PATIENTRECORD_H