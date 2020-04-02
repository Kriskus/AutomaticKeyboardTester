#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
#include <QFile>

#include "tester.h"
#include "generatorscenario.h"
#include "generateconfiguration.h"
#include "fileoperationread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Tester *tester;
    GeneratorScenario *generator;
    GenerateConfiguration *configuration;

private slots:
    void showStatusInformation(QString);
    void showMessageBoxInformation(QString);
    void showAvailableConfigurationFiles(QStringList);
    void readAvailableConfigurationFiles();

    void runTester();
    void runGeneratorScenario();
    void runGeneratorConfiguration();

    void copyConfigurationFile();
signals:
    void sendStatusInformation(QString);
    void sendMessageBoxInformation(QString);

};
#endif // MAINWINDOW_H
