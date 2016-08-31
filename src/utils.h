/*
* This file is part of file-manager
*
* Copyright (C) 2014 Beijing Yuan Xin Technology Co.,Ltd. All rights reserved.
*
* Authors:
*       Zhenbin Cai <caizhenbin@syberos.com>
*       Pengcheng Zhang <zhangpengcheng@syberos.com>
*
* This software, including documentation, is protected by copyright controlled
* by Beijing Yuan Xin Technology Co.,Ltd. All rights are reserved.
*/
#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include <QFile>
#include <QThread>

class Utils : public QThread
{
    Q_OBJECT
    Q_ENUMS(InfoItem)
    Q_ENUMS(TaskType)

public:
    enum InfoItem {
        SubFileCount = 0,
        FileSize = 1,
        ModifiedDate = 2,
        IconPath = 3,
        FileName = 4
    };
    enum TaskType {
        TaskNone,
        TaskCut,
        TaskCopy,
        TaskRemove
    };

    Q_INVOKABLE static QString homePath();
    Q_INVOKABLE static QString sdcardPath();
    Q_INVOKABLE static QString tmpPath();
    Q_INVOKABLE static qreal screenRatio();
    Q_INVOKABLE static QVariant fileInfo(QString filePath, InfoItem item);
    Q_INVOKABLE static qint64 freeSpace(QString path);
    Q_INVOKABLE static qint64 totalSpace(QString path);

    Q_INVOKABLE static bool rename(const QString &filePath, const QString &newName);
    Q_INVOKABLE bool rm(const QStringList &paths);
    Q_INVOKABLE bool hasFileExist(const QStringList &paths, const QString &destPath);
    Q_INVOKABLE void paste(const QStringList &paths, const QString &destPath, const bool isCut);
    Q_INVOKABLE void taskCancel();

    Q_INVOKABLE QString getMimeType(QString filepath);
    Q_INVOKABLE QString getFileType(QString filepath);
    Q_INVOKABLE QStringList getImgFilesInDir(QString dir, QString sortby = "TIME_DESC");

    Q_INVOKABLE QString getFristCopyedFile() {return fristCopyedFile;}
    Q_INVOKABLE QString getFristCutedFile() {return fristCutedFile;}


    // from: http://blog.csdn.net/ymc0329/article/details/7975654
    bool isfileNameTooLong(const QString &pathName);
    bool copyFileToPath(QString sourceFilePath , QString toDir, bool coverFileIfExist);
    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);
    bool cutFileDirToPath(QString sourcePath ,QString toDir, bool coverIfExist);

    void init();

signals:
    Q_INVOKABLE void taskPrepare(TaskType type);
    Q_INVOKABLE void progressChanged(TaskType type, qreal progress);
    Q_INVOKABLE void taskFinished(TaskType type);
    Q_INVOKABLE void taskError(TaskType type,QString info = "");

private slots:
    void copySizeChanged(qint64 size);

protected:
    void run();

private:
    bool reWrite(QString path);

    bool doCopy(QString fromPath, QString toPath);
    QString renameExistPath(QString path, bool isDir);

    TaskType taskType;
    bool isCancel;
    QStringList taskFileList;
    QString copyTarget;
    QFile targetFile;
    qint64 copyedSize;
    qint64 totalCopySize;
    qreal copyProgress;
    QStringList copyedFileList; //for cancel copy

    //Add them for move the listView to the appropriate location after copy/cut
    QString fristCopyedFile;
    QString fristCutedFile;
};

#endif // UTILS_H
