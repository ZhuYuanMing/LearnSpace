#include "UpdateTimeStampNotification.h"
#include "../MainWindow.h"

UpdateTimeStampNotification::UpdateTimeStampNotification(MainWindow *mainwindow) :
    Notification()
{
    this->mainwindow = mainwindow;
}

void UpdateTimeStampNotification::Exec()
{
    mainwindow->RestartTimer();
}