#ifndef SCENARIOOPERATION_H
#define SCENARIOOPERATION_H

#include <QObject>
#include <QThread>
#include <QApplication>
#include <QDebug>
#include <QThread>

class ScenarioOperation : public QObject
{
    Q_OBJECT
public:
    explicit ScenarioOperation(QObject *parent = nullptr, int repeat = 0, int delay = 0);

private:
    QStringList scenario;
    QString processingLine;
    bool waitingReaction;
    bool endSession;
    int repeats;
    int timeDelay;
    int CWK;
    int CNA;
    int line;
    int progress;

public slots:
    void prepareScenario();
    void stopThread();
    void continueThread();
    void getScenarioFile(QStringList);
    QStringList scenarioLinePrepared(QString);

    void scenarioDescription();
    void scenarioCommentary();
    void scenarioSetTime(QString);
    void scenarioWaitThread();
    void scenarioRunRelay(QString);

signals:
    void finished();
    void sendButton(QString);
    void startScenario();
    void sendRelayTime(int, int);
    void sendScenarioStop();
    void sendCurrentRepeat(int);
    void sendStatusInformation(QString);
    void sendScenarioCommentary(QString);
    void countMaximumProgressBar(QStringList);
    void sendProgressInformation(int);
    void sendMessageBoxInformation(QString);
    void sendScenarioStopComentary(QString);

};

#endif // SCENARIOOPERATION_H
