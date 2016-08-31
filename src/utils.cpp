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
#include <QDir>
#include <QDebug>

#include "utils.h"

#include <algorithm>
#include <QGuiApplication>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include <QScreen>
#include <sys/statvfs.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <QMimeDatabase>

void Utils::init()
{
    QStringList category;
    category << "Pictures" << "Videos" << "Music" << "Documents" << "Records";
    if(QFileInfo(homePath()).isWritable()) {
        QDir home(homePath());
        for(int i=0; i<category.length(); i++)
        {
            home.mkdir(category.at(i));
        }
    }
}

#if 0
    struct statvfs {
       unsigned long  f_bsize;    /* file system block size */
       unsigned long  f_frsize;   /* fragment size */
       fsblkcnt_t     f_blocks;   /* size of fs in f_frsize units */
       fsblkcnt_t     f_bfree;    /* # free blocks */
       fsblkcnt_t     f_bavail;   /* # free blocks for unprivileged users */
       fsfilcnt_t     f_files;    /* # inodes */
       fsfilcnt_t     f_ffree;    /* # free inodes */
       fsfilcnt_t     f_favail;   /* # free inodes for unprivileged users */
       unsigned long  f_fsid;     /* file system ID */
       unsigned long  f_flag;     /* mount flags */
       unsigned long  f_namemax;  /* maximum filename length */
    };
#endif

qint64 Utils::totalSpace(QString path)
{
    struct statvfs buf;

    if (!statvfs(path.toLocal8Bit().data(), &buf)) {
        qint64 bsize = buf.f_bsize;
        qint64 blocks = buf.f_blocks;
       return bsize*blocks;
    }
    else {
       return -1;
    }
}
qint64 Utils::freeSpace(QString path)
{
    struct statvfs buf;

    if (!statvfs(path.toLocal8Bit().data(), &buf)) {
        qint64 bsize = buf.f_bsize;
        qint64 blocks = buf.f_bfree;
       return bsize*blocks;;
    }
    else {
       return -1;
    }
}

static bool isPicture(QString ext) {
    return (ext == "png") || (ext == "jpg") || (ext == "jpeg")
            || (ext == "bmp") || (ext == "tiff") || (ext == "gif");
}
static bool isMusic(QString ext) {
    return (ext == "mp3") || (ext == "ape") || (ext == "ogg");
}

static qint64 getDirSize(const QString &sDirPath)
{
    qint64 iSize = 0;
    QDir o_dir(sDirPath);
    if (!o_dir.exists())
    {
        return 0;
    }

    QStringList slst_dir_path;
    slst_dir_path << sDirPath;
    while (slst_dir_path.isEmpty() == false)
    {
        o_dir.setPath(slst_dir_path[0]);
        o_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        o_dir.setSorting(QDir::DirsFirst);
        slst_dir_path.removeFirst();

        foreach (QFileInfo o_file_info, o_dir.entryInfoList())
        {
//            if (o_file_info.fileName() == "." || o_file_info.fileName() == "..")
//            {
//                continue;
//            }

            iSize += o_file_info.size();
            if (o_file_info.isDir())
            {
                slst_dir_path << o_file_info.filePath();
            }
        }
    }

    return iSize;
}

static qint64 getAllFilesSize(const QStringList &paths)
{
    qint64 totalSize = 0;

    foreach (const QString &path, paths) {
        QFileInfo qfi(path);
        if(qfi.isDir()) {
            totalSize += getDirSize(path);
        } else {
            totalSize += qfi.size();
        }
    }

    return totalSize;
}

