#ifndef TAPMANAGER_H
#define TAPMANAGER_H

#include <QObject>

class TapManager : public QObject
{
    Q_OBJECT
public:
    explicit TapManager(QObject *parent = nullptr);

public slots:
    void addTap();
signals:

private:
    QString tapPath;
};

#endif // TAPMANAGER_H
