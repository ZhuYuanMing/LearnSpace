//
// Created by leixiao on 2019/7/14.
//

#include "MySlider.h"

MySlider::MySlider(MainWindow *mainwindow)
{
    this->mainwindow = mainwindow;
}

void MySlider::mousePressEvent ( QMouseEvent *event )
{
    if (event->button() == Qt::LeftButton)
    {
        int nowvalue;
        if (orientation() == Qt::Vertical){
            nowvalue = minimum() + ((maximum()-minimum()) * (height()-event->y())) / height();
            setValue(nowvalue);
        }
        else{
            nowvalue = minimum() + ((maximum()-minimum()) * event->x()) / width();
            setValue(nowvalue) ;
        }
        event->accept();
    }
    QSlider::mousePressEvent(event);
}