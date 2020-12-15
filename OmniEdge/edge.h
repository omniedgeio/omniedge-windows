#ifndef EDGE_H
#define EDGE_H

#include <QObject>
#include <QProcess>
#include <QVector>
class Edge : public QObject
{
    Q_OBJECT

public:
    explicit Edge(QObject *parent = nullptr);
    enum ConnectStatus
    {
        Connecting = -1,
        Success = 1,
        Fail = 2,
        Timeout = 3,
    };
    Q_ENUM(ConnectStatus)
    void kill();

    void ping(QString ip);
   ~Edge();
signals:
    void fatalError(QString error_message);
    void info(QString message);
    void replyConnectStatus(int);

    void replyPingMs(QString ip, int ms);

public slots:
    void connectSuperNode(QString server_ip, QString port, QString community_name, QString password, QString internal_ip);

private:
    ConnectStatus status;
    QProcess* edge_process;
    QString server_ip;
    QString port;
    QString password;
    QString tap_ip;
    QString community_name;
    QString ip_mode;
    QString current_supernode;
    QVector<QString> supernodes;
    int retry_count = 0;

private slots:
    void output(void);
    void checkTimeout();

};

#endif // EDGE_H
