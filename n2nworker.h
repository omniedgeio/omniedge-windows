#ifndef N2NWORKER_H
#define N2NWORKER_H

#include <QObject>

class N2NWorker : public QObject
{
    Q_OBJECT
public:
    explicit N2NWorker(QObject *parent = nullptr);

signals:

};

#endif // N2NWORKER_H
