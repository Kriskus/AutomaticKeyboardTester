#include "generateconfiguration.h"
#include "ui_generateconfiguration.h"

GenerateConfiguration::GenerateConfiguration(QWidget *parent) : QMainWindow(parent), ui(new Ui::GenerateConfiguration) {
    ui->setupUi(this);
    for(int i=0; i<12; i++) {
        ui->tableWidget->setColumnWidth(i, 61.33);
        ui->tableWidget->setRowHeight(i, 33.41);
    }

    connect(ui->pushButtonBack, &QPushButton::clicked, this, &GenerateConfiguration::closeWindow);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &GenerateConfiguration::createButtonConfiguration);
}

GenerateConfiguration::~GenerateConfiguration() {
    delete ui;
}

void GenerateConfiguration::showStatusInformation(QString information) {
    ui->statusbar->showMessage(information, 1500);
}

void GenerateConfiguration::showMessageBoxInformation(QString information) {
    QMessageBox::information(this, "Info", information, QMessageBox::Ok);
}

void GenerateConfiguration::createButtonConfiguration() {
    buttonConfiguration.clear();
    columnRelay.clear();
    rowRelay.clear();
    prepareColumntPin();
    prepareRowPin();
    readButtonTable();
    saveGeneratedConfiguration();
}

void GenerateConfiguration::readButtonTable() {
    QTableWidgetItem *relay = new QTableWidgetItem();
    for(int row = 0; row < rowRelay.size(); row++) {
        for (int column = 0; column < columnRelay.size(); ++column) {
            relay = ui->tableWidget->item(row, column);
            if(relay) {
                prepareButton(row, column, relay->text());
            }
        }
    }
}

void GenerateConfiguration::prepareButton(int row, int column, QString relayName) {
    repeat = 0;
    button.clear();
    for (int currentChar = 0; currentChar < relayName.size(); ++currentChar) {
        if(relayName.at(currentChar) == "[") {
            while(relayName.at(currentChar) != "]") {
                button.append(relayName.at(++currentChar));
            }
            buttonConfiguration.append(button.remove(button.size()-1, 1) + ";" + rowRelay.at(row) + ";" + columnRelay.at(column) + ";" + QString::number(repeat));
        } else {
            button = relayName.at(currentChar);
            buttonConfiguration.append(button + ";" + rowRelay.at(row) + ";" + columnRelay.at(column) + ";" + QString::number(repeat));
        }
        repeat++;
    }
}

void GenerateConfiguration::prepareColumntPin() {
    columnRelay.append(ui->line1->text());
    columnRelay.append(ui->line2->text());
    columnRelay.append(ui->line3->text());
    columnRelay.append(ui->line4->text());
    columnRelay.append(ui->line5->text());
    columnRelay.append(ui->line6->text());
    columnRelay.append(ui->line7->text());
    columnRelay.append(ui->line8->text());
    columnRelay.append(ui->line9->text());
    columnRelay.append(ui->line10->text());
    columnRelay.append(ui->line11->text());
    columnRelay.append(ui->line12->text());
}

void GenerateConfiguration::prepareRowPin() {
    rowRelay.append(ui->line13->text());
    rowRelay.append(ui->line14->text());
    rowRelay.append(ui->line15->text());
    rowRelay.append(ui->line16->text());
    rowRelay.append(ui->line17->text());
    rowRelay.append(ui->line18->text());
    rowRelay.append(ui->line19->text());
    rowRelay.append(ui->line20->text());
    rowRelay.append(ui->line21->text());
    rowRelay.append(ui->line22->text());
    rowRelay.append(ui->line23->text());
    rowRelay.append(ui->line24->text());
}

void GenerateConfiguration::saveGeneratedConfiguration() {
    QString filePath = QFileDialog::getSaveFileName(this, "Wybierz plik", "C:/scnr/", "*.cfg");

    QThread *saveConfigurationThread;
    FileOperationSave *saveFile;

    saveConfigurationThread = new QThread();
    saveFile = new FileOperationSave(nullptr, buttonConfiguration, filePath, false);

    saveFile->moveToThread(saveConfigurationThread);

    connect(saveConfigurationThread, SIGNAL(started()), saveFile, SLOT(saveScenarioFile()));
    connect(saveFile, SIGNAL(sendMessageBoxInformation(QString)), this, SLOT(showMessageBoxInformation(QString)));
    connect(saveFile, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));

    connect(saveFile, SIGNAL(finished()), saveConfigurationThread, SLOT(quit()));
    connect(saveConfigurationThread, SIGNAL(finished()), saveConfigurationThread, SLOT(deleteLater()));
    connect(saveFile, SIGNAL(finished()), saveFile, SLOT(deleteLater()));

    saveConfigurationThread->start();
}

void GenerateConfiguration::closeWindow() {
    emit finished();
    this->close();
}
