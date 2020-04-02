#ifndef PREPAREBUTTON_H
#define PREPAREBUTTON_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QApplication>
#include <QThread>

class PrepareButton : public QObject
{
    Q_OBJECT
public:
    explicit PrepareButton(QObject *parent = nullptr, QString pressedButton = QString());

public slots:
    void getButtonConfiguration(QStringList);
    void checkButtonPinConfiguration(QString);
    void pressButtonFromGenerator();

private:
    bool buttonFinded;
    QString button;

    QStringList buttonConfigurations;
    QStringList buttonConfigurationLine;

signals:
    void sendStatusInformation(QString);
    void sendPinConfiguration(int, int, int);
    void sendButton(QString);
    void finished();
};

#endif // PREPAREBUTTON_H