QVariant Utils::fileInfo(QString filePath, InfoItem item)
{
    QFileInfo info(filePath);
    switch (item) {
    case SubFileCount:
        if(info.isDir())
        {
            QDir dir(filePath);
            //qDebug() << dir.entryList(QStringList("*"));
            return dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot).count();
        } else {
            return 0;
        }
    case ModifiedDate:
        return info.lastModified();
    case IconPath:
        if(info.isDir())
            return "qrc:/img/dic.png";
        else
        {
            QString ext = info.suffix();
            if(isPicture(ext.toLower()))
                return "qrc:/img/file.png";
            else if(isPicture(ext.toLower()))
                return "qrc:/img/file.png";
            else if(isMusic(ext.toLower()))
                return "qrc:/img/file.png";
            else
                return "qrc:/img/file.png";
        }
    case FileName:
        return info.fileName();
    case FileSize:
        if(info.isDir())
            return getDirSize(filePath);
        else
            return info.size();
    default:
        return 0;
    }
}

QString Utils::homePath()
{
    //return "/root";
    //return "/mnt/sdcard";
    //return QDir::homePath();
    return "/home/user";
}

QString Utils::sdcardPath()
{
    QFile file("/mnt/sdcard");
    if(file.exists())
    {
        return "/mnt/sdcard";
    }
    else
    {
        return "NULL";
    }

}

QString Utils::tmpPath()
{
    return "/mnt/tmp";
}

qreal Utils::screenRatio()
{
    return qApp->primaryScreen()->geometry().width() / 720.0;
}

/*递归删除文件夹*/
bool removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }
    return result;
}

/*非递归删除文件夹*/
/*删除文件夹
//入口参数：const QString &folderDir---------------------文件夹全路径
//返回值：true----删除成功；false----文件夹不存在
*/
static bool removeFolderContent(const QString &folderDir)
{
    QDir dir(folderDir);
    QFileInfoList fileList;
    QFileInfo curFile;
    if(!dir.exists())  {return false;}//文件不存，则返回false
    fileList=dir.entryInfoList(QDir::Dirs|QDir::Files
                               |QDir::Readable|QDir::Writable
                               |QDir::Hidden|QDir::NoDotAndDotDot
                               ,QDir::Name);
    while(fileList.size()>0)//跳出条件
    {
        int infoNum=fileList.size();
        for(int i=infoNum-1;i>=0;i--)
        {
            curFile=fileList[i];
            if(curFile.isFile())//如果是文件，删除文件
            {
                QFile fileTemp(curFile.filePath());
                fileTemp.remove();
                fileList.removeAt(i);
            }
            if(curFile.isDir())//如果是文件夹
            {
                QDir dirTemp(curFile.filePath());
                QFileInfoList fileList1=dirTemp.entryInfoList(QDir::Dirs|QDir::Files
                                                              |QDir::Readable|QDir::Writable
                                                              |QDir::Hidden|QDir::NoDotAndDotDot
                                                              ,QDir::Name);
                if(fileList1.size()==0)//下层没有文件或文件夹
                {
                    dirTemp.rmdir(dirTemp.absolutePath());
                    fileList.removeAt(i);
                }
                else//下层有文件夹或文件
                {
                    for(int j=0;j<fileList1.size();j++)
                    {
                        if(!(fileList.contains(fileList1[j])))
                            fileList.append(fileList1[j]);
                    }
                }
            }
        }
    }
    dir.rmdir(dir.absolutePath());
    return true;
}

bool Utils::rm(const QStringList &paths) //temp, wait for nemo-plugin-forderlistmodel update
{
    foreach (const QString &path, paths) {
        if(QFileInfo(path).isDir()) {
            if(!QDir(path).removeRecursively()) { //if(!removeDir(path)) //QDir::removeRecursively()
                emit taskError(TaskRemove);
                return false;
            }
        }
        else {
            if(!QFile::remove(path))
                return false;
        }
    }
    emit taskFinished(TaskRemove);
    return true;
}

void Utils::paste(const QStringList &paths, const QString &destPath, const bool isCut)
{
    fristCopyedFile = "";
    fristCutedFile = "";
    if(isCut) {
        QDir r;
        taskType = TaskCut;
        foreach (const QString &path, paths) {
            cutFileDirToPath(path, destPath, false);
        }
        emit taskFinished(taskType);
    } else {
        taskType = TaskCopy;
        isCancel = false;
        taskFileList.clear();
        taskFileList = QStringList(paths);
        copyTarget = destPath;
        connect(&targetFile, SIGNAL(bytesWritten(qint64)), this, SLOT(copySizeChanged(qint64)));
        start();
    }
}

