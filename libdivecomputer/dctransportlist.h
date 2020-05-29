#ifndef DCTRANSPORTLIST_H
#define DCTRANSPORTLIST_H

#include <QObject>
#include <QModelIndex>
#include <QAbstractListModel>
#include <libdivecomputer/common.h>

class DCTransport : public QObject
{
public:
    DCTransport(int index, dc_transport_t transport);
    virtual ~DCTransport();
    dc_transport_t transport;
    QString* description;
    int index;
};

Q_DECLARE_METATYPE(DCTransport*)

const char* translate_transport(dc_transport_t trans);

class DCTransportList : public QAbstractListModel
{
    Q_OBJECT
public:
    enum TransportRoles {
        DescriptionRole = Qt::UserRole + 1,
        IndexRole
    };
    Q_ENUMS(TransportRoles)

    DCTransportList(QObject *parent = 0);
    virtual ~DCTransportList();
    void loadTransports(unsigned int transports);


    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    DCTransport* addTransport(dc_transport_t trans);


public slots:
    // Extra function to get the thing easily from outside Repeaters.
    DCTransport* get(int idx);
    void add(DCTransport* comp);
    void clear();
protected:
    QHash<int, QByteArray> roleNames() const ;
private:
    QList<DCTransport*> mTransports;
};

#endif // DCTRANSPORTLIST_H
