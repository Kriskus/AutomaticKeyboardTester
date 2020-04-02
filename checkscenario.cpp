#include "checkscenario.h"

CheckScenario::CheckScenario(QObject *parent) {

}

void CheckScenario::checkScenarioSize(QStringList scenarioReaded) {
    numbersOfOperation = 0;
    line = 0;
    scenario = scenarioReaded;

    while(line < scenario.size()) {
        processingLine = scenario.at(line);
        if(processingLine.contains("[OpisScenariusza]")) {
            scenarioDescription();
        } else if(processingLine.contains("[Komentarz]")) {
            scenarioCommentary();
        } else if(processingLine.contains("[CWK]") || processingLine.contains("[CNA]") ) {
            numbersOfOperation++;
        } else if(processingLine.contains("[ReakcjaUzytkownika]")) {
            scenarioWaitThread();
        } else {
            scenarioRunRelay(processingLine);
        }
        line++;
    }
    emit sendMaximumProgressBarValue(numbersOfOperation);
}


void CheckScenario::scenarioDescription() {
    while(!processingLine.contains("[/OpisScenariusza]")) {
        processingLine = scenario.at(++line);
    }
}

void CheckScenario::scenarioCommentary() {
    while(!processingLine.contains("[/Komentarz]")) {
        processingLine = scenario.at(++line);
        if(processingLine.contains("[/Komentarz]")) {
            numbersOfOperation++;
        }
    }
}

void CheckScenario::scenarioWaitThread() {
    while(!processingLine.contains("[/ReakcjaUzytkownika]")) {
        processingLine = scenario.at(++line);
        if(!processingLine.contains("[/ReakcjaUzytkownika]")) {
            numbersOfOperation++;
        }
    }
}

void CheckScenario::scenarioRunRelay(QString buttonLine) {
    int positionChar = 0;
    while(positionChar < buttonLine.size()) {
        if(buttonLine.at(positionChar) == "[") {
            while(buttonLine.at(positionChar) != "]") {
                positionChar++;
            }
            numbersOfOperation++;
        } else {
            numbersOfOperation++;
        }
        positionChar++;
    }
}