void Utils::copySizeChanged(qint64 size)
{
    copyedSize += size;
    qDebug() << "copyed size: " << copyedSize << size;
}

bool Utils::rename(const QString &filePath, const QString &newName)
{
    const QFileInfo fi(filePath);
    bool re;

    //Because have limited the length of newName in the interface so we don't need to check it again
    if (fi.isDir()) {
        QDir d(fi.absoluteFilePath());
        re = d.rename(fi.absoluteFilePath(), fi.absolutePath() + QDir::separator() + newName);
    } else {
        QFile f(fi.absoluteFilePath());
        re = f.rename(fi.absolutePath() + QDir::separator() + newName);
    }

    return re;
}

#define BUFSIZE (128*1024)

bool Utils::doCopy(QString fromPath, QString toPath)
{
    struct stat st;
    char buffer[BUFSIZE];

    int in = open(fromPath.toLocal8Bit().data(), O_RDONLY | O_LARGEFILE);

    // Get the input file permission
    fstat(in, &st);

    int out = open(toPath.toLocal8Bit().data(), O_WRONLY | O_CREAT | O_TRUNC | O_LARGEFILE, st.st_mode);
    qDebug()<<"*****************************file permission: "<<st.st_mode;
    if (out<0) {
        qDebug() << "Utils::doCopy() failed!!!!!!!!!!!!!! fromPath and toPath---------------" << fromPath << toPath;
        return false;
    }

    ssize_t bytes_read;
    while(!isCancel && (bytes_read = read(in, buffer, BUFSIZE)) > 0)
    {
        write(out, buffer, bytes_read);
        copyedSize += bytes_read;
        qreal p = qreal(copyedSize)/totalCopySize;
        if(p - copyProgress >= 0.05) {
            copyProgress = p;
            if(!isCancel)
                emit progressChanged(TaskCopy, copyProgress);
        }
    }

    // copy is done, or an error occurred
    close(in);
    close(out);

    return true;
}

//Check the length of the pathName, which the maximum lenght of it cannot be more than 255.
bool Utils::isfileNameTooLong(const QString &fileName)
{
    //    qDebug() << "filename and length----------------------:" << fileName << fileName.toUtf8().length();

    if (fileName.toUtf8().length() > 255) {
        qDebug() << "FileName" << fileName << "is too long-----------------" << fileName.toUtf8().length();
        return true;
    } else {
        return false;
    }
}

//拷贝文件：
bool Utils::cutFileDirToPath(QString sourcePath ,QString toDir, bool coverIfExist)
{
    QFileInfo fileInfo(sourcePath);
    if (!fileInfo.exists()) {
        emit taskError(TaskCut);
        return false;
    }

    QString fileName = fileInfo.fileName();
    QDir targetDir(toDir);
    QString targetPath = targetDir.filePath(fileName);
    QFileInfo targetFileInfo = QFileInfo(targetPath);

    if (targetFileInfo.exists()) {
        if (coverIfExist) {
            if (targetFileInfo.isDir()) {
                targetDir.rmpath(targetPath);
            } else if (targetFileInfo.isFile()) {
                targetDir.remove(fileName);
            }
        } else {//new name
            targetPath = renameExistPath(targetPath, false);
        }
    }

    qDebug() << "cutFileDirToPath(): cut sourcePath to targetPath-------" << sourcePath << targetPath;

    if (isfileNameTooLong(QFileInfo(targetPath).fileName())) {
        qDebug() << "Utils::cutFileToPath() failed, the new file name is too long__________targetPath:" << targetPath;
        emit taskError(TaskCut, "ENAMETOOLONG");
        return false;
    }

    QDir r;

    //! On most file systems, rename() fails only if oldName does not exist,
    //! or if a file with the new name already exists. However,
    //! there are also other reasons why rename() can fail.
    //! For example, on at least one file system rename() fails if newName points to an open file.
    //! If oldName is a file (not a directory) that can't be renamed right away,
    //! Qt will try to copy oldName to newName and remove oldName.
    if(!r.rename(sourcePath, targetPath)) {
        emit taskError(TaskCut);
        qDebug() << "failed to move file "<<sourcePath<<" to "<<targetPath;
        return false;
    }

    if (fristCutedFile == "")
        fristCutedFile = QFileInfo(targetPath).fileName();

    return true;
}

