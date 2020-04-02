#include "relaybutton.h"

RelayButton::RelayButton(QObject *parent) : QObject(parent) {
    CWK = 50;
    CNA = 50;
}

void RelayButton::runPin(int repeat, int pinA, int pinB) {
    qApp->processEvents();

    QThread::msleep(CNA);
    for (int currentRepeat = 0; currentRepeat < repeat; ++currentRepeat) {
        emit sendStatusInformation("PIN ON");
        // run relay by wiringPi
        QThread::msleep(CWK);
        emit sendStatusInformation("PIN OFF");
        // stop relay by wiringPi
        if(repeat > 1) {
            QThread::msleep(50);
        }
    }
}

void RelayButton::getNewTimeValue(int pressTime, int nextTime) {
    qApp->processEvents();
    emit sendStatusInformation("Ustawiono nowy czas przekaźników");
    CWK = pressTime;
    CNA = nextTime;
}
