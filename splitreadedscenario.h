#ifndef SPLITREADEDSCENARIO_H
#define SPLITREADEDSCENARIO_H

#include <QObject>

class SplitReadedScenario : public QObject
{
    Q_OBJECT
public:
    explicit SplitReadedScenario(QObject *parent = nullptr);

public slots:
    void splitScenario(QStringList scenario);

    void prepareCommentary(QStringList scenario);
    void prepareDescription(QStringList scenario);
    void prepareUserReaction(QStringList scenario);
    void prepareRelay();

private:
    QStringList preparedScenario = QStringList();
    QString preparedLine = QString();
    int currentLine = 0;

signals:
    void sendPreparedScenario(QStringList);

};

#endif // SPLITREADEDSCENARIO_H
