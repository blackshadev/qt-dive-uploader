#ifndef QDCDIVELISTMODEL_H
#define QDCDIVELISTMODEL_H
#include <QObject>
#include <QAbstractListModel>
#include <set>
#include "../entities/qdcdive.h"
#include "../device/qdcdevice.h"

class QDCDiveListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(unsigned int selectedCount READ selectedCount NOTIFY selectedCountChanged)

public:
    enum DiveRoles {
        DateRole = Qt::UserRole + 1,
        DepthRole,
        TimeRole,
        SelectedRole,
        DiveRole,
    };
    Q_ENUM(DiveRoles)
    QDCDiveListModel(QObject *parent = NULL);
    virtual ~QDCDiveListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    std::vector<QDCDive *> *getData();
    QDCDive *getDive(const QModelIndex &index) const;
    QModelIndex indexOf(QDCDive *dive) const;
public slots:
    void add(QDCDive *dive);
    void clear();
    void select(QDCDive *dive);
    void select(const QModelIndex &index);
    void deselect(const QModelIndex &index);
    void deselect(QDCDive *dive);
    void toggle(const QModelIndex &index);
    void toggle(const QModelIndex &index, bool state);
    void toggle(QDCDive *dive);
    void toggle(QDCDive *dive, bool state);
    bool isSelected(QDCDive *dive) const;
    bool isSelected(const QModelIndex &index) const;
    void selectAll();
    void deselectAll();
    unsigned int selectedCount() const;
signals:
    void selectedCountChanged(unsigned int c);
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    std::vector<QDCDive *> items;
    std::set<QDCDive *> selected;
};
Q_DECLARE_METATYPE(QDCDiveListModel *)

#endif // QDCDIVELISTMODEL_H
