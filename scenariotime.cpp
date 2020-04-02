#include "scenariotime.h"
#include "ui_scenariotime.h"

ScenarioTime::ScenarioTime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScenarioTime) {
    ui->setupUi(this);
    connect(ui->pushButtonOk, SIGNAL(clicked(bool)), this, SLOT(addTimeScenario()));
    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(cancelOperations()));
}

ScenarioTime::~ScenarioTime() {
    delete ui;
}

void ScenarioTime::addTimeScenario() {
    if(ui->checkBoxCWK->isChecked())
        timeScenario = "[CWK]" + QString::number(ui->spinBoxCWK->value());
    if(ui->checkBoxCNA->isChecked())
        timeScenario.append("[CNA]" + QString::number(ui->spinBoxCNA->value()));
    if(!timeScenario.isEmpty()) {
        emit sendTimeScenario(timeScenario);
        emit sendStatusInformation("Dodano programowanie czasu do scenariusza...");
        emit finished();
        this->close();
    }
}

void ScenarioTime::cancelOperations() {
    emit sendStatusInformation("Rezygnacja...");
    emit finished();
    this->close();
}
