#include "listmodel.h"
#include <QFileInfo>
#include <QDir>

ListModel::ListModel(QObject *parent) : QAbstractListModel(parent)
{
    qDebug() <<__FUNCTION__<<__LINE__<< "list";
    mIndex = 0;
    mCurrentIndex = -1;             //设初值
}

int ListModel::currentIndex() const
{
    return mCurrentIndex;
}

int ListModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    qDebug()<<"=======mList.count()======"<<mList.count();
    return mList.count();
}

QUrl ListModel::getcurrentPath() const
{
    if(mList.size() > mCurrentIndex && mCurrentIndex >= 0) {//是否越界
        qDebug() <<"current path: "<<mList.at(mCurrentIndex).getPath();
        return mList.at(mCurrentIndex).getPath();
    }
    return QUrl();
}

QString ListModel::getcurrentSuffix() const
{
    if(mList.size() > mCurrentIndex && mCurrentIndex >= 0) {//是否越界
        qDebug() <<"current suffix: "<<mList.at(mCurrentIndex).getSuffix();
        return mList.at(mCurrentIndex).getSuffix();
    }
    return NULL;
}

QVariant ListModel::data(const QModelIndex & index, int role) const
{
    qDebug()<<"=======data======";
    if (index.row() < 0 || index.row() >= mList.count())//是否越界
        return QVariant();
    const FileInfos f = mList.at(index.row());
    switch (role) {
    case sizeRole:
        return f.getSize();
        qDebug()<<"=======size======"<<f.getSize();
    case titleRole:
        return f.getTitle();
    case suffixRole:
        return f.getSuffix();
    default:
        return QVariant();
    }
}

void ListModel::setCurrentIndex(const int & i)
{
    if (i >= mList.count() && mCurrentIndex != 0) {
        mCurrentIndex = 0;
        emit currentIndexChanged();
    } else if ((i >= 0) && (i < mList.count()) && (mCurrentIndex != i)) {
        mCurrentIndex = i;
        emit currentIndexChanged();
    }
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    qDebug() << "roleNames =";
    QHash<int, QByteArray> role;
    role[sizeRole] = "size";
    role[titleRole] = "filename";
    role[suffixRole] = "suffix";
    return role;
}

void ListModel::handleFileList(InfoList list)
{
    qDebug()<<"list.size: "<<list.size();
    if(list.size() == 0) {
        qDebug()<<"list is null !";
        return;
    } else {
        mList.clear();
        qDebug()<<__LINE__<<"list.size: "<<list.size();
        beginResetModel();
        mList = list;
        endResetModel();
    }
}
