#include "specialtext.h"
#include "ui_specialtext.h"

SpecialText::SpecialText(QWidget *parent, int task) : QDialog(parent), ui(new Ui::SpecialText) {
    ui->setupUi(this);
    taskType = task;

    connect(ui->pushButtonCancel, SIGNAL(clicked(bool)), this, SLOT(cancelOperations()));
    connect(ui->pushButtonSave, SIGNAL(clicked(bool)), this, SLOT(confirmOperations()));
}

SpecialText::~SpecialText() {
    delete ui;
}

void SpecialText::confirmOperations(){
    specialText.clear();
    if(taskType == 0) {
        specialText.append("[Komentarz]");
        specialText.append(ui->textEdit->toPlainText().split("\n"));
        specialText.append("[/Komentarz]");
        emit sendSpecialText(specialText);
    } else if (taskType == 1) {
        specialText.append("[ReakcjaUzytkownika]");
        specialText.append(ui->textEdit->toPlainText().split("\n"));
        specialText.append("[/ReakcjaUzytkownika]");
        emit sendSpecialText(specialText);
    } else {
        specialText.append("[OpisScenariusza]");
        specialText.append(ui->textEdit->toPlainText().split("\n"));
        specialText.append("[/OpisScenariusza]");
        emit sendSpecialText(specialText);
    }
    emit sendStatusInformation("Dodano tekst...");
    emit finish();
    this->close();
}

void SpecialText::cancelOperations() {
    emit sendStatusInformation("Rezygnacja...");
    emit finish();
    this->close();
}
