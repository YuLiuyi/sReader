#ifndef __SREADER_WORKSPACE__
#define __SREADER_WORKSPACE__

#include <QQuickView>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QQmlComponent>
#include <SyberosGuiCache>
#include <cgui_application.h>
#include <cworkspace.h>
#include <QProcess>
#include "listmodel.h"
#include "controller.h"
#include "utils.h"


using namespace SYBEROS;

class sReader_Workspace : public CWorkspace
{
    Q_OBJECT

private:
    QQuickView *m_view;
    Controller contrl;
    ListModel *mListModel;

private slots:
    void showProcessError(QProcess::ProcessError);
    void showProcessState(QProcess::ProcessState);

public:
    sReader_Workspace();

    // 应用启动结束时，回调此函数。根据传入的option，应用可以区分启动的方式。
    void onLaunchComplete(Option option, const QStringList& params);

};


#endif //__SREADER_WORKSPACE__
