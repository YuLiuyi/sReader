#include "controller.h"
#include <QDebug>
#include <QUrl>
#define FILE_PATH "/home/user/files"

Controller::Controller()
{
    mThread = new MyThread();
    connect(mThread, SIGNAL(infoListResult(InfoList)), this, SLOT(handleResult(InfoList)));
}

void Controller::getfiles()
{
    mList.clear();
    qDebug() << Q_FUNC_INFO <<__LINE__;
    mThread->start();
}

void Controller::handleResult(InfoList list)
{
    mList.clear();
    mList = list;
    emit result(mList);
}
