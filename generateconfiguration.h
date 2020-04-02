#ifndef GENERATECONFIGURATION_H
#define GENERATECONFIGURATION_H

#include <QMainWindow>
#include <QThread>
#include <QMessageBox>
#include <QFileDialog>

#include "fileoperationsave.h"

namespace Ui {
class GenerateConfiguration;
}

class GenerateConfiguration : public QMainWindow
{
    Q_OBJECT

public:
    explicit GenerateConfiguration(QWidget *parent = nullptr);
    ~GenerateConfiguration();

private slots:
    void showStatusInformation(QString);
    void showMessageBoxInformation(QString);
    void createButtonConfiguration();
    void readButtonTable();
    void prepareButton(int row, int column, QString relayName);
    void prepareColumntPin();
    void prepareRowPin();
    void saveGeneratedConfiguration();
    void closeWindow();

private:
    Ui::GenerateConfiguration *ui;
    QStringList buttonConfiguration = QStringList();
    QStringList columnRelay = QStringList();
    QStringList rowRelay = QStringList();
    QString button = QString();
    int repeat;

signals:
    void finished();

};

#endif // GENERATECONFIGURATION_H
