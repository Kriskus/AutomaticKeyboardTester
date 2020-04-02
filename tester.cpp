#include "tester.h"
#include "ui_tester.h"

Tester::Tester(QWidget *parent, QString confFile) : QMainWindow(parent), ui(new Ui::Tester), buttonMapper(new QSignalMapper), threadRunning(false) {
    ui->setupUi(this);

    showAvailableScenario();
    signButtons();
    connect(ui->comboBoxScenarioList, SIGNAL(currentIndexChanged(int)), this, SLOT(showCurrentScenarioDesription(int)));

    connect(ui->pushButtonAddScenario, SIGNAL(clicked(bool)), this, SLOT(copyFileScenario()));
    connect(ui->pushButtonStartScenario, SIGNAL(clicked(bool)), this, SLOT(startScenario()));
    connect(ui->pushButtonClose, SIGNAL(clicked(bool)), this, SLOT(closeWindow()));

    connect(this, SIGNAL(sendMessageBoxInformation(QString)), this, SLOT(showMessageBoxInformation(QString)));
    connect(this, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));

    ui->pushButtonStop->hide();
    ui->pushButtonContinue->hide();
    ui->pushButtonStartScenario->setDisabled(1);
    ui->progressBar->setValue(0);

    fileConfiguration = confFile;
}

Tester::~Tester() {
    delete ui;
}

void Tester::showStatusInformation(QString information) {
    ui->statusbar->showMessage(information);
}

void Tester::showMessageBoxInformation(QString information) {
    QMessageBox::information(this, "Info", information, QMessageBox::Ok);
}

void Tester::updateProgressBar(int currentProgress) {
    ui->progressBar->setValue(currentProgress);
}

void Tester::updateScenarioList(QStringList scenarioList) {
    scenarioList.prepend("----------------");
    ui->comboBoxScenarioList->clear();
    ui->comboBoxScenarioList->addItems(scenarioList);
    ui->comboBoxScenarioList->setCurrentIndex(0);
}

void Tester::updateCurrentRepeat(int currentRepeat) {
    ui->statusbar->showMessage("Powtórzenie " + QString::number(currentRepeat) + " z " + QString::number(ui->spinBoxRepeat->value()), 3500);
}

void Tester::updateRepeats(int buttonValue) {
    ui->spinBoxRepeat->setValue(ui->spinBoxRepeat->value() + buttonValue);
}

void Tester::signButtons() {
    ui->pushButtonAdd10->setAutoRepeat(1);
    ui->pushButtonAdd100->setAutoRepeat(1);
    ui->pushButtonAdd1000->setAutoRepeat(1);
    ui->pushButtonSub10->setAutoRepeat(1);
    ui->pushButtonSub100->setAutoRepeat(1);
    ui->pushButtonSub1000->setAutoRepeat(1);
    buttonMapper->setMapping(ui->pushButtonAdd10, 10);
    buttonMapper->setMapping(ui->pushButtonAdd100, 100);
    buttonMapper->setMapping(ui->pushButtonAdd1000, 1000);
    buttonMapper->setMapping(ui->pushButtonSub10, -10);
    buttonMapper->setMapping(ui->pushButtonSub100, -100);
    buttonMapper->setMapping(ui->pushButtonSub1000, -1000);
    connect(buttonMapper, SIGNAL(mapped(int)), this, SLOT(updateRepeats(int)));
    connect(ui->pushButtonAdd10, SIGNAL(clicked(bool)), buttonMapper, SLOT(map()));
    connect(ui->pushButtonAdd100, SIGNAL(clicked(bool)), buttonMapper, SLOT(map()));
    connect(ui->pushButtonAdd1000, SIGNAL(clicked(bool)), buttonMapper, SLOT(map()));
    connect(ui->pushButtonSub10, SIGNAL(clicked(bool)), buttonMapper, SLOT(map()));
    connect(ui->pushButtonSub100, SIGNAL(clicked(bool)), buttonMapper, SLOT(map()));
    connect(ui->pushButtonSub1000, SIGNAL(clicked(bool)), buttonMapper, SLOT(map()));
}

void Tester::updateTextBrowser(QString description) {
    ui->textBrowser->setTextColor("black");
    ui->textBrowser->append(description);
}

void Tester::updateTextBrowserScenarioStop(QString stopInformation) {
    ui->textBrowser->setTextColor("red");
    ui->textBrowser->append(stopInformation);
    ui->textBrowser->setTextColor("black");
}

