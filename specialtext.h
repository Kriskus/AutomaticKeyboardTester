#ifndef SPECIALTEXT_H
#define SPECIALTEXT_H

#include <QDialog>
#include <QObject>

namespace Ui {
class SpecialText;
}

class SpecialText : public QDialog
{
    Q_OBJECT

public:
    explicit SpecialText(QWidget *parent = nullptr, int task = 0);
    ~SpecialText();

private:
    Ui::SpecialText *ui;
    int taskType;
    QStringList specialText;

private slots:
    void confirmOperations();
    void cancelOperations();

signals:
    void sendSpecialText(QStringList);
    void sendStatusInformation(QString);
    void finish();
};

#endif // SPECIALTEXT_H
