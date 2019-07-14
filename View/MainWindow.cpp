#include "MainWindow.h"
#include <QDebug>

MainWindow::MainWindow():
    QMainWindow()
{
    resize(QSize(960, 600));

    QPalette palette(this->palette());
    palette.setColor(QPalette::Window, qRgb(180, 180, 180));
    this->setPalette(palette);

    CreateStatusBar();
    CreateMenuAndToolBar();
    CreatePicLabel();
    CreatePlayButton();
    SetTimer();
    SetSlider();
    SetLayer();

    TheUpdateViewNotification = std::make_shared<UpdateViewNotification>(this);
    TheUpdateTimeStampNotification = std::make_shared<UpdateTimeStampNotification>(this);

    isLoaded = false;
    isPlay = false;
}

void MainWindow::CreateStatusBar()
{
    QLabel *statusLabel = new QLabel();
    QPalette palette(statusLabel->palette());
    palette.setColor(QPalette::Window, Qt::gray);
    statusLabel->setPalette(palette);
    statusBar()->addWidget(statusLabel);
    statusBar()->setStyleSheet(QString("QStatusBar::item{border:0px;}"));
}

void MainWindow::CreateMenuAndToolBar()
{
    //File MenuBar and ToolBar
    QMenu *fileMenu = menuBar()->addMenu(tr("File"));
    QToolBar *fileTool = addToolBar(tr("File"));

    //open file action
    QAction *openAction = new QAction(QIcon("../View/image/open.png"), tr("Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a vedio file"));
    fileMenu->addAction(openAction);
    fileTool->addAction(openAction);
    connect(openAction, &QAction::triggered, this, &MainWindow::OpenOperation);

    //add a separator before "exit" in menu
    fileMenu->addSeparator();

    //exit action in file menu
    QAction *exitAction = new QAction(tr("Exit"));
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit now"));
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &MainWindow::ExitOperation);

    fileTool->addSeparator();

    //about menu
    QMenu *aboutMenu = menuBar()->addMenu(tr("About"));
    //QToolBar *aboutTool = addToolBar(tr("About"));

    //help action in about menu
    QAction *helpAction = new QAction(QIcon("../View/image/help.png"), tr("Help"), this);
    helpAction->setStatusTip(tr("Some description"));
    aboutMenu->addAction(helpAction);
    fileTool->addAction(helpAction);

    connect(helpAction, &QAction::triggered, this, &MainWindow::HelpOperation);

    fileTool->addSeparator();

    //about us action in about menu
    QAction *aboutUsAction = new QAction(QIcon("../View/image/developer.png"), tr("About us..."), this);
    aboutUsAction->setStatusTip("The developers");
    aboutMenu->addAction(aboutUsAction);
    fileTool->addAction(aboutUsAction);
    connect(aboutUsAction, &QAction::triggered, this, &MainWindow::AboutUsOperation);

    //statusBar()->showMessage(" ");
}

//for test
void MainWindow::CreatePicLabel()
{
    pic = new QLabel(tr("MiniPlayer"));
    pic->setAlignment(Qt::AlignCenter);
    pic->setFont(QFont("Microsoft YaHei", 40, 75));
    QPalette pale(pic->palette());
    pale.setColor(QPalette::Window, qRgb(100,100,100));
    pale.setColor(QPalette::WindowText, qRgb(150,150,150));
    pic->setPalette(pale);
    pic->setAutoFillBackground(true);
    pic->setMaximumSize(1350, 540);
    pic->setScaledContents(true);
    //layout()->addWidget(pic);
}

void MainWindow::CreatePlayButton()
{
    button = new QPushButton();
    button->setIcon(QIcon("../View/image/play.png"));
    button->setIconSize(QSize(40, 40));
    button->setStyleSheet(QString("QPushButton{border-radius:35px;}"));
    //layout()->addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(OnClick()));

    back = new QPushButton();
    back->setIcon(QIcon("../View/image/back.png"));
    back->setIconSize(QSize(40, 40));
    back->setStyleSheet(QString("QPushButton{border-radius:35px;}"));
    connect(back, SIGNAL(clicked()), this, SLOT(BackFiveSec()));

    forward = new QPushButton();
    forward->setIcon(QIcon("../View/image/forward.png"));
    forward->setIconSize(QSize(40, 40));
    forward->setStyleSheet(QString("QPushButton{border-radius:35px;}"));
    connect(forward, SIGNAL(clicked()), this, SLOT(ForwardFiveSec()));
}

