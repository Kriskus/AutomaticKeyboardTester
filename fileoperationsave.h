#ifndef FILEOPERATIONSAVE_H
#define FILEOPERATIONSAVE_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTextCodec>

class FileOperationSave : public QObject
{
    Q_OBJECT
public:
    explicit FileOperationSave(QObject *parent = nullptr, QStringList fileInformation = QStringList(), QString fileName = QString(), bool scenario = true);

private:
    QString fileNameToSave;
    QStringList fileInformationToSave;

    int scenarioLine = 0;
    bool scenarioFile;
    QString line = QString();
    QString tempLine = QString();
    QStringList scenarioTemp = QStringList();

public slots:
    void saveScenarioFile();
    QStringList prepareScenarioToSave();

    int prepareScenarioCommentary(int);
    int prepareScenarioDescription(int);
    int prepareScenarioUserReaction(int);
    void prepareScenarioTime();
    void prepareScenarioRelay();

    void clearTemp();

signals:
    void finished();
    void sendStatusInformation(QString);
    void sendMessageBoxInformation(QString);
};

#endif // FILEOPERATIONSAVE_H
