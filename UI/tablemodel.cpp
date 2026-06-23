#include "tablemodel.h"

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            // Check boundaries to prevent crashing if 'section' is out of bounds
            if (section >= 0 && section < m_headers.size()) {
                return m_headers.at(section);
            }
            return QStringLiteral("Unknown");
        }

        if (orientation == Qt::Vertical) {
            return section + 1;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}



int TableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_currentPageData.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return m_headers.size();
}

bool TableModel::hasChildren(const QModelIndex &parent) const
{
    return false;
}

bool TableModel::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return false;
}



QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // Ensure the requested row is within our current page's list of patients
    if (index.row() < 0 || index.row() >= m_currentPageData.size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        // Get the specific patient pointer for this row
        Patient patient = m_currentPageData.at(index.row());


        // Map columns to the Patient properties based on your Arabic headers
        switch (index.column()) {
        case 5 : return patient.id();
        case 4: return patient.name();
        case 3: return patient.age();
        case 2: return patient.job();
        case 1: return patient.address();
        case 0: return patient.date();
        default: return QVariant();
        }
    }

    if (role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignRight | Qt::AlignVCenter); // Right-align for Arabic
    }

    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    // FIX 1: Use a reference (&) so modifications alter the actual vector cache memory!
    Patient &patient = m_currentPageData[index.row()];

    bool trackingChanged = false;
    QString stringValue = value.toString();

    switch (index.column()) {
    case 5:
        return false;
    case 2:
        if (patient.job() != stringValue) {
            patient.setJob(stringValue);
            trackingChanged = true;
        }
        break;
    case 4:
        if (patient.name() != stringValue) {
            patient.setName(stringValue);
            trackingChanged = true;
        }
        break;
    case 3:
        if (patient.age() != stringValue) {
            patient.setAge(stringValue);
            trackingChanged = true;
        }
        break;
    case 1:
        if (patient.address() != stringValue) {
            patient.setAddress(stringValue);
            trackingChanged = true;
        }
        break;
    case 0:
        if (patient.date() != stringValue) {
            patient.setDate(stringValue);
            trackingChanged = true;
        }
        break;
    default:
        return false;
    }

    // 3. Only hit the hard drive database if an actual text change occurred!
    if (trackingChanged) {
        if (m_repository.savePatient(patient)) {
            // Explicitly notify the QML TableView to repaint the modified text cell
            emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
            return true;
        } else {
            qCritical() << "Database update failed inside setData loop.";
            return false;
        }
    }

    return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row > rowCount(parent) || count <= 0)
        return false;

    // 1. Notify the QML view architecture that rows are being appended
    beginInsertRows(parent, row, row + count - 1);



    // 3. Finalize the layout change event loop
    endInsertRows();
    return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    // FIXME: Implement me!
    endRemoveRows();
    return true;
}

TableModel::~TableModel() {
}


void TableModel::loadData(int pageIndex, int pageSize, const QString &searchQuery)
{
    // 1. Tell the QTableView that the underlying grid rows are about to completely change
    beginResetModel();

    // 2. Fetch the fresh page from the repository and overwrite the vector
    // QVector safely handles clearing the old Patient objects automatically
    m_currentPageData = m_repository.fetchPage(pageIndex, pageSize, searchQuery);

    // 3. Notify the view that the reset is finished so it can redraw the grid layout
    endResetModel();
}

void TableModel::addPatient(const QString& name, const QString& age, const QString& job, const QString& location) {
    // 1. Calculate the index where the new row will sit (usually the end of the list)
    int nextRowIndex = m_currentPageData.count();

    // 2. Notify the QML View architecture that an insert operation is starting
    beginInsertRows(QModelIndex(), nextRowIndex, nextRowIndex);

    // 3. Execute your database append write via the repository
    // Note: Use a blank or temporary database primary ID here (e.g., -1 or 0)
    Patient newPatient("-1", name, age, job, location ,QDate::currentDate().toString("yyyy-MM-dd"));
    m_repository.addPatient(newPatient);

    // 4. Update your TableModel's local C++ memory cache list so rowCount() matches
    m_currentPageData.append(newPatient);

    // 5. Finalize the operation to force QML to instantly render the new row
    endInsertRows();
}