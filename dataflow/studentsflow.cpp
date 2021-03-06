#include "studentsflow.h"
#include <QDebug>

StudentsFlow::StudentsFlow(QObject *parent)
    : QAbstractListModel(parent)
{
}

void StudentsFlow::addStudent(const SingleStudent &myStudent)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_students << myStudent;
    endInsertRows();
}

void StudentsFlow::removeStudent(const int ind)
{
    int i = findById(ind);
    qInfo() << "REMOVE" << i << "ID:" << ind;

    if (i >= 0)
    {
        beginRemoveRows(QModelIndex(),i,i);
        m_students.removeAt(i);
        endRemoveRows();
    }

}

int StudentsFlow::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_students.count();
}

QVariant StudentsFlow::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_students.count())
        return QVariant();

    const SingleStudent &myStudent = m_students[index.row()];
    if (role == NameRole)
        return myStudent.name();
    else if (role == SurnameRole)
        return myStudent.surname();
    else if (role == PlusesRole)
        return myStudent.pluses();
    else if (role == AverageRole)
        return myStudent.average();
    else if (role == AvatarRole)
        return myStudent.avatar();
    else if (role == StatusRole)
        return myStudent.status();
    else if (role == IsmainRole)
        return myStudent.ismain();
    return QVariant();
}

//![0]
QHash<int, QByteArray> StudentsFlow::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[OrderRole] = "order";
    roles[NameRole] = "name";
    roles[SurnameRole] = "surname";
    roles[AverageRole] = "average";
    roles[PlusesRole] = "pluses";
    roles[AvatarRole] = "avatar";
    roles[StatusRole] = "status";
    roles[IsmainRole] = "ismain";
    return roles;
}
//![0]

bool StudentsFlow::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() >= 0 && index.row() < rowCount())
    {
        SingleStudent local = m_students[index.row()];
        switch (role) {

        case StatusRole:

            m_students.replace(index.row(), SingleStudent(local.order(),local.name(),
                                                      local.surname(),
                                                      local.pluses(),
                                                      local.average(),
                                                      local.avatar(),value.toBool(),
                                                      local.ismain()));
            emit dataChanged(index, index, {role});
            return true;

        case IsmainRole:

            m_students.replace(index.row(), SingleStudent(local.order(),local.name(),
                                                      local.surname(),
                                                      local.pluses(),
                                                      local.average(),
                                                      local.avatar(),local.status(),
                                                      value.toBool()));
            emit dataChanged(index, index, {role});
            return true;

        case PlusesRole:

            m_students.replace(index.row(), SingleStudent(local.order(),local.name(),
                                                          local.surname(),
                                                          value.toString(),
                                                          local.average(),
                                                          local.avatar(),local.status(),
                                                          local.ismain()));
            emit dataChanged(index, index, {role});
            return  true;

        case AverageRole:

            m_students.replace(index.row(), SingleStudent(local.order(),local.name(),
                                                          local.surname(),
                                                          local.pluses(),
                                                          value.toString(),
                                                          local.avatar(),local.status(),
                                                          local.ismain()));
            emit dataChanged(index, index, {role});
            return true;

        default:
            return false;
        }
    }
    return false;
}

void StudentsFlow::setIsOn(int i)
{
    if (( i >= 0 ) && ( i < rowCount() ))
        setData(this->index(i), true, StatusRole);
}

void StudentsFlow::setMain(int id)
{
    int i = findById(id);
    if (i >= 0)
        setData(this->index(i), true, IsmainRole);
}

void StudentsFlow::removeAll()
{
    int mcount = getMainCount();
    int i = rowCount() - 1;
    while (rowCount() > mcount){
        if ( !m_students[i].ismain() ) {
            beginRemoveRows(QModelIndex(),i,i);
            m_students.removeAt(i);
            endRemoveRows();
        }
        i--;
    }
}

void StudentsFlow::setPM(QString plumin, int id)
{
    int ind = findById(id);
    if (ind >= 0)
        setData(this->index(ind), plumin, PlusesRole);
}

void StudentsFlow::setAverage(QString average, int id)
{
    int ind = findById(id);
    if (ind >= 0)
        setData(this->index(ind), average, AverageRole);
}

void StudentsFlow::activeStudent(int id)
{
    qInfo() << id;
    setMain(id);
    int mainCount = getMainCount();
    int i = rowCount() - 1 ; //INITIAL I MUST BE ROWCOUNT-1 <INDEX OUT OF RANGE>
    while (rowCount() > mainCount){
        if ( !m_students[i].ismain() )
        {
            qInfo() << i;
            beginRemoveRows(QModelIndex(),i,i);
            m_students.removeAt(i);
            endRemoveRows();
        }
        i--;
    }
    qInfo() << "SET MAIN";

}

int StudentsFlow::findById(int id)
{
    int i = 0;
    while((m_students[i].order() != id) && (i < rowCount()))
    {
        i++;
    }

    if ( i == rowCount() )
        return -1;
    else {
        return i;
    }
}

int StudentsFlow::getMainCount()
{
    int count = 0;
    int i = 0;
    while ( i < rowCount() ) {
        if ( m_students[i].ismain() )
            count++;
        i++;
    }
    return count;
}

int StudentsFlow::getFirst()
{
    if (rowCount() > 0)
        return m_students[0].order();
    else {
        return -1;
    }
}

int StudentsFlow::getLast()
{
    int mainCount = getMainCount();
    if (mainCount > 0)
        return m_students[mainCount-1].order();
    else {
        return -1;
    }
}