void MainWindow::SetTimer()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
}

void MainWindow::SetLayer()
{
    QWidget *widget = new QWidget(this);
    QHBoxLayout *hLayout = new QHBoxLayout();
    QVBoxLayout *vLayout = new QVBoxLayout();
    QHBoxLayout *hLayout_button = new QHBoxLayout();


    hLayout->addWidget(start);
    hLayout->addWidget(slider);
    hLayout->addWidget(end);

    vLayout->addWidget(pic);
    vLayout->addLayout(hLayout);

    hLayout_button->addStretch();
    hLayout_button->addWidget(back);
    hLayout_button->addWidget(button);
    hLayout_button->addWidget(forward);
    hLayout_button->addStretch();
    hLayout_button->setSpacing(10);

    vLayout->addLayout(hLayout_button);
    widget->setLayout(vLayout);
    setCentralWidget(widget);
}

void MainWindow::SetSlider()
{
    slider = new QSlider(Qt::Horizontal);
    slider->setOrientation(Qt::Horizontal);
    slider->setMaximum(0);
    slider->setMinimum(0);
    slider->setValue(0);
    slider->setStyleSheet(QString("QSlider::handle:horizontal{border:0;width: 15px;margin: -7px -7px -7px -7px;color:#808080} QSlider::sub-page:horizontal{background: qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, stop:0 #808080, stop:0.25 #808080, stop:0.5 #808080, stop:1 #808080);}"));

    start = new QLineEdit("00:00");
    end = new QLineEdit("00:00");
    start->setStyleSheet(QString("background:transparent;border-style:outset;border-width:0"));
    end->setStyleSheet(QString("background:transparent;border-style:outset;border-width:0"));
    start->setEnabled(false);
    end->setEnabled(false);
    start->setFocusPolicy(Qt::NoFocus);
    end->setFocusPolicy(Qt::NoFocus);
    start->setFixedSize(QSize(50, 30));
    end->setFixedSize(QSize(50, 30));
    start->setAlignment(Qt::AlignHCenter);
    end->setAlignment(Qt::AlignHCenter);

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(SetLineEditValue()));
}

void MainWindow::SetLineEditValue()
{
    int pos = slider->value();
    QString str = QString("%1:%2").arg(pos / 60, 2, 10, QLatin1Char('0')).arg(pos % 60, 2, 10, QLatin1Char('0'));
    start->setText(str);
}

void MainWindow::OnTimer()
{
    static int amount = 0;

    FetchQImageCommand->Exec();

    if(amount == *framerate && amount != 0){
        amount = 0;
        slider->setValue(slider->value() + 1);
    }
    amount++;

    //play over
    if(slider->value() == *timeduration / 1000000 && amount == *framerate){
        slider->setValue(0);
        timer->stop();
        start->setText(QString("00:00"));
        button->setIcon(QIcon("../View/image/play.png"));
        isLoaded = false;
        isPlay = false;
    }
}

void MainWindow::OnClick()
{
    static int flag = 0;

    if(!isLoaded){
        TimeJumpCommand->SetParameters(0);
        timer->stop();
        TimeJumpCommand->Exec();
    }

    if(flag == 1){
        flag = 0;
        button->setIcon(QIcon("../View/image/pause.png"));
        timer->start(500 / *framerate);
        isPlay = true;
    }
    else{
        flag = 1;
        button->setIcon(QIcon("../View/image/play.png"));
        timer->stop();
    }
}

void MainWindow::ForwardFiveSec()
{
//    if(!isLoaded){
//        return;
//    }

    int nowtime = slider->value();
    int jumptime = nowtime + 5;

    if(jumptime >= *timeduration / 1000000){
        TimeJumpCommand->SetParameters(jumptime);
    }
    else{
        TimeJumpCommand->SetParameters(jumptime);
    }

    slider->setValue(jumptime);

    timer->stop();
    TimeJumpCommand->Exec();
}

