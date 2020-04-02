#ifndef RELAYBUTTON_H
#define RELAYBUTTON_H

#include <QObject>
#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QTime>

class RelayButton : public QObject
{
    Q_OBJECT
public:
    explicit RelayButton(QObject *parent = nullptr);

private:
    int CWK;
    int CNA;

public slots:
    void runPin(int, int, int);
    void getNewTimeValue(int, int);

signals:
    void sendStatusInformation(QString);

};

#endif // RELAYBUTTON_H