//拷贝文件：
bool Utils::copyFileToPath(QString sourceFilePath ,QString toDir, bool coverFileIfExist)
{
    if (!QFile::exists(sourceFilePath)){
        emit taskError(TaskCopy);
        return false;
    }

    QString fileName = QFileInfo(sourceFilePath).fileName();
    QDir targetDir(toDir);
    QString targetFilePath = targetDir.filePath(fileName);

    if(QFile(targetFilePath).exists()) {
        if(coverFileIfExist){
            targetDir.remove(fileName);
        } else { // new name
            targetFilePath = renameExistPath(targetFilePath, false);
        }
    }

    if (isfileNameTooLong(QFileInfo(targetFilePath).fileName())) {
        qDebug() << "Utils::copyFileToPath() failed, the new file name is too long__________targetFilePath:" << targetFilePath;
        emit taskError(TaskCopy, "ENAMETOOLONG");
        return false;
    }

    if (fristCopyedFile == "") {
        fristCopyedFile = QFileInfo(targetFilePath).fileName();
    }

    copyedFileList << targetFilePath;

    /// 进行文件copy
//    targetFile.setFileName(targetDir.filePath(fileInfo.fileName()));
//    QFile fromFile(fileInfo.filePath());
    //qDebug() << "----- copy file to: " << fileInfo.filePath() << targetDir.filePath(fileInfo.fileName());
    //if(!fromFile.copy(targetFile.fileName())) {
    //if(!QFile::copy(fileInfo.filePath(),
        //targetDir.filePath(fileInfo.fileName()))){
    if(!doCopy(sourceFilePath, targetFilePath)) {
        qDebug() << "failed to copy " << targetFile.fileName();
        emit taskError(TaskCopy);
        return false;
    }
    return true;
}