void Tester::showAvailableScenario() {
    QThread *showingScenarioThread;
    FileOperationRead *fileRead;

    showingScenarioThread = new QThread;
    fileRead = new FileOperationRead(nullptr, QString(), 0, QString(), "C:/scnr");
    fileRead->moveToThread(showingScenarioThread);

    connect(showingScenarioThread, SIGNAL(started()), fileRead, SLOT(readFileList()));
    connect(fileRead, SIGNAL(sendScenarioList(QStringList)), this, SLOT(updateScenarioList(QStringList)));
    connect(fileRead, SIGNAL(sendMessageBoxInformation(QString)), this, SLOT(showMessageBoxInformation(QString)));
    connect(fileRead, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));

    connect(fileRead, SIGNAL(finished()), showingScenarioThread, SLOT(quit()));
    connect(showingScenarioThread, SIGNAL(finished()), showingScenarioThread, SLOT(deleteLater()));
    connect(fileRead, SIGNAL(finished()), fileRead, SLOT(deleteLater()));

    showingScenarioThread->start();
}

void Tester::showCurrentScenarioDesription(int scenarioId) {
    if(scenarioId > 0) {
        ui->textBrowser->clear();
        QThread *scenarioDescriptionThread;
        FileOperationRead *fileRead;

        scenarioDescriptionThread = new QThread;
        fileRead = new FileOperationRead(nullptr, "C:/scnr/" + ui->comboBoxScenarioList->currentText(), 1);
        fileRead->moveToThread(scenarioDescriptionThread);

        connect(scenarioDescriptionThread, SIGNAL(started()), fileRead, SLOT(startReadOperations()));
        connect(fileRead, SIGNAL(sendScenarioDescription(QString)), this, SLOT(updateTextBrowser(QString)));
        connect(fileRead, SIGNAL(sendMessageBoxInformation(QString)), this, SLOT(showMessageBoxInformation(QString)));
        connect(fileRead, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));

        connect(fileRead, SIGNAL(finished()), scenarioDescriptionThread, SLOT(quit()));
        connect(scenarioDescriptionThread, SIGNAL(finished()), scenarioDescriptionThread, SLOT(deleteLater()));
        connect(fileRead, SIGNAL(finished()), fileRead, SLOT(deleteLater()));

        scenarioDescriptionThread->start();

        ui->pushButtonStartScenario->setEnabled(1);
    }
    else {
        emit ui->textBrowser->setText("Wybierz scenariusz...");
    }
}

void Tester::copyFileScenario() {
    emit sendMessageBoxInformation("Po wybraniu pliku scenariusza,\nzostanie on umieszczony w katalogu wraz z innymi scenariuszami.");
    QString fileToCopy = QFileDialog::getOpenFileName(this, "Wybierz nowy plik z konfiguracją", QString(), "*.txt");
    QStringList fileName = fileToCopy.split("/");
    QFile fileCopyFrom(fileToCopy);
    QFile fileCopyTarget("C:/scnr/" + fileName.last());
    if(!fileCopyFrom.copy(fileCopyTarget.fileName())) {
        emit sendMessageBoxInformation(fileCopyFrom.errorString());
    } else {
        showAvailableScenario();
        ui->comboBoxScenarioList->setCurrentText(fileName.last());
    }
}

