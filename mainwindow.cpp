#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    connect(ui->pushButtonTester, SIGNAL(clicked(bool)), this, SLOT(runTester()));
    connect(ui->pushButtonGenerator, SIGNAL(clicked(bool)), this, SLOT(runGenerator()));
    connect(ui->pushButtonAddConfiguration, SIGNAL(clicked(bool)), this, SLOT(copyConfigurationFile()));

    readAvailableConfigurationFiles();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::showStatusInformation(QString information) {
    ui->statusbar->showMessage(information, 2000);
}

void MainWindow::showMessageBoxInformation(QString information) {
    QMessageBox::information(this, "Info", information, QMessageBox::Ok);
}

void MainWindow::showAvailableConfigurationFiles(QStringList configurationList) {
    ui->comboBoxConfigurationType->clear();
    ui->comboBoxConfigurationType->addItem("----------------");
    ui->comboBoxConfigurationType->addItems(configurationList);
    ui->comboBoxConfigurationType->setCurrentIndex(0);
}

void MainWindow::runTester() {
    if(ui->comboBoxConfigurationType->currentIndex() > 0) {
        tester = new Tester(nullptr, ui->comboBoxConfigurationType->currentText());
        connect(tester, SIGNAL(finished()), this, SLOT(show()));
        connect(tester, SIGNAL(finished()), tester, SLOT(deleteLater()));
        this->hide();
        tester->show();
    } else {
        emit sendMessageBoxInformation("Nie wybrano pliku konfiguracji...");
    }
}

void MainWindow::runGeneratorScenario() {
    if(ui->comboBoxConfigurationType->currentIndex() > 0) {
        generator = new GeneratorScenario(nullptr, ui->comboBoxConfigurationType->currentText());
        connect(generator, SIGNAL(finished()), this, SLOT(show()));
        connect(generator, SIGNAL(finished()), generator, SLOT(deleteLater()));
        this->hide();
        generator->show();
    } else {
        emit sendMessageBoxInformation("Nie wybrano pliku konfiguracji...");
    }
}

void MainWindow::runGeneratorConfiguration() {
    configuration = new GenerateConfiguration();
    connect(configuration, SIGNAL(finished()), this, SLOT(show()));
    connect(configuration, SIGNAL(finished()), configuration, SLOT(deleteLater()));
    this->hide();
    configuration->show();
}

void MainWindow::readAvailableConfigurationFiles() {
    QThread *showingScenarioThread;
    FileOperationRead *fileRead;

    showingScenarioThread = new QThread;
    fileRead = new FileOperationRead(nullptr, QString(), 1, "", "C:/scnr/");
    fileRead->moveToThread(showingScenarioThread);

    connect(showingScenarioThread, SIGNAL(started()), fileRead, SLOT(readFileList()));
    connect(fileRead, SIGNAL(sendScenarioList(QStringList)), this, SLOT(showAvailableConfigurationFiles(QStringList)));
    connect(fileRead, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));

    connect(fileRead, SIGNAL(finished()), showingScenarioThread, SLOT(quit()));
    connect(showingScenarioThread, SIGNAL(finished()), showingScenarioThread, SLOT(deleteLater()));
    connect(fileRead, SIGNAL(finished()), fileRead, SLOT(deleteLater()));

    showingScenarioThread->start();
}

void MainWindow::copyConfigurationFile() {
    emit sendMessageBoxInformation("Po wybraniu pliku konfiguracji,\nzostanie on umieszczony w katalogu wraz z innymi plikami.");
    QString fileToCopy = QFileDialog::getOpenFileName(this, "Wybierz nowy plik z konfiguracją", QString(), "*.cfg");
    QStringList fileName = fileToCopy.split("/");
    QFile fileCopyFrom(fileToCopy);
    QFile fileCopyTarget("C:/scnr/" + fileName.last());
    if(!fileCopyFrom.copy(fileCopyTarget.fileName())) {
        emit sendMessageBoxInformation(fileCopyFrom.errorString());
    } else {
        readAvailableConfigurationFiles();
        ui->comboBoxConfigurationType->setCurrentText(fileName.last());
    }
}
