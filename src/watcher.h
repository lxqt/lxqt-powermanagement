#ifndef WATCHER_H
#define	WATCHER_H

#include <QObject>
#include <LXQt/Power>

class Watcher : public QObject
{

    Q_OBJECT

public:
    Watcher(QObject *parent = 0);
    virtual ~Watcher();

protected:
    void doAction(int action);

private:
    LxQt::Power mPower;

};

#endif	/* WATCHER_H */

