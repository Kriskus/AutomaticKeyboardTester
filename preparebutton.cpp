#include "preparebutton.h"

PrepareButton::PrepareButton(QObject *parent, QString pressedButton) : QObject(parent) {
    button = pressedButton;
}

void PrepareButton::getButtonConfiguration(QStringList configuration) {
    qApp->processEvents();
    buttonConfigurations = configuration;
    emit sendStatusInformation("Zapisano konfigurację przycisków");
}

void PrepareButton::checkButtonPinConfiguration(QString button) {
    qApp->processEvents();
    QString tempLine;
    int line = 0;
    buttonFinded = false;
    while(!buttonFinded && line < buttonConfigurations.size()) {
        tempLine = buttonConfigurations.at(line++);
        buttonConfigurationLine = tempLine.split(";");
        if(buttonConfigurationLine.at(0) == button) {
            emit sendPinConfiguration(buttonConfigurationLine.at(1).toInt(), buttonConfigurationLine.at(2).toInt(), buttonConfigurationLine.at(3).toInt());
            buttonFinded = true;
        }
    }
}

void PrepareButton::pressButtonFromGenerator() {
    if(button.at(0) == "[") {
        button.remove(0, 1);
        button.remove(button.size()-1, 1);
    }
    emit sendButton(button);
    emit finished();
}


