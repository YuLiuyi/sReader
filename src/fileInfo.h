#ifndef FILEINFO_H
#define FILEINFO_H
#include <QUrl>
#include <QObject>

class FileInfos
{
public:
    FileInfos();
    explicit FileInfos(QUrl path, qint64 size, QString title, QString suffix);
    QUrl getPath() const;
    qint64 getSize() const;
    QString getTitle() const;
    QString getSuffix() const;


private:
    QUrl mPath;
    qint64 mSize;
    QString mTitle, mSuffix;
};

typedef QList <FileInfos> InfoList;

#endif // FILEINFO_H
