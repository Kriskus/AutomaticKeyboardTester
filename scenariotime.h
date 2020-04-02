#ifndef SCENARIOTIME_H
#define SCENARIOTIME_H

#include <QDialog>

namespace Ui {
class ScenarioTime;
}

class ScenarioTime : public QDialog
{
    Q_OBJECT

public:
    explicit ScenarioTime(QWidget *parent = nullptr);
    ~ScenarioTime();

private:
    Ui::ScenarioTime *ui;
    QString timeScenario = QString();

private slots:
    void addTimeScenario();
    void cancelOperations();

signals:
    void sendTimeScenario(QString);
    void sendStatusInformation(QString);
    void finished();

};

#endif // SCENARIOTIME_H