void MainWindow::BackFiveSec()
{
//    if(!isLoaded){
//        return;
//    }

    int nowtime = slider->value();
    int jumptime = nowtime - 5;

    if(jumptime < 0){
        TimeJumpCommand->SetParameters(jumptime);
    }
    else{
        TimeJumpCommand->SetParameters(jumptime);
    }

    slider->setValue(jumptime);

    timer->stop();
    TimeJumpCommand->Exec();
}

void MainWindow::UpdateQImage()
{
    picture = QPixmap::fromImage(*image);
    picture.scaled(pic->size(), Qt::KeepAspectRatioByExpanding);
    pic->setPixmap(picture);
}

void MainWindow::RestartTimer()
{
    button->setIcon(QIcon("../View/image/pause.png"));
    timer->start(500 / *framerate);
    isPlay = true;
}

void MainWindow::SetOpenFileCommand(std::shared_ptr<CommandBase> OpenFileCommand)
{
    this->OpenFileCommand = OpenFileCommand;
}

void MainWindow::SetFetchQImageCommand(std::shared_ptr<CommandBase> FetchQImageCommand){
    this->FetchQImageCommand = FetchQImageCommand;
}

void MainWindow::SetTimeJumpCommand(std::shared_ptr<CommandBase> TimeJumpCommand){
    this->TimeJumpCommand = TimeJumpCommand;
}

void MainWindow::SetQImage(std::shared_ptr<QImage> image)
{
    this->image = image;
}

void MainWindow::SetTimeDuration(std::shared_ptr<int> timeduration)
{
    this->timeduration = timeduration;
}

void MainWindow::SetFrameRate(std::shared_ptr<int> framerate)
{
    this->framerate = framerate;
}

std::shared_ptr<Notification> MainWindow::GetUpdateViewNotification()
{
    return std::static_pointer_cast<Notification>(TheUpdateViewNotification);
}

std::shared_ptr<Notification> MainWindow::GetUpdateTimeStampNotification()
{
    return std::static_pointer_cast<Notification>(TheUpdateTimeStampNotification);
}

//need to be updated
void MainWindow::OpenOperation()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Open Video"), ".", tr("Video File(*.avi *.mp4)"));
    std::string path = filepath.toStdString();

    if(filepath == nullptr){
        return;
    }

    if(isLoaded){
        slider->setValue(0);
        timer->stop();
        start->setText(QString("00:00"));
        button->setIcon(QIcon("../View/image/play.png"));
        isLoaded = false;
    }

    OpenFileCommand->SetParameters(path);
    OpenFileCommand->Exec();

    slider->setMaximum(*timeduration / 1000000);
    QString str = QString("%1:%2").arg(*timeduration / 1000000 / 60, 2, 10, QLatin1Char('0')).arg(*timeduration / 1000000 % 60, 2, 10, QLatin1Char('0'));
    end->setText(str);

    isLoaded = true;

    button->setIcon(QIcon("../View/image/pause.png"));
    timer->start(500 / *framerate);
    isPlay = true;
}

void MainWindow::ExitOperation()
{
    switch(QMessageBox::question(this, tr("Exit"), tr("Do you really want to exit?"), QMessageBox::Yes | QMessageBox::No)){
        case QMessageBox::Yes:
            exit(0);
        case QMessageBox::No:
            break;
        default:
            break;
    }
}

void MainWindow::HelpOperation()
{
    const char message[] = "<h2>Supported:</h2>Play video</br><h2>Not supported yet:</h2>The audio<br/>The procedure control";
    QMessageBox helpmessage(QMessageBox::NoIcon, "Help", message);
    QPixmap helppic("../View/image/help.png");

    helpmessage.setIconPixmap(helppic.scaled(QSize(40, 40), Qt::KeepAspectRatio));
    helpmessage.exec();
}

void MainWindow::AboutUsOperation()
{
    qDebug() << "Press About us" << endl;

    const char message[] = "<h2>The developers:</h2>Zhu Yuanming<br/>Pang Siyuan<br/>Lei Xiao";
    QMessageBox aboutusmessage(QMessageBox::NoIcon, "Developer", message);
    QPixmap aboutuspic("../View/image/developer.png");

    aboutusmessage.setIconPixmap(aboutuspic.scaled(QSize(40, 40), Qt::KeepAspectRatio));
    aboutusmessage.exec();
}
