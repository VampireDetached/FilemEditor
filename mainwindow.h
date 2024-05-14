#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mymenubar.h"
#include "QtMultimedia/QtMultimedia"
#include "QtMultimediaWidgets/QVideoWidget"
#include "myvideolist.h"

#include "QTableWidget"
#include "QGraphicsView"
#include "QGraphicsScene"
#include "QGraphicsLineItem"
#include "QPointF"
#include "ui_mainwindow.h"
#include "vitem.h"
#include "QMessageBox"
#include "QTextEdit"
#include "QFileDialog"
#include "QPushButton"
#include "QIcon"
#include "QList"
#include "QTime"
#include "QAction"
#include "QString"
#include "QDebug"
#include "QFileDialog"
#include "QSlider"
#include "QLabel"
#include "QTableWidget"
#include "QHeaderView"
#include "QGraphicsView"
#include "QGraphicsScene"
#include "QGraphicsRectItem"
#include "QMimeData"
#include "QTableView"
#include "QColor"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool eventFilter(QObject * obj,QEvent * eve);
    MyMenuBar * MenuBar;
    MyVideoList * videolist;
    QTableWidget * picturelib;
    QMediaPlayer * mainplayer;
    QString videofilename;
    QGraphicsScene * tl;
    QGraphicsView * timeline;
    QPushButton * pausebutton;
    QPen tp;
    QTableWidget * tlw;
    vItem * * vilist=new vItem * [100];
    vItem * currentPlace;
    QList<QString> picnames;
    QList<QString> videonames;
    int haspic[10]={0};
    int dura[10];
    int duration=0,min=0,sec=0,vicount=0,colcount=0,startvid=0,totaldura=0,inputpic=0,t=0, fromoutside=0;;
    void getduration(){duration=mainplayer->duration();min=duration/60000;sec=(duration-60000*min)/1000;};

private slots:
    void ShowInfo();
    void drawit(){
        int r=0,c=0;
        for(int i=0;i<10;i++)
        {
            if(tlw->item(r,i))
            {
                if(haspic[i]==1)
                {
                    tl->removeItem(vilist[i]);
                    delete vilist[i];
                    totaldura-=dura[i];
                    vicount--;
                    haspic[i]=0;
                    dura[i]=0;
                }
            }
        }
        while(c<10)
        {
            if(tlw->item(r,c)!=nullptr)
            {
                QString  f(tlw->item(r,c)->text());
                mainplayer->setSource(QUrl::fromLocalFile(f));
                qDebug()<<mainplayer->duration();
                dura[c]=mainplayer->duration();
                vilist[c]=new vItem(0,dura[c],totaldura+30*vicount*5000,f,tl,tp);
                haspic[c]=1;
                totaldura+=dura[c];
                vicount++;
                colcount++;
                if(100+10*colcount>255)colcount=0;
                QColor co;co.setGreen(100+20*colcount);tp.setColor(co);
            }
            c++;
        }
    };
    void deletevideo(int r,int c)
    {
        if(tlw->item(r,c)!=0)
        {
            delete tlw->item(r,c);
        }
    }
    void deletemainvideo(int r,int c)
    {
        if(videolist->item(r,c)!=0)
        {
            delete videolist->item(r,c);
        }
    }
    void updatevideo(){

    }
    void changevideo(int r,int c)
    {
          if(videolist->item(r,c))
        {
            videofilename=videonames[r];
            mainplayer->stop();
            mainplayer->setSource(QUrl::fromLocalFile(videofilename));

        }
    }


  private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
