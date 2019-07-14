#ifndef MINIPREMIERE_UPDATETIMESTAMPNOTIFICATION_H
#define MINIPREMIERE_UPDATETIMESTAMPNOTIFICATION_H

#include <iostream>
#include <memory>
#include "../../common/notification.h"

class MainWindow;

class UpdateTimeStampNotification : public Notification
{
private:
    MainWindow *mainwindow;
public:
    UpdateTimeStampNotification(MainWindow *mainwindow);
    virtual void Exec();
};


#endif //MINIPREMIERE_UPDATETIMESTAMPNOTIFICATION_H
