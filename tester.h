#ifndef TESTER_H
#define TESTER_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>
#include <QSignalMapper>

#include "fileoperationread.h"
#include "scenariooperation.h"
#include "checkscenario.h"
#include "preparebutton.h"
#include "relaybutton.h"

namespace Ui {
class Tester;
}

class Tester : public QMainWindow
{
    Q_OBJECT

public:
    explicit Tester(QWidget *parent = nullptr, QString configurationFile = QString());
    ~Tester();

private:
    Ui::Tester *ui;
    QSignalMapper *buttonMapper;
    bool threadRunning;
    QString fileConfiguration;

private slots:
    void signButtons();
    void updateRepeats(int);
    void updateProgressBar(int);
    void updateTextBrowser(QString);
    void updateScenarioList(QStringList);
    void updateCurrentRepeat(int);
    void showStatusInformation(QString);
    void showMessageBoxInformation(QString);
    void updateTextBrowserScenarioStop(QString);

    void endScenario();
    void closeWindow();
    void stopScenario();
    void startScenario();
    void pauseScenario(QString informationText);
    void showAvailableScenario();
    void showCurrentScenarioDesription(int);
    void copyFileScenario();

signals:
    void sendStatusInformation(QString);
    void sendMessageBoxInformation(QString);
    void continueThread();
    void stopThread();
    void finished();

};

#endif // TESTER_H
