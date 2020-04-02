#include "generatorscenario.h"
#include "ui_generatorscenario.h"

GeneratorScenario::GeneratorScenario(QWidget *parent, QString configurationFile) : QMainWindow(parent), ui(new Ui::GeneratorScenario) {
    ui->setupUi(this);
    buttonMapper = new QSignalMapper(this);
    updateListSize(ui->listWidget->count());
    buttonClickedTimes = 0;

    connect(ui->pushButtonListUp, SIGNAL(clicked(bool)), this, SLOT(moveLineUp()));
    connect(ui->pushButtonListDown, SIGNAL(clicked(bool)), this, SLOT(moveLineDown()));
    connect(ui->pushButtonClearList, SIGNAL(clicked(bool)), this, SLOT(clearList()));
    connect(ui->pushButtonRemoveLast, SIGNAL(clicked(bool)), this, SLOT(removeLastLine()));
    connect(ui->pushButtonLoadScenario, SIGNAL(clicked(bool)), this, SLOT(loadScenario()));
    connect(ui->pushButtonSaveScenario, SIGNAL(clicked(bool)), this, SLOT(saveScenario()));
    connect(ui->pushButtonComment, SIGNAL(clicked(bool)), this, SLOT(createComment()));
    connect(ui->pushButtonScenarioDescribe, SIGNAL(clicked(bool)), this, SLOT(createScenarioDescription()));
    connect(ui->pushButtonAddReaction, SIGNAL(clicked(bool)), SLOT(addUserReaction()));
    connect(ui->pushButtonCloseGenerator, SIGNAL(clicked(bool)), this, SLOT(closeWindow()));
    connect(ui->pushButtonTimeRelay, SIGNAL(clicked(bool)), this, SLOT(addTimeRelay()));

    configurationFilePath = "C:/scnr/" + configurationFile;

    loadConfigurationFile(configurationFilePath);
}

GeneratorScenario::~GeneratorScenario() {
    delete ui;
    delete buttonMapper;
}

void GeneratorScenario::showStatusInformation(QString information) {
    ui->statusbar->showMessage(information, 1500);
}

void GeneratorScenario::showMessageBoxInformation(QString information) {
    QMessageBox::information(this, "Info", information, QMessageBox::Ok);
}

void GeneratorScenario::closeWindow() {
    emit finished();
    close();
}

void GeneratorScenario::removeLastLine() {
    qDeleteAll(ui->listWidget->selectedItems());
    updateListSize(ui->listWidget->count());
}

void GeneratorScenario::clearList() {
    ui->listWidget->clear();
    updateListSize(ui->listWidget->count());
}

void GeneratorScenario::moveLineUp() {
    if(ui->listWidget->currentRow() > 0) {
        QListWidgetItem *current = ui->listWidget->currentItem();
        int currIndex = ui->listWidget->row(current);

        QListWidgetItem *prev = ui->listWidget->item(ui->listWidget->row(current) - 1);
        int prevIndex = ui->listWidget->row(prev);

        QListWidgetItem *temp = ui->listWidget->takeItem(prevIndex);
        ui->listWidget->insertItem(prevIndex, current);
        ui->listWidget->insertItem(currIndex, temp);
    }
}

void GeneratorScenario::moveLineDown() {
    if(ui->listWidget->currentRow() < ui->listWidget->count()) {
        QListWidgetItem *current = ui->listWidget->currentItem();
        int currIndex = ui->listWidget->row(current);

        QListWidgetItem *next = ui->listWidget->item(ui->listWidget->row(current) + 1);
        int nextIndex = ui->listWidget->row(next);

        QListWidgetItem *temp = ui->listWidget->takeItem(nextIndex);
        ui->listWidget->insertItem(currIndex, temp);
        ui->listWidget->insertItem(nextIndex, current);
    }
}

void GeneratorScenario::updateListSize(int listSize) {
    ui->labelSizeCount->setText(QString::number(listSize));
}

void GeneratorScenario::createButtons(QStringList buttonList) {
    if(buttonList.size() == 0) {
        emit sendMessageBoxInformation("Brak przycisków w wybranym pliku...");
    } else {
        buttonList = prepareButtons(buttonList);
        int row = 0;
        int column = 0;
        if(buttonList.size() > 72) {
            while(buttonList.size() > 72) {
                buttonList.removeLast();
            }
        }
        foreach (QString button, buttonList) {
            QPushButton *newKeyboardButton = new QPushButton;
            connect(newKeyboardButton, SIGNAL(clicked(bool)), buttonMapper, SLOT(map()));
            newKeyboardButton->setText(button);
            if(button.size() > 1) {
                button.prepend("[");
                button.append("]");
            }
            buttonMapper->setMapping(newKeyboardButton, button);
            newKeyboardButton->setMinimumHeight(50);
            newKeyboardButton->setMaximumHeight(50);
            newKeyboardButton->setMaximumWidth(45);
            newKeyboardButton->setMinimumWidth(45);
            ui->gridLayoutButtons->addWidget(newKeyboardButton, row++, column, Qt::AlignCenter);
            if(row == 8) {
                column++;
                row = 0;
            }
        }
        connect(buttonMapper, SIGNAL(mapped(QString)), this, SLOT(pressedButton(QString)));
    }
}

