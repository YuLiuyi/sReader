#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include "mythread.h"
#include "listmodel.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller();

signals:
    void result(InfoList);

public slots:
    void getfiles();
    void handleResult(InfoList);

private:
    MyThread *mThread;
    ListModel mListModel;
    InfoList mList;
};

#endif // CONTROLLER_H
