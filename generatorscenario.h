#ifndef GENERATORSCENARIO_H
#define GENERATORSCENARIO_H

#include <QMainWindow>
#include <QDialog>
#include <QObject>
#include <QSignalMapper>
#include <QThread>
#include <QFileDialog>
#include <QMessageBox>

#include "fileoperationread.h"
#include "fileoperationsave.h"
#include "specialtext.h"
#include "scenariotime.h"
#include "preparebutton.h"
#include "relaybutton.h"
#include "scenariooperation.h"
#include "splitreadedscenario.h"

namespace Ui {
class GeneratorScenario;
}

class GeneratorScenario : public QMainWindow
{
    Q_OBJECT

public:
    explicit GeneratorScenario(QWidget *parent = nullptr, QString configurationFile = QString());
    ~GeneratorScenario();

private:
    Ui::GeneratorScenario *ui;
    QSignalMapper *buttonMapper;
    QTimer *buttonTimer;
    SpecialText *specialTextWindow;
    ScenarioTime *timeScenarioWindow;

    int buttonClickedTimes;
    QString previousButtonPressed;
    QString configurationFilePath;

private slots:
    void showStatusInformation(QString);
    void showMessageBoxInformation(QString);
    void closeWindow();

    void removeLastLine();
    void clearList();
    void moveLineUp();
    void moveLineDown();
    void updateListSize(int);

    void createButtons(QStringList buttonList);
    QStringList prepareButtons(QStringList buttonList);
    void addItemToList(QString);
    void addSpecialTextToList(QStringList);
    void addUserReaction();
    void pressedButton(QString);
    void createComment();
    void createScenarioDescription();
    void addTimeRelay();

    void loadConfigurationFile(QString);
    void loadScenario();
    void saveScenario();
    void showLoadedScenario(QStringList);

signals:
    void finished();
    void sendMessageBoxInformation(QString);
    void sendStatusBarInformationText(QString);
};

#endif // GENERATORSCENARIO_H
