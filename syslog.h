#ifndef SYSLOG_H
#define SYSLOG_H

#include <QObject>

class syslog : public QObject
{
    Q_OBJECT
private:
     bool isFileExist(QString fullFileName);
public:
    explicit syslog(QObject *parent = nullptr);
    ~syslog();
    void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void SoftWareShutDown();
    void installReleaseMsgHandler();
signals:

public slots:
};

#endif // SYSLOG_H
