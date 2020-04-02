#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QFile>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(!QDir("log").exists())
        QDir().mkdir("log");
    QString logTxt;
    switch (type) {
    case QtInfoMsg:
        logTxt = QString("Info: %1").arg(msg);
        break;
    case QtDebugMsg:
        logTxt = QString("Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        logTxt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        logTxt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        logTxt = QString("Fatal: %1").arg(msg);
        break;
    }
    QFile logFile("log/log.msg");
    logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream logStream(&logFile);
    logStream << logTxt << endl;
}

int main(int argc, char **argv)
{
    qInstallMessageHandler(myMessageOutput); // Install the handler
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
