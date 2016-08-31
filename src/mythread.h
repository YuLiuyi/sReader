#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include "fileInfo.h"


class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread(QObject *parent = 0);
    void getFiles();
    void run();
    bool stop;

private slots:


signals:
    void infoListResult(InfoList);

private:
    InfoList mList;

};

#endif // MYTHREAD_H
