#include "fileInfo.h"

FileInfos::FileInfos()
{
    mSize = 0;
    mTitle = "";
    mSuffix = "";
}
FileInfos::FileInfos(QUrl path, qint64 size, QString title, QString suffix)
{
    mPath = path;
    mSize = size;
    mTitle = title;
    mSuffix = suffix;
}

QUrl FileInfos::getPath() const
{
    return mPath;
}

qint64 FileInfos::getSize() const
{
    return mSize;
}


QString FileInfos::getTitle() const
{
    return mTitle;
}

QString FileInfos::getSuffix() const
{
    return mSuffix;
}