void Tester::startScenario() {
    ui->textBrowser->append("\n");

    if(threadRunning) {
        emit sendMessageBoxInformation("Operacja w trakcie, poczekaj na zakończenie...");
    } else {
        ui->textBrowser->clear();
        ui->pushButtonStartScenario->setEnabled(0);
        ui->progressBar->setValue(0);
        ui->comboBoxScenarioList->setEnabled(0);
        ui->pushButtonAddScenario->setEnabled(0);

        QThread *ScenarioThread;
        FileOperationRead *fileRead;
        ScenarioOperation *scenario;
        PrepareButton *buttonCheck;
        RelayButton *relay;
        CheckScenario *scenarioCheck;

        ScenarioThread = new QThread;
        fileRead = new FileOperationRead(nullptr, "C:/scnr/" + ui->comboBoxScenarioList->currentText(), 2, fileConfiguration);
        scenario = new ScenarioOperation(nullptr, ui->spinBoxRepeat->value(), ui->spinBoxDelayTime->value());
        buttonCheck = new PrepareButton;
        relay = new RelayButton;
        scenarioCheck = new CheckScenario;

        fileRead->moveToThread(ScenarioThread);
        scenario->moveToThread(ScenarioThread);
        buttonCheck->moveToThread(ScenarioThread);
        relay->moveToThread(ScenarioThread);
        scenarioCheck->moveToThread(ScenarioThread);

        connect(ScenarioThread, SIGNAL(started()), fileRead, SLOT(startReadOperations()));

        connect(fileRead, SIGNAL(sendButtonConfigurations(QStringList)), buttonCheck, SLOT(getButtonConfiguration(QStringList)));
        connect(fileRead, SIGNAL(sendScenario(QStringList)), scenarioCheck, SLOT(checkScenarioSize(QStringList)));
        connect(fileRead, SIGNAL(sendScenario(QStringList)), scenario, SLOT(getScenarioFile(QStringList)));

        connect(scenarioCheck, SIGNAL(sendMaximumProgressBarValue(int)), ui->progressBar, SLOT(setMaximum(int)));

        connect(scenario, SIGNAL(startScenario()), scenario, SLOT(prepareScenario()));
        connect(scenario, SIGNAL(sendScenarioCommentary(QString)), this, SLOT(updateTextBrowser(QString)));
        connect(scenario, SIGNAL(sendScenarioStop(QString)), this, SLOT(updateTextBrowserScenarioStop(QString)));
        connect(scenario, SIGNAL(sendButton(QString)), buttonCheck, SLOT(checkButtonPinConfiguration(QString)));
        connect(scenario, SIGNAL(sendRelayTime(int,int)), relay, SLOT(getNewTimeValue(int,int)));
        connect(buttonCheck, SIGNAL(sendPinConfiguration(int,int,int)), relay, SLOT(runPin(int,int,int)));

        connect(fileRead, SIGNAL(sendMessageBoxInformation(QString)), this, SLOT(showMessageBoxInformation(QString)));
        connect(fileRead, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));
        connect(scenario, SIGNAL(sendMessageBoxInformation(QString)), this, SLOT(showMessageBoxInformation(QString)));
        connect(scenario, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));
        connect(buttonCheck, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));
        connect(relay, SIGNAL(sendMessageBoxInformation(QString)), this, SLOT(showMessageBoxInformation(QString)));
        connect(relay, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));

        connect(scenario, SIGNAL(sendProgressInformation(int)), ui->progressBar, SLOT(setValue(int)));
        connect(scenario, SIGNAL(sendCurrentRepeat(int)), this, SLOT(updateCurrentRepeat(int)));

        connect(scenario, SIGNAL(sendScenarioStopComentary(QString)), this, SLOT(pauseScenario(QString)));
        connect(this, SIGNAL(continueThread()), scenario, SLOT(continueThread()), Qt::DirectConnection);
        connect(ui->pushButtonClose, SIGNAL(clicked(bool)), this, SLOT(stopScenario()));
        connect(this, SIGNAL(stopThread()), scenario, SLOT(stopThread()));

        connect(scenario, SIGNAL(finished()), this, SLOT(endScenario()));
        connect(scenario, SIGNAL(finished()), ScenarioThread, SLOT(quit()));
        connect(ScenarioThread, SIGNAL(finished()), ScenarioThread, SLOT(deleteLater()));
        connect(scenario, SIGNAL(finished()), fileRead, SLOT(deleteLater()));
        connect(scenario, SIGNAL(finished()), buttonCheck, SLOT(deleteLater()));
        connect(scenario, SIGNAL(finished()), relay, SLOT(deleteLater()));
        connect(scenario, SIGNAL(finished()), scenario, SLOT(deleteLater()));

        threadRunning = true;
        ScenarioThread->start();
    }
}

void Tester::pauseScenario(QString informationText) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Oczekiwanie na reakcję...", informationText + "\nCzy kontynuować?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) emit continueThread();
    else stopScenario();
}

void Tester::stopScenario() {
    emit stopThread();
    ui->progressBar->setValue(0);
    ui->textBrowser->clear();
    ui->textBrowser->setTextColor("red");
    ui->textBrowser->append("\n\n\n\n"
                            "**************************\n"
                            "**        Wątek przewany        **\n"
                            "**************************\n");
    ui->textBrowser->setTextColor("red");
    endScenario();
}

void Tester::endScenario() {
    threadRunning = false;
    ui->comboBoxScenarioList->setEnabled(1);
    ui->pushButtonAddScenario->setEnabled(1);
    ui->pushButtonStartScenario->setEnabled(1);
}

void Tester::closeWindow() {
    emit finished();
    this->close();
}
