#include "fileoperationsave.h"

FileOperationSave::FileOperationSave(QObject *parent, QStringList fileInformation, QString fileName, bool scenario) : QObject(parent) {
    fileInformationToSave = fileInformation;
    fileNameToSave = fileName;
}

void FileOperationSave::saveScenarioFile() {
    emit sendStatusInformation("Rozpoczęcie zapisu pliku...");
    if(scenarioFile)
        fileInformationToSave = prepareScenarioToSave();
    QFile fileTosave(fileNameToSave);
    fileTosave.open(QIODevice::WriteOnly);
    if(fileTosave.isOpen()) {
        QTextStream fileSaveStream(&fileTosave);
        foreach (QString scenarioLineToSave, fileInformationToSave) {
            fileSaveStream << scenarioLineToSave + "\n";
        }
        fileTosave.close();
    } else
        emit sendMessageBoxInformation("Błąd utworzenia pliku\n" + fileTosave.errorString());
    emit finished();
}

QStringList FileOperationSave::prepareScenarioToSave() {
    while(scenarioLine < fileInformationToSave.size()) {
        line = fileInformationToSave.at(scenarioLine);
        if(line.contains("[Komentarz]")) {
            scenarioLine = prepareScenarioCommentary(scenarioLine);
        } else if(line.contains("[OpisScenariusza]")) {
            scenarioLine = prepareScenarioDescription(scenarioLine);
        } else if(line.contains("[ReakcjaUzytkownika]")) {
            scenarioLine = prepareScenarioUserReaction(scenarioLine);
        } else if(line.contains("[CWK]") || line.contains("[CNA]")) {
            prepareScenarioTime();
        } else {
            prepareScenarioRelay();
        }
        scenarioLine++;
    }
    if(!tempLine.isEmpty()) {
        scenarioTemp.append(tempLine);
        tempLine.clear();
    }
    return scenarioTemp;
}

int FileOperationSave::prepareScenarioCommentary(int currentLine) {
    if(!tempLine.isEmpty())
        clearTemp();
    scenarioTemp.append(line);
    while(!line.contains("[/Komentarz]")) {
        line = fileInformationToSave.at(++currentLine);
        scenarioTemp.append(line);
    }
    return currentLine;
}

int FileOperationSave::prepareScenarioDescription(int currentLine) {
    if(!tempLine.isEmpty())
        clearTemp();
    scenarioTemp.append(line);
    while(!line.contains("[/OpisScenariusza]")) {
        line = fileInformationToSave.at(++currentLine);
        scenarioTemp.append(line);
    }
    return currentLine;
}

int FileOperationSave::prepareScenarioUserReaction(int currentLine) {
    if(!tempLine.isEmpty())
        clearTemp();
    scenarioTemp.append(line);
    while(!line.contains("[/ReakcjaUzytkownika]")) {
        line = fileInformationToSave.at(++currentLine);
        scenarioTemp.append(line);
    }
    return currentLine;
}

void FileOperationSave::prepareScenarioTime() {
    if(!tempLine.isEmpty())
        clearTemp();
    scenarioTemp.append(line);
}

void FileOperationSave::prepareScenarioRelay() {
    if(tempLine.size() > 60) {
        scenarioTemp.append(tempLine);
        tempLine.clear();
    }
    tempLine.append(line);
}

void FileOperationSave::clearTemp() {
    scenarioTemp.append(tempLine);
    tempLine.clear();
}
