#include "qdcdivelistmodel.h"
#include "../parsers/qdiveparser.h"
#include "../divecomputer/reader/dcreader.h"

QDCDiveListModel::QDCDiveListModel(QObject *parent) : QAbstractListModel(parent)
{

}

QDCDiveListModel::~QDCDiveListModel()
{

}

int QDCDiveListModel::rowCount(const QModelIndex &parent) const
{
    return items.size();
}

QVariant QDCDiveListModel::data(const QModelIndex &index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= items.size()) {
        return QVariant(QVariant::Invalid);
    }

    auto dive = items[i];
    switch (role) {
        case DateRole:
            return QVariant::fromValue(QString::fromStdString(dive->getDisplayDateTime()));
        case DepthRole:
            return QVariant::fromValue(dive->getMaxDepth());
        case TimeRole:
            return QVariant::fromValue(QString::fromStdString(dive->getDisplayDivetime()));
    }
}

void QDCDiveListModel::add(QDCDive *dive)
{
    items.push_back(dive);
}

void QDCDiveListModel::clear()
{
    beginResetModel();
    for(auto item : items) {
        delete item;
    }
    items.clear();
    endResetModel();
}

void QDCDiveListModel::read(QDCDevice *device)
{
    DCReader reader;
    QDiveParser parser;

    reader.setDevice((DCDevice *)device);
    reader.setOnDiveCallback([this, &parser](rawdivedata_t data) {
        auto dive = parser.parseDive(data);
        add((QDCDive *)dive);
    });

    beginResetModel();
    clear();
    reader.start();
    endResetModel();
}

QHash<int, QByteArray> QDCDiveListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DateRole] = "date";
    roles[DepthRole] = "depth";
    roles[TimeRole] = "time";
    return roles;
}
