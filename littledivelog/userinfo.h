#ifndef USERINFO_H
#define USERINFO_H
#include <QObject>
#include <QDateTime>

class UserInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int userId MEMBER m_user_id CONSTANT)
    Q_PROPERTY(QString name MEMBER m_name CONSTANT)
    Q_PROPERTY(QString email MEMBER m_email CONSTANT)
    Q_PROPERTY(QDateTime inserted MEMBER m_inserted CONSTANT)
    Q_PROPERTY(int diveCount MEMBER m_dive_count CONSTANT)
    Q_PROPERTY(int computerCount MEMBER m_computer_count CONSTANT)
    Q_PROPERTY(int buddyCount MEMBER m_buddy_count CONSTANT)
    Q_PROPERTY(int tagCount MEMBER m_tag_count CONSTANT)
public:
    explicit UserInfo(QObject *parent = 0);
    virtual ~UserInfo();
    int m_user_id;
    QString m_name;
    QString m_email;
    QDateTime m_inserted;
    int m_dive_count;
    int m_computer_count;
    int m_buddy_count;
    int m_tag_count;
signals:

public slots:
};

Q_DECLARE_METATYPE(UserInfo*)

#endif // USERINFO_H
