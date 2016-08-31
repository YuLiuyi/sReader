#ifndef LISTMODEL_H
#define LISTMODEL_H
#include <QUrl>
#include <QDebug>
#include <QFileInfo>
#include <QAbstractListModel>
#include "fileInfo.h"

class ListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

    explicit ListModel(QObject *parent = 0);
    int currentIndex() const;
    Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const;		//总行数
    Q_INVOKABLE QUrl getcurrentPath() const;					//得到当前路径
    Q_INVOKABLE QString getcurrentSuffix() const;					//得到当前路径

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;	//用于在qml显示的内容

    void setCurrentIndex(const int & i);

    enum Role {
        titleRole = Qt::UserRole + 1,
        sizeRole,
        suffixRole,
    };

public slots:
    void handleFileList(InfoList);

signals:
    void currentIndexChanged();

private:
    int mIndex;
    int mCurrentIndex;
    InfoList mList;
    QHash<int, QByteArray>roleNames() const;
};



#endif // LISTMODEL_H
