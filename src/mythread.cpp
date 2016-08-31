#include "mythread.h"
#include <QDir>
#include <QDebug>
#include <QFileInfoList>

MyThread::MyThread(QObject *parent)
{
    qDebug()<<"enter thread";
    stop = false;
}

void MyThread::getFiles()
{
    QDir dir("/home/user/documents");
    QString path, title, suffix;
    qint64 size;
    if(dir.exists()) {
        QFileInfoList fileInfoList = dir.entryInfoList();
        qDebug() << Q_FUNC_INFO<<"list : "<<fileInfoList.size();
        for (int index = 2; index < fileInfoList.size(); index ++)
        {
            QFileInfo file = fileInfoList.at(index);
            path = file.filePath();
            qDebug() << Q_FUNC_INFO<<"path : "<<path;
            title = file.fileName();
            suffix = file.suffix();
            size = file.size();
            mList.append(FileInfos(path, size, title, suffix));
        }
    }
    else {
        qDebug() << Q_FUNC_INFO<<"null list .";
    }
}

void MyThread::run()
{
    qDebug()<< "thread run.";
    mList.clear();
    getFiles();
    emit infoListResult(mList);
}
