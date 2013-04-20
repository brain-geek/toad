#ifndef TOADAPPLICATION_H
#define TOADAPPLICATION_H

#include <QApplication>
#include <QDebug>
#include <QThread>

class ToadApplication : public QApplication
{
    Q_OBJECT
public:
    int exec();
    ToadApplication(int &argc, char **& argv);

signals:
    void start();

public slots:

protected:
};

#endif // TOADAPPLICATION_H
