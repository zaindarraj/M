#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QtQml/qqmlregistration.h>
#include <QDate>

#include "patient.h"
#include "repository.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit TableModel(QObject *parent = nullptr);

    ~TableModel();

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    bool canFetchMore(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_INVOKABLE void addPatient(const QString& name, const QString& age, const QString& job, const QString& location);

    Q_INVOKABLE void loadData(int pageIndex, int pageSize = 10, const QString &searchQuery = "");

private:
    QStringList m_headers = {"التاريخ" ,"الموقع",  "المهنة", "العمر", "الاسم", "المعرف"};
    QVector<Patient> m_currentPageData;
    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    int m_totalRecordsInDatabase = 0; // Total count (e.g., 50000)
    int m_currentPage = 0;
    int m_itemsPerPage = 10;
    Repository m_repository;
};

#endif // TABLEMODEL_H
