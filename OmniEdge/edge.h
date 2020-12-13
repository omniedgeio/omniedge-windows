#ifndef EDGE_H
#define EDGE_H

#include <QObject>
#include <QProcess>

class Edge : public QObject
{
    Q_OBJECT
public:
    explicit Edge(QObject *parent = nullptr);

signals:

public slots:
    void connectSuperNode(QString server_ip, QString port, QString community_name, QString password, QString internal_ip);

private:
    QProcess *edge_process;
    ;
    QString port;
    QString password;
    QString internal_ip;
    QString community_name;

private slots:
    void output(void);

};

#endif // EDGE_H
