#ifndef MYVIDEO_H
#define MYVIDEO_H

#include "QtMultimedia/QtMultimedia"
#include "QEvent"
#include "QWidget"
#include "QtMultimediaWidgets/QVideoWidget"
#include "QPushButton"
class MyVideo:public QVideoWidget
{
public:
    void MouseEventFunc(QMouseEvent * event);

    QPushButton * stopbutton;
};

#endif // MYVIDEO_H
