#ifndef WATCHER_H
#define WATCHER_H

#include <QObject>
#include <QEventLoop>
#include <LXQt/Power>
#include <LXQt/ScreenSaver>

class Watcher : public QObject
{
    Q_OBJECT

public:
    Watcher(QObject *parent = 0);
    virtual ~Watcher();

protected:
    void doAction(int action);

signals:
    void done();

private:
    LXQt::Power mPower;
    LXQt::ScreenSaver mScreenSaver;
    QEventLoop mLoop;
};

#endif // WATCHER_H
