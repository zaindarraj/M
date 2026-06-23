#include "patient.h"


// Parameterized constructor implementation
Patient::Patient(const QString &id , const QString &name, const QString &age,  const QString &job,  const QString &address, const QString &date)
    : m_id(id), m_job(job), m_name(name), m_age(age), m_address(address) , m_date(date)
{
}
// 2. Getters
QString Patient::job() const  { return m_job; }
QString Patient::name() const { return m_name; }
QString Patient::age() const  { return m_age; }
QString Patient::date() const { return m_date; }
QString Patient::id() const { return m_id; }
QString Patient::address() const { return m_address; }

// 3. Setters
void Patient::setJob(const QString &job)   { m_job = job; }
void Patient::setName(const QString &name) { m_name = name; }
void Patient::setAge(const QString &age)   { m_age = age; }
void Patient::setDate(const QString &date) { m_date = date; }
void Patient::setID(const QString &id){m_id = id;};
void Patient::setAddress(const QString &address){m_address = address;};
