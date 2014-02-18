/* 
 * File:   Watcher.cpp
 * Author: christian
 * 
 * Created on 14. februar 2014, 18:34
 */

#include <lxqt/lxqtpower.h>
#include <QDebug>
#include "../config/powermanagementsettings.h"
#include "watcher.h"

Watcher::Watcher(QObject *parent) :
    QObject(parent)
{
}

Watcher::~Watcher()
{
}

void Watcher::doAction(int action)
{
    if (action > -1)
    {   
        qDebug() << "Watcher doing " << action;
        mPower.doAction((LxQt::Power::Action) action);
        qDebug() << "Watcher done " << action;
    }
}