//拷贝文件夹：
bool Utils::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{

    if(fromDir == toDir)
        return false;
    QString targetPath = QDir(toDir).filePath(QFileInfo(fromDir).fileName());
    QDir sourceDir(fromDir);
    if(!coverFileIfExist && QDir(targetPath).exists()) { // rename dir
        targetPath = renameExistPath(targetPath, true);
    }

    QDir targetDir(targetPath);

    if (fristCopyedFile == "") {
        fristCopyedFile = targetDir.dirName();
    }

    if(!targetDir.exists()){    /**< 如果目标目录不存在，则进行创建 */
        //check the directory name
        if (isfileNameTooLong(QFileInfo(targetPath).fileName())) {
            qDebug() << "Utils::copyDirectoryFiles() failed, the directory name is too long__________directory targetPath:" << targetPath;
            emit taskError(TaskCopy, "ENAMETOOLONG");
            return false;
        }

        if(!targetDir.mkpath(targetDir.absolutePath()))
        {
            qDebug()<<"1"<<targetDir.absolutePath();
            emit taskError(TaskCopy);
            return false;
        }
        //!
        //! why mkdir is ok,but QDir.mkDir is invalide when meet nestification dir
        //!
//        QString targetDirNew = targetDir.absolutePath().replace("(","\\\(");
//        targetDirNew = targetDirNew.replace(")","\\)");
//        qDebug()<<"create dir.... .....1"<<targetDirNew;
//        if(-1 == system(QString("mkdir -p " + targetDirNew).toLocal8Bit().constData()))
//        {
//            qDebug()<<"1"<<targetDir.absolutePath();
//            emit taskError(TaskCopy);
//            return false;
//        }
    }

    copyedFileList << targetPath;

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList) {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /**< 当为目录时，递归的进行copy */
            if(!copyDirectoryFiles(fileInfo.filePath(),
                targetDir.absolutePath()/*filePath(fileInfo.fileName())*/, //bug9604
                coverFileIfExist))
            {
                qDebug()<<"2";
                emit taskError(TaskCopy);
                return false;
            }
        }
        else{            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
//            targetFile.setFileName(targetDir.filePath(fileInfo.fileName()));
//            QFile fromFile(fileInfo.filePath());
//            if(fromFile.copy(targetFile.fileName())) {
            //qDebug() << "----- copy file to: " << fileInfo.filePath() << targetDir.filePath(fileInfo.fileName());
            QString targetFilePath = targetDir.filePath(fileInfo.fileName());

//            if (isfileNameTooLong(targetFilePath)) {
//                qDebug() << "Utils::copyDirectoryFiles() failed, the newFilePath is too long__________" << targetFilePath;
//                emit taskError(TaskCopy, "ENAMETOOLONG");
//                return false;
//            }

            if(!doCopy(fileInfo.filePath(), targetFilePath)) {
            //if(!QFile::copy(fileInfo.filePath(),
                //targetDir.filePath(fileInfo.fileName()))){
                qDebug()<<"3";
                emit taskError(TaskCopy);
                return false;
            }
        }
    }
    return true;
}

void Utils::run()
{
    qDebug()<<"file-manager Utils:doCopy entering...";
    if(taskType == TaskCopy) {
        totalCopySize = getAllFilesSize(taskFileList);
        qDebug()<<"file-manager Utils:==============totalCopySize:"<<totalCopySize<<"getAllFilesSize(taskFileList):"<<"task file list:"<<getAllFilesSize(taskFileList)<<taskFileList;
        if(totalCopySize>freeSpace(copyTarget))
        {
            qDebug()<<"space not enough!!!error!";
            emit taskError(TaskCopy,"full");
            return;
        }

        //qDebug() << "Total Size: " << totalCopySize;
        copyedSize = 0;
        copyProgress = 0;
        copyedFileList.clear();

        foreach (const QString &path, taskFileList) {
            if(isCancel)
                break;

            QFileInfo fi(path);
            if(fi.fileName() == "." || fi.fileName() == "..")
                continue;
            if(fi.isDir()) {
                if(path == copyTarget) // don't copy a dir to itself
                    continue;
                bool isOk = copyDirectoryFiles(path, copyTarget, false);
                qDebug()<<isOk;
            } else {
                copyFileToPath(path, copyTarget, false);
            }
        }

        if(isCancel) {
            if(taskType == TaskCopy) {
                //qDebug()<< Q_FUNC_INFO <<copyedFileList;
                foreach (const QString &path, copyedFileList) {
                    QFileInfo qfi(path);
                    if(QFileInfo(path).isDir()) {
                        QDir(qfi.filePath()).removeRecursively();
                    } else {
                        QFile::remove(qfi.path());
                    }
                }
            }
        }
        emit taskFinished(taskType);
    }
}

bool Utils::reWrite(QString path)
{
    QFile file(path);
    QFileInfo info(file);
    qDebug()<<info.absolutePath()<<","<<info.size();
    qint64 fileSize = info.size();

    if(!file.open(QIODevice::WriteOnly))
        return false;

    QByteArray str("");
    QDataStream out(&file);
    for(qint64 i = 0;i<fileSize;i++)
    {
        str.append("0");
    }
    file.write(str);
    file.close();
    qDebug()<<"after changed:"<<info.absolutePath()<<","<<info.size();
    QFileInfo info1(file);
    qDebug()<<"after changed:"<<info1.absolutePath()<<","<<info1.size();
    return true;
}

