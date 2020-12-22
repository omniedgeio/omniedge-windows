#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

class syslog : public QObject
{
    Q_OBJECT
public:
    explicit syslog(QObject *parent = nullptr);
    ~syslog();
    void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void SoftWareShutDown();
    void installReleaseMsgHandler();
signals:

public slots:
};

#endif // LOGGER_H
