#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <memory>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QTextStream>
#include <QPalette>
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLineEdit>
#include <QMessageBox>
#include "../common/command.h"
#include "./Notification/UpdateViewNotification.h"
#include "./Notification/UpdateTimeStampNotification.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Initialize the view
    MainWindow();
    void CreateMenuAndToolBar();
    void CreateStatusBar();
    void CreatePicLabel();
    void CreatePlayButton();
    void SetTimer();
    void SetSlider();
    void SetLayer();

    void UpdateQImage();
    void RestartTimer();
    //set command
    void SetOpenFileCommand(std::shared_ptr<CommandBase>);
    void SetFetchQImageCommand(std::shared_ptr<CommandBase>);
    void SetTimeJumpCommand(std::shared_ptr<CommandBase>);
    //set data
    void SetQImage(std::shared_ptr<QImage>);
    void SetFrameRate(std::shared_ptr<int>);
    void SetTimeDuration(std::shared_ptr<int>);

    //get notification
    std::shared_ptr<Notification> GetUpdateViewNotification();
    std::shared_ptr<Notification> GetUpdateTimeStampNotification();

private slots:
    void OnClick();
    void BackFiveSec();
    void ForwardFiveSec();
    void OnTimer();
    void SetLineEditValue();
    void OpenOperation();
    void ExitOperation();
    void HelpOperation();
    void AboutUsOperation();

private:
    QPushButton *button;
    QPushButton *back;
    QPushButton *forward;
    QLabel *pic;
    QSlider *slider;
    QLineEdit *start;
    QLineEdit *end;
    QTimer *timer;
    QPixmap picture;

    bool isLoaded;
    bool isPlay;

    std::shared_ptr<QImage> image;
    std::shared_ptr<int> framerate;
    std::shared_ptr<int> timeduration;
    std::shared_ptr<CommandBase> OpenFileCommand;
    std::shared_ptr<CommandBase> FetchQImageCommand;
    std::shared_ptr<CommandBase> TimeJumpCommand;
    std::shared_ptr<UpdateViewNotification> TheUpdateViewNotification;
    std::shared_ptr<UpdateTimeStampNotification> TheUpdateTimeStampNotification;
};
#endif
