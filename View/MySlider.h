#ifndef MINIPREMIERE_MYSLIDER_H
#define MINIPREMIERE_MYSLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include "MainWindow.h"

class MySlider : public QSlider
{
private:
    MainWindow *mainwindow;

public:
    MySlider(MainWindow *mainwindow);

protected:
    void mousePressEvent(QMouseEvent *ev);
};


#endif //MINIPREMIERE_MYSLIDER_H
