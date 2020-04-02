#ifndef FILEOPERATIONREAD_H
#define FILEOPERATIONREAD_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTextCodec>

class FileOperationRead : public QObject
{
    Q_OBJECT
public:
    explicit FileOperationRead(QObject *parent = nullptr, QString scenarioFile = QString(), int taskNumber = 0, QString configurationFile = QString(), QString directoryPath = QString());
    bool descriptionReaded;
    int taskType;
    QString scenarioFilePath;
    QString scenarioDescription;
    QString configurationFilePath;
    QString currentApplicationDirectory;
    QStringList fileList;
    QStringList scenarioOperation;
    QStringList buttonConfiguration;

public slots:
    void startReadOperations();
    void readScenarioFile();
    void readFileList();
    void readScenarioDescription();
    void readButtonConfiguration();

private:
    int line = 0;

signals:
    void sendStatusInformation(QString);
    void sendMessageBoxInformation(QString);
    void sendScenario(QStringList);
    void sendScenarioList(QStringList);
    void sendScenarioDescription(QString);
    void sendButtonConfigurations(QStringList);
    void sendScenarioSizeInformations(int);
    void endThread();
    void finished();

};

#endif // FILEOPERATIONREAD_H
