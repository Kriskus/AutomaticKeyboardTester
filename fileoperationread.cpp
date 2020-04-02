#include "fileoperationread.h"

FileOperationRead::FileOperationRead(QObject *parent, QString scenarioFile, int taskNumber, QString configurationFile, QString directoryPath) : QObject(parent), descriptionReaded(false) {
    scenarioFilePath = scenarioFile;
    taskType = taskNumber;
    configurationFilePath = configurationFile;
    currentApplicationDirectory = directoryPath;
}

void FileOperationRead::startReadOperations() {
    switch (taskType) {
    case 0:
        readScenarioFile();
        emit sendScenario(scenarioOperation);
        emit finished();
        break;

    case 1:
        readScenarioFile();
        readScenarioDescription();
        emit sendScenario(scenarioOperation);
        emit sendScenarioDescription(scenarioDescription);
        emit finished();
        break;

    case 2:
        readButtonConfiguration();
        readScenarioFile();
        emit sendScenario(scenarioOperation);
        emit sendButtonConfigurations(buttonConfiguration);
        emit finished();
        break;

    default:
        break;
    }
}

void FileOperationRead::readScenarioFile() {
    emit sendStatusInformation("Rozpoczęcie odczytu pliku scenariusza...");
    QFile fileScenarioToRead(scenarioFilePath);
    fileScenarioToRead.open(QIODevice::ReadOnly);
    if(fileScenarioToRead.isOpen()) {
        QTextStream readFileStream(&fileScenarioToRead);
        while(!readFileStream.atEnd()) {
            scenarioOperation.append(readFileStream.readLine());
        }
    } else {
        emit sendMessageBoxInformation("Błąd otwarcia pliku...\n" + fileScenarioToRead.errorString());
    }
    fileScenarioToRead.close();
}

void FileOperationRead::readFileList() {
    emit sendStatusInformation("Rozpoczęcie odczytu listy scenariusza...");
    QDir applicationPath(currentApplicationDirectory);
    if(taskType == 0)
        fileList = applicationPath.entryList(QStringList() << "*.txt" << "*.txt", QDir::Files);
    else
        fileList = applicationPath.entryList(QStringList() << "*.cfg" << "*.cfg", QDir::Files);
    emit sendScenarioList(fileList);
    emit finished();
}

void FileOperationRead::readScenarioDescription() {
    emit sendStatusInformation("Rozpoczęcie odczytu opisu scenariusza...");
    scenarioDescription.clear();
    while(line < scenarioOperation.size() && !descriptionReaded) {
        if(scenarioOperation.at(line++) == "[OpisScenariusza]") {
            while(scenarioOperation.at(line) != "[/OpisScenariusza]") {
                if(scenarioOperation.at(line) != "[/OpisScenariusza]" && scenarioOperation.at(line) != "[OpisScenariusza]")
                    scenarioDescription.append(scenarioOperation.at(line++) + "\n");
                if(scenarioOperation.at(line) == "[/OpisScenariusza]")
                    descriptionReaded = true;
            }
        }
    }
}

void FileOperationRead::readButtonConfiguration() {
    emit sendStatusInformation("Rozpoczęcie odczytu konfiguracji przycisków");
    QFile buttonFileRead(configurationFilePath);
    buttonFileRead.open(QIODevice::ReadWrite);
    if(buttonFileRead.isOpen()) {
        QTextStream fileStream(&buttonFileRead);
        while(!fileStream.atEnd()) {
            buttonConfiguration.append(fileStream.readLine());
        }
        emit sendButtonConfigurations(buttonConfiguration);
        buttonFileRead.close();
    } else {
        emit sendMessageBoxInformation("Nie udało się odczytać pliku konfiguracji\n" + buttonFileRead.errorString());
    }
}
