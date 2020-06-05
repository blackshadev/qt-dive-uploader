#ifndef DCPORTLIST_H
#define DCPORTLIST_H

#include <QObject>
#include <QModelIndex>
#include <QAbstractListModel>
#include <libdivecomputer/device.h>

class DCPort : public QObject
{
public:
    DCPort(int index, dc_transport_t trans, void *dev);
    virtual ~DCPort();
    QString* description;
    int index;
    dc_transport_t transport;
    void *device;
};

Q_DECLARE_METATYPE(DCPort*);

class DCPortList : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PortRoles {
        DescriptionRole,
        IndexRole
    };
    Q_ENUMS(PortRoles)

    DCPortList(QObject *parent = 0);
    ~DCPortList();

    void clear();
    dc_status_t load(dc_context_t* ctx, dc_descriptor_t* descr, dc_transport_t trans);

    DCPort* get(int idx);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
signals:
    void loadstart();
    void loadend();

protected:
    QHash<int, QByteArray> roleNames() const ;
private:
    QList<DCPort*> m_ports;
};

#endif // DCPORTLIST_H