QStringList GeneratorScenario::prepareButtons(QStringList buttonList) {
    QStringList newButtonList;
    foreach (QString elementList, buttonList) {
        QStringList splitetedElement = elementList.split(";");
        newButtonList.append(splitetedElement.first());
    }
    return newButtonList;
}

void GeneratorScenario::addItemToList(QString pressedButton) {
    ui->listWidget->insertItem(ui->listWidget->count(), pressedButton);
    ui->listWidget->scrollToBottom();
    updateListSize(ui->listWidget->count());
}

void GeneratorScenario::addSpecialTextToList(QStringList textList) {
    foreach (QString lineToAddToList, textList) {
        addItemToList(lineToAddToList);
    }
}

void GeneratorScenario::pressedButton(QString pressedButton) {
    addItemToList(pressedButton);

    QThread *buttonPressThread;
    FileOperationRead *fileRead;
    PrepareButton *buttonCheck;
    RelayButton *relay;

    buttonPressThread = new QThread;
    fileRead = new FileOperationRead(nullptr, "", 0, configurationFilePath);
    buttonCheck = new PrepareButton(nullptr, pressedButton);
    relay = new RelayButton;

    fileRead->moveToThread(buttonPressThread);
    buttonCheck->moveToThread(buttonPressThread);
    relay->moveToThread(buttonPressThread);

    connect(buttonPressThread, SIGNAL(started()), fileRead, SLOT(readButtonConfiguration()), Qt::BlockingQueuedConnection);
    connect(buttonPressThread, SIGNAL(started()), buttonCheck, SLOT(pressButtonFromGenerator()));

    connect(buttonCheck, SIGNAL(sendButton(QString)), buttonCheck, SLOT(checkButtonPinConfiguration(QString)));
    connect(buttonCheck, SIGNAL(sendPinConfiguration(int,int,int)), relay, SLOT(runPin(int,int,int)));

    connect(fileRead, SIGNAL(sendMessageBoxInformation(QString)), this, SLOT(showMessageBoxInformation(QString)));
    connect(fileRead, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));
    connect(buttonCheck, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));
    connect(relay, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));

    connect(buttonCheck, SIGNAL(finished()), buttonPressThread, SLOT(quit()));
    connect(buttonPressThread, SIGNAL(finished()), buttonPressThread, SLOT(deleteLater()));
    connect(buttonCheck, SIGNAL(finished()), fileRead, SLOT(deleteLater()));
    connect(buttonCheck, SIGNAL(finished()), relay, SLOT(deleteLater()));
    connect(buttonCheck, SIGNAL(finished()), buttonCheck, SLOT(deleteLater()));

    buttonPressThread->start();
}

void GeneratorScenario::createComment() {
    specialTextWindow = new SpecialText(nullptr, 0);
    connect(specialTextWindow, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));
    connect(specialTextWindow, SIGNAL(sendSpecialText(QStringList)), this, SLOT(addSpecialTextToList(QStringList)));
    connect(specialTextWindow, SIGNAL(finished()), specialTextWindow, SLOT(deleteLater()));
    specialTextWindow->show();
}

void GeneratorScenario::addUserReaction() {
    specialTextWindow = new SpecialText(nullptr, 1);
    connect(specialTextWindow, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));
    connect(specialTextWindow, SIGNAL(sendSpecialText(QStringList)), this, SLOT(addSpecialTextToList(QStringList)));
    connect(specialTextWindow, SIGNAL(finished()), specialTextWindow, SLOT(deleteLater()));
    specialTextWindow->show();
}

void GeneratorScenario::createScenarioDescription() {
    specialTextWindow = new SpecialText(nullptr, 2);
    connect(specialTextWindow, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));
    connect(specialTextWindow, SIGNAL(sendSpecialText(QStringList)), this, SLOT(addSpecialTextToList(QStringList)));
    connect(specialTextWindow, SIGNAL(finish()), specialTextWindow, SLOT(deleteLater));
    specialTextWindow->show();
}

