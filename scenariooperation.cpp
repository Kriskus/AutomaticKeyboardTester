#include "scenariooperation.h"

ScenarioOperation::ScenarioOperation(QObject *parent, int repeat, int delay) : QObject(parent) {
    timeDelay = delay;
    repeats = repeat;
}

void ScenarioOperation::prepareScenario() {
    for(int currentRepeat = 0; currentRepeat < repeats; currentRepeat++) {
        emit sendCurrentRepeat(currentRepeat + 1);
        line = 0;
        progress = 0;
        while(line < scenario.size()) {
            processingLine = scenario.at(line);
            if(processingLine.contains("[OpisScenariusza]")) {
                scenarioDescription();
            } else if(processingLine.contains("[Komentarz]")) {
                scenarioCommentary();
            } else if(processingLine.contains("[CWK]") || processingLine.contains("[CNA]") ) {
                scenarioSetTime(processingLine);
            } else if(processingLine.contains("[ReakcjaUzytkownika]")) {
                scenarioWaitThread();
                if(endSession) {
                    break;
                }
            } else {
                scenarioRunRelay(processingLine);
            }
            line++;
        }
        if(timeDelay != 0) {
            QThread::sleep(timeDelay);
        }
    }
    emit sendStatusInformation("Zakończono scenariusz");
    emit finished();
}

void ScenarioOperation::stopThread() {
    qApp->processEvents();
    waitingReaction = false;
    endSession = true;
}

void ScenarioOperation::continueThread() {
    qApp->processEvents();
    waitingReaction = false;
}

void ScenarioOperation::getScenarioFile(QStringList scenarioReaded) {
    qApp->processEvents();
    scenario = scenarioReaded;
    emit countMaximumProgressBar(scenario);
    emit startScenario();
}

void ScenarioOperation::scenarioSetTime(QString timeLine) {
    QStringList splittedProcessingLine = scenarioLinePrepared(timeLine);
    if(splittedProcessingLine.size() > 1) {
        QString tempTime;
        if(splittedProcessingLine.at(1) == "CWK") {
            tempTime = splittedProcessingLine.at(2);
            CWK = tempTime.toInt();
        } else {
            tempTime = splittedProcessingLine.at(2);
            CNA = tempTime.toInt();
        }
        if(splittedProcessingLine.size() > 4) {
            if(splittedProcessingLine.at(3) == "CWK") {
                tempTime = splittedProcessingLine.at(4);
                CWK = tempTime.toInt();
            } else {
                tempTime = splittedProcessingLine.at(4);
                CNA = tempTime.toInt();
            }
        }
    }
    emit sendRelayTime(CWK, CNA);
    emit sendProgressInformation(++progress);
}

QStringList ScenarioOperation::scenarioLinePrepared(QString) {
    processingLine.replace("[CWK]", ";CWK;");
    processingLine.replace("[CNA]", ";CNA;");
    return processingLine.split(";");
}

void ScenarioOperation::scenarioDescription() {
    while(!processingLine.contains("[/OpisScenariusza]")) {
        processingLine = scenario.at(++line);
    }
}

void ScenarioOperation::scenarioCommentary() {
    while(!processingLine.contains("[/Komentarz]")) {
        processingLine = scenario.at(++line);
        if(processingLine != "[/Komentarz]") {
            emit sendScenarioCommentary(processingLine);
        }
    }
    emit sendProgressInformation(++progress);
}

void ScenarioOperation::scenarioWaitThread() {
    QString informationText = QString();
    while(!processingLine.contains("[/ReakcjaUzytkownika]")) {
        processingLine = scenario.at(++line);
        if(processingLine != "[/ReakcjaUzytkownika]") {
            informationText += processingLine + "\n";
        } else {
            emit sendScenarioStopComentary(informationText);
        }
    }
    emit sendProgressInformation(++progress);
    waitingReaction = true;
    while(waitingReaction) {
        qApp->processEvents();
        QThread::msleep(10);
    }
}

void ScenarioOperation::scenarioRunRelay(QString buttonLine) {
    int positionChar = 0;
    QString tempButton = "";
    while(positionChar < buttonLine.size()) {
        if(buttonLine.at(positionChar) == "[") {
            while(buttonLine.at(positionChar) != "]") {
                if(buttonLine.at(positionChar) != "[" && buttonLine.at(positionChar) != "]") {
                    tempButton.append(buttonLine.at(positionChar++));
                } else {
                    positionChar++;
                }
            }
            emit sendButton(tempButton.toUpper());
            tempButton.clear();
            if(buttonLine.at(positionChar) == "]") positionChar++;
            emit sendProgressInformation(++progress);
        } else {
            emit sendButton(buttonLine.at(positionChar++).toUpper());
            emit sendProgressInformation(++progress);
        }
    }
}
