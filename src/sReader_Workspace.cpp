#include "sReader_Workspace.h"
#include <QDebug>
#include <QQmlContext>

sReader_Workspace::sReader_Workspace()
    : CWorkspace()
{


    qDebug()<<"progress test yuyuyu... ";
    qRegisterMetaType<InfoList>("InfoList");
    m_view = SYBEROS::SyberosGuiCache::qQuickView();
    QObject::connect(m_view->engine(), SIGNAL(quit()), qApp, SLOT(quit()));

    qDebug() << "test" <<__LINE__;
    mListModel = new ListModel;
    m_view->rootContext()->setContextProperty("listmodel", mListModel);

    m_view->rootContext()->setContextProperty("contrl", &contrl);
    QObject::connect(&contrl, SIGNAL(result(InfoList)), mListModel, SLOT(handleFileList(InfoList)));

    Utils *utils = new Utils;
    qRegisterMetaType<Utils::TaskType>("TaskType");

    qmlRegisterUncreatableType<Utils>("com.syberos.filemanager", 1, 0, "Utils", "expose TaskType type to qml"); //register enum type

    QQmlContext *c = m_view->rootContext();
    c->setContextProperty("fileUtils", utils);


    //拷贝内置files
    qDebug()<< "start to copy files.";
    QString program = "/bin/cp";
    QStringList arguments;
    arguments << "/data/apps/com.syberos.sreader/documents" << "/home/user/" << "-R";
    QProcess *myProcess = new QProcess();
    myProcess->start(program, arguments);

    qDebug()<< "arguments "<< arguments.at(0);
    connect(myProcess, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(showProcessState(QProcess::ProcessState)));
    connect(myProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(showProcessError(QProcess::ProcessError)));
    connect(myProcess, SIGNAL(finished(int,QProcess::ExitStatus)), &contrl, SLOT(getfiles()));

    m_view->setSource(QUrl("qrc:/qml/main.qml"));
    m_view->showFullScreen();

}
void sReader_Workspace::showProcessError(QProcess::ProcessError error)
{
    qDebug()<<"progress error : "<< error;
}

void sReader_Workspace::showProcessState(QProcess::ProcessState state)
{
    qDebug()<<"progress state : "<< state;
}

void sReader_Workspace::onLaunchComplete(Option option, const QStringList& params)
{
    Q_UNUSED(params)

    switch (option) {
    case CWorkspace::HOME:
        qDebug()<< "Start by Home";
        break;
    case CWorkspace::URL:
        break;
    case CWorkspace::EVENT:
        break;
    case CWorkspace::DOCUMENT:
        break;
    default:
        break;
    }
}