void Utils::taskCancel()
 {
    isCancel = true;
    //this->wait(5000);
}

bool Utils::hasFileExist(const QStringList &paths, const QString &destPath)
{
    foreach (const QString &path, paths) {
        QString fileName = QFileInfo(path).fileName();
        QDir targetDir(destPath);
        if(QDir(targetDir.filePath(fileName)).exists()) {
            return true;
        }
    }
    return false;
}
QString Utils::renameExistPath(QString path, bool isDir)
{
    QString basePath = path.left(path.lastIndexOf("/"));
    QString fileName = path.right(path.length() - path.lastIndexOf("/") - 1);
    QString baseName = isDir ? fileName : QFileInfo(path).baseName();
    int rindex = baseName.lastIndexOf(")");
    QString originName = rindex >= 0 ? baseName.left(rindex + 1) : baseName;
    //QString originName = baseName.left(baseName.lastIndexOf(" "));
    QString newPath;

    int index = 1;
    do {
        if(isDir)
            newPath = basePath + "/" + originName + QString("(%1)").arg(index);
        else {
            if (QFileInfo(path).completeSuffix().isEmpty())
                newPath = basePath + "/" + originName + QString("(%1)").arg(index);
            else {
                newPath = basePath + "/" + originName + QString("(%1).").arg(index) + QFileInfo(path).completeSuffix();
            }
        }
        index ++;
    } while (QFileInfo(newPath).exists());

    return newPath;
}

QString Utils::getMimeType(QString filepath)
{
    QMimeDatabase mdb;
    return mdb.mimeTypeForFile(filepath).name();
}

QString Utils::getFileType(QString filepath)
{
    QString mimetype = getMimeType(filepath);

    QStringList imglist;
    imglist << "image/bmp" << "image/gif" << "image/jpeg" << "image/png" << "image/tiff"; //"image/*"
    QStringList videolist;
    videolist << "video/mp4";
    QStringList musiclist;
    musiclist << "audio/mp4" << "audio/mpeg" << "audio/flac" << "audio/mp4a-latm"
              << "audio/ogg" << "audio/x-flac+ogg" << "audio/x-speex+ogg" << "audio/x-vorbis+ogg"
              << "audio/x-ogg" << "audio/x-oggflac" << "audio/vorbis" << "audio/x-vorbis"
              << "audio/x-flac" << "audio/x-wav" << "audio/x-aac" << "audio/x-amr"
              << "audio/x-ms-wma" << "audio/x-ms-wmv" << "audio/x-ape" << "audio/AMR";
    QStringList textlist;
    textlist << "text/plain";

    if(imglist.contains(mimetype)) {
        return "image";
    } else if(videolist.contains(mimetype)) {
        return "video";
    } else if(musiclist.contains(mimetype)) {
        return "music";
    } else if(textlist.contains(mimetype)) {
        return "text";
    } else {
        return "unknown";
    }
}

QStringList Utils::getImgFilesInDir(QString dir, QString sortby)
{
    Q_UNUSED(sortby);

    QDir di(dir);
    if(!di.isReadable() || !di.exists()) {
        qDebug() << Q_FUNC_INFO << "dir is not available";
        return QStringList();
    }

    QStringList fls = di.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDir::Time);

    QStringList imglist;
    imglist << "image/bmp" << "image/gif" << "image/jpeg" << "image/png" << "image/tiff";

//    QMutableListIterator<QString> i(fls);
//    while (i.hasNext()) {
//        QString mimetype = getMimeType(i.next());
//        if (!imglist.contains(mimetype))
//            i.remove();
//    }
//    return fls;

    QStringList re;
    foreach (QString file, fls) {
        QString mimetype = getMimeType(dir + "/" +file);//Must use the file absolute path, otherwise it returns a wrong type.
        if(imglist.contains(mimetype))
            re << di.absoluteFilePath(file);
    }
    return re;
}
