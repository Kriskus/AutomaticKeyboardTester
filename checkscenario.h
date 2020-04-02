#ifndef CHECKSCENARIO_H
#define CHECKSCENARIO_H

#include <QApplication>
#include <QObject>

class CheckScenario : public QObject {
    Q_OBJECT

public:
    explicit CheckScenario(QObject *parent = nullptr);

public slots:
    void checkScenarioSize(QStringList);

private:
    int numbersOfOperation;
    int line;

    QString processingLine;
    QStringList scenario;

    void scenarioDescription();
    void scenarioCommentary();
    void scenarioWaitThread();
    void scenarioRunRelay(QString);

signals:
    void sendMaximumProgressBarValue(int);

};

#endif // CHECKSCENARIO_H
