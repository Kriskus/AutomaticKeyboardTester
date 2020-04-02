#include "splitreadedscenario.h"

SplitReadedScenario::SplitReadedScenario(QObject *parent) : QObject(parent) {

}

void SplitReadedScenario::splitScenario(QStringList scenario) {
    while(currentLine < scenario.size()) {
        preparedLine = scenario.at(currentLine);
        if(preparedLine.contains("[Komentarz]")) {
            prepareCommentary(scenario);
        } else if(preparedLine.contains("[OpisScenariusza]")) {
            prepareDescription(scenario);
        } else if(preparedLine.contains("[ReakcjaUzytkownika]")) {
            prepareUserReaction(scenario);
        } else if(preparedLine.contains("[CWK]") || preparedLine.contains("[CNA]")) {
            preparedScenario.append(preparedLine);
        } else {
            prepareRelay();
        }
        currentLine++;
    }
    emit sendPreparedScenario(preparedScenario);
}

void SplitReadedScenario::prepareCommentary(QStringList scenario) {
    preparedScenario.append(preparedLine);
    while(!preparedLine.contains("[/Komentarz]")) {
        preparedLine = scenario.at(++currentLine);
        preparedScenario.append(preparedLine);
    }
}

void SplitReadedScenario::prepareDescription(QStringList scenario) {
    preparedScenario.append(preparedLine);
    while(!preparedLine.contains("[/OpisScenariusza]")) {
        preparedLine = scenario.at(++currentLine);
        preparedScenario.append(preparedLine);
    }
}

void SplitReadedScenario::prepareUserReaction(QStringList scenario) {
    preparedScenario.append(preparedLine);
    while(!preparedLine.contains("[/ReakcjaUzytkownika]")) {
        preparedLine = scenario.at(++currentLine);
        preparedScenario.append(preparedLine);
    }
}

void SplitReadedScenario::prepareRelay() {
    QString tempLine = QString();
    for(int currentChar = 0; currentChar < preparedLine.size(); currentChar++) {
        if(preparedLine.at(currentChar) == "[") {
            tempLine.append(preparedLine.at(currentChar));
            while(preparedLine.at(currentChar) != "]") {
                tempLine.append(preparedLine.at(++currentChar));
            }
            preparedScenario.append(tempLine);
        } else {
            preparedScenario.append(preparedLine.at(currentChar));
        }
        tempLine.clear();
    }
}

