#ifndef PATIENT_H
#define PATIENT_H

#include <QString>

class Patient
{
public:
    Patient();

    Patient(const QString &id, const QString &name, const QString &age, const QString &job, const QString &address,const QString &date);

    // Public getters (These SHOULD be const because they only read data)
    QString job() const;
    QString name() const;
    QString age() const;
    QString date() const;
    QString id() const;
    QString address() const;
    // Public setters (Removed 'const' so they can modify member variables)
    void setJob(const QString &job);
    void setName(const QString &name);
    void setAge(const QString &age);
    void setDate(const QString &date);
    void setID(const QString &id);
    void setAddress(const QString &address);

private:
    QString m_job;
    QString m_name;
    QString m_age;
    QString m_date;
    QString m_id;
    QString m_address;
};

#endif // PATIENT_H