void GeneratorScenario::addTimeRelay() {
    timeScenarioWindow = new ScenarioTime();
    connect(timeScenarioWindow, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));
    connect(timeScenarioWindow, SIGNAL(sendTimeScenario(QString)), this, SLOT(addItemToList(QString)));
    connect(timeScenarioWindow, SIGNAL(finish()), timeScenarioWindow, SLOT(deleteLater()));
    timeScenarioWindow->show();
}

void GeneratorScenario::loadConfigurationFile(QString configurationFilePath) {
    QThread *loadConfigurationScenarioThread;
    FileOperationRead *fileRead;

    loadConfigurationScenarioThread = new QThread;
    fileRead = new FileOperationRead(nullptr, "", 0, configurationFilePath);
    fileRead->moveToThread(loadConfigurationScenarioThread);

    connect(loadConfigurationScenarioThread, SIGNAL(started()), fileRead, SLOT(readButtonConfiguration()));
    connect(fileRead, SIGNAL(sendButtonConfigurations(QStringList)), this, SLOT(createButtons(QStringList)));
    connect(fileRead, SIGNAL(sendMessageBoxInformation(QString)), this, SLOT(showMessageBoxInformation(QString)));
    connect(fileRead, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));

    connect(fileRead, SIGNAL(finished()), loadConfigurationScenarioThread, SLOT(quit()));
    connect(loadConfigurationScenarioThread, SIGNAL(finished()), loadConfigurationScenarioThread, SLOT(deleteLater()));
    connect(fileRead, SIGNAL(finished()), fileRead, SLOT(deleteLater()));

    loadConfigurationScenarioThread->start();
}

void GeneratorScenario::loadScenario() {
    QString filePath = QFileDialog::getOpenFileName(this, "Wybierz plik", QString(), "*.txt");

    QThread *loadScenarioThread;
    FileOperationRead *fileRead;
    SplitReadedScenario *splitScenario;

    loadScenarioThread = new QThread;
    fileRead = new FileOperationRead(nullptr, filePath, 0);
    splitScenario = new SplitReadedScenario();
    fileRead->moveToThread(loadScenarioThread);
    splitScenario->moveToThread(loadScenarioThread);

    connect(loadScenarioThread, SIGNAL(started()), fileRead, SLOT(startReadOperations()));
    connect(fileRead, SIGNAL(sendScenario(QStringList)), splitScenario, SLOT(splitScenario(QStringList)));
    connect(splitScenario, SIGNAL(sendPreparedScenario(QStringList)), this, SLOT(showLoadedScenario(QStringList)));
    connect(fileRead, SIGNAL(sendMessageBoxInformation(QString)), this, SLOT(showMessageBoxInformation(QString)));
    connect(fileRead, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));

    connect(fileRead, SIGNAL(finished()), loadScenarioThread, SLOT(quit()));
    connect(loadScenarioThread, SIGNAL(finished()), loadScenarioThread, SLOT(deleteLater()));
    connect(fileRead, SIGNAL(finished()), fileRead, SLOT(deleteLater()));

    loadScenarioThread->start();
}

void GeneratorScenario::saveScenario() {
    QStringList scenarioToSave;
    for (int listLine = 0; listLine < ui->listWidget->count(); ++listLine) {
        scenarioToSave.append(ui->listWidget->item(listLine)->text());
    }
    QString filePath = QFileDialog::getSaveFileName(this, "Wybierz plik", QString(), "*.txt");

    QThread *saveScenarioThread;
    FileOperationSave *saveFile;

    saveScenarioThread = new QThread();
    saveFile = new FileOperationSave(nullptr, scenarioToSave, filePath, true);

    saveFile->moveToThread(saveScenarioThread);

    connect(saveScenarioThread, SIGNAL(started()), saveFile, SLOT(saveScenarioFile()));
    connect(saveFile, SIGNAL(sendMessageBoxInformation(QString)), this, SLOT(showMessageBoxInformation(QString)));
    connect(saveFile, SIGNAL(sendStatusInformation(QString)), this, SLOT(showStatusInformation(QString)));

    connect(saveFile, SIGNAL(finished()), saveScenarioThread, SLOT(quit()));
    connect(saveScenarioThread, SIGNAL(finished()), saveScenarioThread, SLOT(deleteLater()));
    connect(saveFile, SIGNAL(finished()), saveFile, SLOT(deleteLater()));

    saveScenarioThread->start();
}

void GeneratorScenario::showLoadedScenario(QStringList scenario) {
    ui->listWidget->clear();
    ui->listWidget->addItems(scenario);
    updateListSize(ui->listWidget->count());
}
