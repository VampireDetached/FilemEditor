#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QCoreApplication"


bool MainWindow::eventFilter(QObject * obj,QEvent * eve)
{
    if(obj==this->videolist->viewport())
    {
        if(eve->type()==QEvent::Drop)
        {
            const QMimeData * mime=((QDropEvent*)eve)->mimeData();
            QByteArray encodedata = mime->data("application/x-qabstractitemmodeldatalist");
            if (encodedata.isEmpty())
            {
                   return false;
            }
            QDataStream stream(&encodedata, QIODevice::ReadOnly);
            while (!stream.atEnd())
            {
                int row, col;
                QMap<int, QVariant> roleDataMap;
                stream >> row >> col >> roleDataMap;
                QTableWidgetItem* pDropItem = this->videolist->itemAt(((QDropEvent*)eve)->position().toPoint());
                if (!pDropItem)
                    return true;

                if (pDropItem->row() == row)
                {
                    return true;
                }
                QString sn1 = this->videolist->item(row, 0)->data(Qt::DisplayRole).toString();
                QString sn2 =this->videolist->item(pDropItem->row(), 0)->data(Qt::DisplayRole).toString();
                if (sn1.isEmpty() || sn2.isEmpty())
                    return true;
                this->videolist->item(row, 0)->setData(Qt::DisplayRole, sn2);
                this->videolist->item(pDropItem->row(), 0)->setData(Qt::DisplayRole, sn1);
                return true;
            }
         }
         else if(eve->type()==QEvent::DragLeave)
         {
              fromoutside=1;
         }
         else
         {
             return QWidget::eventFilter(obj, eve);
         }

    }
    else if(obj==this->tlw->viewport())
    {
        if(eve->type()==QEvent::DragEnter)
        {

            ((QDragEnterEvent*)eve)->acceptProposedAction();
            return true;
        }
        else if(eve->type()==QEvent::Drop)
        {
            if(fromoutside)
            {
                fromoutside=0;
                return QWidget::eventFilter(obj, eve);
            }
            else
            {
                const QMimeData * mime=((QDropEvent*)eve)->mimeData();
                QByteArray encodedata = mime->data("application/x-qabstractitemmodeldatalist");
                if (encodedata.isEmpty())
                {
                    return false;
                }
                QDataStream stream(&encodedata, QIODevice::ReadOnly);
                while (!stream.atEnd())
                {
                    int row, col;
                    QMap<int, QVariant> roleDataMap;
                    stream >> row >> col >> roleDataMap;
                    QTableWidgetItem* pDropItem = this->tlw->itemAt(((QDropEvent*)eve)->position().toPoint());

                    if (!pDropItem)
                        return true;

                    if (pDropItem->column() == col)
                    {
                        return true;
                    }
                    QString sn1 = this->tlw->item(0,col)->data(Qt::DisplayRole).toString();
                    QString sn2 =this->tlw->item(0,pDropItem->column())->data(Qt::DisplayRole).toString();
                    if (sn1.isEmpty() || sn2.isEmpty())
                        return true;
                    this->tlw->item(0,col)->setData(Qt::DisplayRole, sn2);
                    this->tlw->item(0,pDropItem->column())->setData(Qt::DisplayRole, sn1);
                    return true;
                }
            }
         }

    }
    return QWidget::eventFilter(obj, eve);
}

void MainWindow::ShowInfo()
{
    QMessageBox::information(this,"信息","本软件由vampyr开发，仅供学习使用，如无授权禁止使用转载");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(820,600);

    MenuBar=new MyMenuBar(this);
    mainplayer=new QMediaPlayer(this);
    QVideoWidget * video=new QVideoWidget(this);
    QAudioOutput * audio=new QAudioOutput(this);
    video->setFixedSize(400,300);video->move(0,20);
    mainplayer->setVideoOutput(video);mainplayer->setAudioOutput(audio);
    QPushButton * pausebutton=new QPushButton(this);

    pausebutton->setFixedSize(20,20);pausebutton->move(0,320);pausebutton->setIcon(QIcon("://brouse.png"));
    connect(pausebutton,&QPushButton::clicked,[=](){
        if(mainplayer->playbackState()!=QMediaPlayer::PlayingState)
        {
            mainplayer->play();
            pausebutton->setIcon(QIcon("://pause.png"));
        }
        else
        {
            mainplayer->pause();
            pausebutton->setIcon(QIcon("://brouse.png"));
        }

    });
    QSlider * videobar=new QSlider(this);videobar->setSingleStep(1);videobar->setOrientation(Qt::Horizontal);
    videobar->setFixedSize(320,20);videobar->move(20,320);
    QLabel * timeLabel=new QLabel(this);timeLabel->setFixedSize(100,20);timeLabel->move(340,320);

    videolist=new MyVideoList(this);
    videolist->setFixedSize(400,300);videolist->move(400,20);



    tlw=new QTableWidget(this);tlw->setFixedSize(700,50);tlw->move(100,340);tlw->installEventFilter(this);
    QHeaderView * thheader=tlw->horizontalHeader();thheader->setHidden(true);
    QHeaderView * tvheader=tlw->verticalHeader();tvheader->setHidden(true);
    tlw->setSelectionBehavior(QAbstractItemView::SelectItems);tlw->setSelectionMode(QAbstractItemView::SingleSelection);
    tlw->setColumnCount(5);tlw->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tlw->setFocusPolicy(Qt::NoFocus);tlw->setAcceptDrops(true);
    for(int i=0;i<10;i++)tlw->setColumnWidth(i,300);
    tlw->setDragDropMode(QAbstractItemView::DragDrop);tlw->setDragEnabled(true);
    tlw->viewport()->installEventFilter(this);

    tl=new QGraphicsScene(this);tl->setBackgroundBrush(Qt::black);tl->setSceneRect(0,0,650,50);
    timeline=new QGraphicsView(tl,this);timeline->setFixedSize(700,60);timeline->move(100,390);
    QLabel * videoline=new QLabel(this);videoline->setFixedSize(100,50);videoline->move(0,340);videoline->setText("VideoName");videoline->setAlignment(Qt::AlignCenter);
    QLabel * audioline=new QLabel(this);audioline->setFixedSize(100,50);audioline->move(0,390);audioline->setText("Video");audioline->setAlignment(Qt::AlignCenter);
    QLabel * pictureline=new QLabel(this);pictureline->setFixedSize(100,50);pictureline->move(100,510);pictureline->setText("Picture");pictureline->setAlignment(Qt::AlignCenter);
    picturelib=new QTableWidget(this);picturelib->setFixedSize(410,110);picturelib->move(200,480);
    picturelib->installEventFilter(this);picturelib->setSelectionBehavior(QAbstractItemView::SelectItems);
    picturelib->setEditTriggers(QAbstractItemView::NoEditTriggers);picturelib->setFocusPolicy(Qt::NoFocus);picturelib->setAcceptDrops(true);
    picturelib->setDragDropMode(QAbstractItemView::DragDrop);picturelib->setDragEnabled(true);
    picturelib->viewport()->installEventFilter(this);picturelib->setColumnCount(8);picturelib->setRowCount(2);
    picturelib->horizontalHeader()->setHidden(1);picturelib->verticalHeader()->setHidden(1);
    for (int i=0;i<8;i++)picturelib->setColumnWidth(i,50);
    for (int i=0;i<2;i++)picturelib->setRowHeight(i,50);
    QColor x;x.setGreen(100);tp.setColor(x);tp.setWidth(30);

    connect(this->MenuBar->InputFiles,&QAction::triggered,[=](){

        videofilename=QFileDialog::getOpenFileName(this,"选择视频文件","c:");
        mainplayer->setSource(QUrl::fromLocalFile(videofilename));
        int rowcount=videolist->rowCount();videolist->insertRow(rowcount);
        videolist->setItem(rowcount,0,new QTableWidgetItem(videofilename,1));
        videonames<<videofilename;
        pausebutton->setIcon(QIcon("://brouse.png"));
    });

    connect(this->MenuBar->InputPic,&QAction::triggered,[=](){
        QString picfilename=QFileDialog::getOpenFileName(this,"选择图片文件","c:");
        bool findplace=0;
        for(int i=0;i<2;i++)
        {
            for(int j=0;j<8;j++)
            {
                if(picturelib->item(i,j)==nullptr)
                {
                    QIcon x=QIcon(picfilename);
                    picturelib->setItem(i,j,new QTableWidgetItem(x,"",1));
                    picnames<<picfilename;
                    picturelib->setIconSize(QSize(50,50));
                    findplace=1;
                    inputpic=1;
                }
                if(findplace)break;
            }
            if(findplace)break;
        }

    });

    connect(videolist,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(changevideo(int,int)));

    connect(mainplayer,&QMediaPlayer::durationChanged,[=](){
        pausebutton->setIcon(QIcon("://brouse.png"));
        this->getduration();
        videobar->setMaximum(mainplayer->duration());
        videobar->setValue(0);
        timeLabel->setText("00:00/"+QString::number(min)+":"+QString::number(sec));
    });    

    connect(mainplayer,&QMediaPlayer::positionChanged,[=](){
        videobar->setValue(mainplayer->position());
        int minnow=mainplayer->position()/60000;
        int secnow=(mainplayer->position()-60000*minnow)/1000;
        timeLabel->setText(QString::number(minnow)+":"+QString::number(secnow)+"/"+QString::number(min)+":"+QString::number(sec));
    });

    connect(videobar,&QSlider::valueChanged,[=](){
        mainplayer->setPosition(videobar->value());
    });

    connect(this->MenuBar->BuildTimeLine,SIGNAL(triggered(bool)),this,SLOT(drawit()));

    connect(videolist,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(deletemainvideo(int,int)));

    connect(tlw,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(deletevideo(int,int)));

    connect(this->MenuBar->DivideAction,&QAction::triggered,[=](){
        if(mainplayer->source().toString()!="")
        {
            QWidget * inputtime=new QWidget;inputtime->setFixedSize(400,120);inputtime->setWindowIconText("Input Start Time And End Time");
            QLabel * stml=new QLabel(inputtime);stml->setText("开始（min）");stml->setFixedSize(80,30);stml->move(10,10);
            QLabel * stsl=new QLabel(inputtime);stsl->setText("开始（sec）");stsl->setFixedSize(80,30);stsl->move(110,10);
            QLabel * etml=new QLabel(inputtime);etml->setText("结束（min）");etml->setFixedSize(80,30);etml->move(210,10);
            QLabel * etsl=new QLabel(inputtime);etsl->setText("结束（sec）");etsl->setFixedSize(80,30);etsl->move(310,10);
            QTextEdit * starttimemin=new QTextEdit(inputtime);starttimemin->setFixedSize(80,30);starttimemin->move(10,40);
            QTextEdit * starttimesec=new QTextEdit(inputtime);starttimesec->setFixedSize(80,30);starttimesec->move(110,40);
            QTextEdit * endtimemin=new QTextEdit(inputtime);endtimemin->setFixedSize(80,30);endtimemin->move(210,40);
            QTextEdit * endtimesec=new QTextEdit(inputtime);endtimesec->setFixedSize(80,30);endtimesec->move(310,40);
            QPushButton * confirmtimebutton=new QPushButton(inputtime);confirmtimebutton->setFixedSize(40,40);
            confirmtimebutton->move(180,75);confirmtimebutton->setText("OK");
            inputtime->show();
            connect(confirmtimebutton,&QPushButton::clicked,[=](){
                QString program = QCoreApplication::applicationDirPath()+"/bin/ffmpeg.exe";
                QFile sourceFile(videofilename);
                if(!sourceFile.exists()){
                QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("找不到源文件"));
                return;
                }
                QString filenamesimple=videofilename.section("/",-1,-1);
                QString videofilenamesimple=filenamesimple.section(".",-2,-2);
                qDebug()<<videofilenamesimple;
                QString outputPath = QFileInfo(sourceFile).absolutePath() +"/"+videofilenamesimple+"clip.mp4";
                QFile destFile(outputPath);
                if(destFile.exists()){
                    destFile.remove();
                }
                int vsm=starttimemin->toPlainText().toInt();
                int vss=starttimesec->toPlainText().toInt();
                int vem=endtimemin->toPlainText().toInt();
                int ves=endtimesec->toPlainText().toInt();
                if(vsm>=0&&vsm<=60&&vss>=0&&vss<=60&&vem>=0&&vem<=60&&ves>=0&&ves<=60&&vem>=vsm)
                {
                    if(!(vem==vsm&&ves<vss))
                    {
                        QString startTime="00:"+QString::number(vsm)+":"+QString::number(vss);;
                        QString len;
                        if(ves>=vss)
                        len = "00:"+QString::number(vem-vsm)+":"+QString::number(ves-vss);
                        else
                        len="00:"+QString::number(vem-vsm-1)+":"+QString::number(60+ves-vss);
                        QStringList arguments;
                        arguments << "-i" << videofilename << "-r" << "25"<<"-ss";
                        arguments <<startTime<< "-t" << len<< outputPath;
                        qDebug()<<arguments;
                        QProcess *clipProcess = new QProcess(this);
//                        connect(clipProcess,SIGNAL(finished(int)),this,SLOT(clipVideoFinished(int)));
                        clipProcess->start(program, arguments);
                        QCoreApplication::processEvents();
                        QTimer * timer=new QTimer(this);
                        timer->start(3000);
                        connect(timer,&QTimer::timeout,[=](){
                                int rowcount=videolist->rowCount();videolist->insertRow(rowcount);
                                videolist->setItem(rowcount,0,new QTableWidgetItem(outputPath,1));
                                videofilename=outputPath;
                                mainplayer->setSource(QUrl::fromLocalFile(videofilename));
                                videonames<<videofilename;
                                pausebutton->setIcon(QIcon("://brouse.png"));
                                timer->stop();
                        });

                    }
                    else
                    QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("输入时间有误"));
                }
                else
                QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("输入时间有误"));
                delete inputtime;

            });

        }
    });

   connect(this->MenuBar->ComBineVideo,&QAction::triggered,[=](){
        QString program = QCoreApplication::applicationDirPath()+"/bin/ffmpeg.exe";
        int i=0;
        QString outputPath="";
        QString totalmpg="";
        while(i<=10)
        {
            if(tlw->item(0,i))
            {
                QString videonamecurrent=tlw->item(0,i)->text();
                QFile sourceFile(tlw->item(0,i)->text());
                if(!sourceFile.exists()){
                    QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("找不到源文件"));
                    return;
                }
                QString filenamesimple=videonamecurrent.section("/",-1,-1);
                QString videofilenamesimple=filenamesimple.section(".",-2,-2);
                if(outputPath=="")
                    outputPath = QFileInfo(sourceFile).absolutePath() +"/"+videofilenamesimple;
                else
                    outputPath+="&"+videofilenamesimple;
                if(totalmpg=="")
                    totalmpg=videofilenamesimple+".mpg";
                else
                    totalmpg+="|"+videofilenamesimple+".mpg";
                QStringList arguments;
                arguments << "-i" << videonamecurrent << "-qscale" << "4"<<videofilenamesimple+".mpg";
                QProcess *clipProcess = new QProcess(this);
                clipProcess->start(program, arguments);
            }
            i++;
        }
        if(outputPath!="")
        {
            QStringList arguments;
            arguments << "-i" << "concat:"+totalmpg<< "-c" << "copy"<<outputPath+".mpg";
            QProcess *clipProcess = new QProcess(this);
            clipProcess->start(program, arguments);
            QStringList argumentsfinal;
            argumentsfinal << "-i" << outputPath+".mpg"<< "-y" << "-qscale"<<0<<"-vcodec"<<"libx264"<<outputPath+".mpg";
            outputPath+=".mpg";
            QFile destFile(outputPath);
            if(destFile.exists()){
                destFile.remove();
            }
            qDebug()<<arguments;
            qDebug()<<argumentsfinal;
            QProcess *clipProcessfinal = new QProcess(this);
            clipProcessfinal->start(program, argumentsfinal);
            QTimer * timer=new QTimer(this);
            timer->start(3000);
            connect(timer,&QTimer::timeout,[=](){
                    int rowcount=videolist->rowCount();videolist->insertRow(rowcount);
                    videolist->setItem(rowcount,0,new QTableWidgetItem(outputPath,1));
                    videofilename=outputPath;
                    mainplayer->setSource(QUrl::fromLocalFile(videofilename));
                    videonames<<videofilename;
                    pausebutton->setIcon(QIcon("://brouse.png"));
                    timer->stop();
            });
        }
    });

   connect(this->MenuBar->AddCaptions,&QAction::triggered,[=](){
       if(mainplayer->source().toString()!="")
       {
           QString CaptionsFile=QFileDialog::getOpenFileName(this,"选择字幕文件","c:");
           QString program = QCoreApplication::applicationDirPath()+"/bin/ffmpeg.exe";
           QString filenamesimple=videofilename.section("/",-1,-1);
           QString videofilenamesimple=filenamesimple.section(".",-2,-2);
           filenamesimple=CaptionsFile.section("/",-1,-1);
           QString captionsfilenamesimple=filenamesimple.section(".",-2,-2);
           QString outputPath = QFileInfo(videofilename).absolutePath()+"/"+videofilenamesimple+"&"+captionsfilenamesimple+"_as_captions"+".mp4" ;
           QStringList arguments;
           arguments <<"-i" <<videofilename<<"-i"<<CaptionsFile<<"-c"<<"copy"<<"-c:s"<<"mov_text"<<outputPath;
           QProcess *clipProcess = new QProcess(this);
           clipProcess->start(program, arguments);
           QTimer * timer=new QTimer(this);
           timer->start(3000);
           connect(timer,&QTimer::timeout,[=](){
                   int rowcount=videolist->rowCount();videolist->insertRow(rowcount);
                   videolist->setItem(rowcount,0,new QTableWidgetItem(outputPath,1));
                   videofilename=outputPath;
                   mainplayer->setSource(QUrl::fromLocalFile(videofilename));
                   videonames<<videofilename;
                   pausebutton->setIcon(QIcon("://brouse.png"));
                   timer->stop();
           });
       }
   });

   connect(this->MenuBar->AddPicture,&QAction::triggered,[=](){
       if(inputpic)
       {
           if(picturelib->selectedItems()[0]!=nullptr)
           {
               int row=picturelib->selectedItems()[0]->row();
               int col=picturelib->selectedItems()[0]->column();
               QString picturename=picnames[8*row+col];
               QString program = QCoreApplication::applicationDirPath()+"/bin/ffmpeg.exe";
               QFile sourceFile(picturename);
               if(!sourceFile.exists()){
               QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("找不到源文件"));
               return;
               }
               QString filenamesimple=videofilename.section("/",-1,-1);
               QString videofilenamesimple=filenamesimple.section(".",-2,-2);
               filenamesimple=picturename.section("/",-1,-1);
               QString picturefilenamesimple=filenamesimple.section(".",-2,-2);

               QString outputPath = QFileInfo(sourceFile).absolutePath() +"/"+videofilenamesimple+picturefilenamesimple+".mp4";
               QFile destFile(outputPath);
               if(destFile.exists()){
                   destFile.remove();
               }
    //           int vsm=starttimemin->toPlainText().toInt();
    //           int vss=starttimesec->toPlainText().toInt();
    //           int vem=endtimemin->toPlainText().toInt();
    //           int ves=endtimesec->toPlainText().toInt();
    //           if(vsm>=0&&vsm<=60&&vss>=0&&vss<=60&&vem>=0&&vem<=60&&ves>=0&&ves<=60&&vem>=vsm)
    //           {
    //               if(!(vem==vsm&&ves<vss))
    //               {
    //                   QString startTime="00:"+QString::number(vsm)+":"+QString::number(vss);;
    //                   QString len;
    //                   if(ves>=vss)
    //                   len = "00:"+QString::number(vem-vsm)+":"+QString::number(ves-vss);
    //                   else
    //                   len="00:"+QString::number(vem-vsm-1)+":"+QString::number(60+ves-vss);

                       QWidget * datawidget=new QWidget;
                       datawidget->setFixedSize(560,100);
                       QTextEdit * sttm=new QTextEdit(datawidget);sttm->setFixedSize(70,50);sttm->move(0,20);
                       QLabel * sttmname=new QLabel(datawidget);sttmname->setText("开始时间");sttmname->setFixedSize(70,20);sttmname->setAlignment(Qt::AlignCenter);
                       QTextEdit * edtm=new QTextEdit(datawidget);edtm->setFixedSize(70,50);edtm->move(70,20);
                       QLabel * edtmname=new QLabel(datawidget);edtmname->setText("结束时间");edtmname->setFixedSize(70,20);edtmname->setAlignment(Qt::AlignCenter);edtmname->move(70,0);
                       QTextEdit * stpsx=new QTextEdit(datawidget);stpsx->setFixedSize(70,50);stpsx->move(140,20);
                       QLabel * stpsxname=new QLabel(datawidget);stpsxname->setText("起始x坐标");stpsxname->setFixedSize(70,20);stpsxname->setAlignment(Qt::AlignCenter);stpsxname->move(140,0);
                       QTextEdit * stpsy=new QTextEdit(datawidget);stpsy->setFixedSize(70,50);stpsy->move(210,20);
                       QLabel * stpsyname=new QLabel(datawidget);stpsyname->setText("起始y坐标");stpsyname->setFixedSize(70,20);stpsyname->setAlignment(Qt::AlignCenter);stpsyname->move(210,0);
                       QTextEdit * vx=new QTextEdit(datawidget);vx->setFixedSize(70,50);vx->move(280,20);
                       QLabel * vxname=new QLabel(datawidget);vxname->setText("x轴速度");vxname->setFixedSize(70,20);vxname->setAlignment(Qt::AlignCenter);vxname->move(280,0);
                       QTextEdit * vy=new QTextEdit(datawidget);vy->setFixedSize(70,50);vy->move(350,20);
                       QLabel * vyname=new QLabel(datawidget);vyname->setText("y轴速度");vyname->setFixedSize(70,20);vyname->setAlignment(Qt::AlignCenter);vyname->move(350,0);
                       QTextEdit * piclength=new QTextEdit(datawidget);piclength->setFixedSize(70,50);piclength->move(420,20);
                       QLabel * piclengthname=new QLabel(datawidget);piclengthname->setText("图片长度");piclengthname->setFixedSize(70,20);piclengthname->setAlignment(Qt::AlignCenter);piclengthname->move(420,0);
                       QTextEdit * picwidth=new QTextEdit(datawidget);picwidth->setFixedSize(70,50);picwidth->move(490,20);
                       QLabel * picwidthname=new QLabel(datawidget);picwidthname->setText("图片宽度");picwidthname->setFixedSize(70,20);picwidthname->setAlignment(Qt::AlignCenter);picwidthname->move(490,0);
                       QPushButton * getinbutton=new QPushButton("OK",datawidget);getinbutton->setFixedSize(40,20);
    //                   QTextEdit * vl=new QTextEdit(datawidget);vl->setFixedSize(70,50);vl->move(400,275);
    //                   QTextEdit * vw=new QTextEdit(datawidget);vw->setFixedSize(70,50);vw->move(450,275);
                       getinbutton->move(260,80);datawidget->show();
                       picturename.replace("\\","/");
                       picturename.replace(":","\\:");
    //                 arguments << " -i " + videofilename + " -vf movie='"+picturename+"'[wm];[in][wm]overlay="+QString::number(tx)+":"+QString::number(ty)+":enable='between(t,"+QString::number(t1)+","+QString::number(t2)+")' "+outputPath+" -y";
                       QProcess *clipProcess = new QProcess(this);
                       connect(getinbutton,&QPushButton::clicked,this,[=](){
                           qDebug()<<1;
                           int tx=stpsx->toPlainText().toInt();int ty=stpsy->toPlainText().toInt();
                           int t1=sttm->toPlainText().toInt();int t2=edtm->toPlainText().toInt();
                           int vox=vx->toPlainText().toInt();int voy=vy->toPlainText().toInt();
                           int len=piclength->toPlainText().toInt();int wid=picwidth->toPlainText().toInt();
    //                       int vol=vl->toPlainText().toInt();int vow=vw->toPlainText().toInt();
                           qDebug()<<t1<<t2;
                           QStringList arguments;
                           arguments<<"-i"<<videofilename<<"-vf"<<"[in0]scale=720:576[v];movie='"+picturename+"'[g0];[g0]scale="+QString::number(len)+":"+QString::number(wid)+"[ge0];[v][ge0]overlay=x='if(between(t,"+QString::number(t1)+","+QString::number(t2)+"),"+QString::number(tx)+"+t*"+QString::number(vox)+",NAN)':"+"y='if(between(t,"+QString::number(t1)+","+QString::number(t2)+"),"+QString::number(ty)+"+t*"+QString::number(voy)+",NAN)'[out]"<<outputPath<<"-y";
                           qDebug()<<arguments;
                           clipProcess->start(program, arguments);
                           delete datawidget;
                           QTimer * timer=new QTimer(this);
                           timer->start(3000);
                           connect(timer,&QTimer::timeout,[=](){
                                   int rowcount=videolist->rowCount();videolist->insertRow(rowcount);
                                   videolist->setItem(rowcount,0,new QTableWidgetItem(outputPath,1));
                                   videofilename=outputPath;
                                   mainplayer->setSource(QUrl::fromLocalFile(videofilename));
                                   videonames<<videofilename;
                                   pausebutton->setIcon(QIcon("://brouse.png"));
                                   timer->stop();
                           });
                       });

    //               }
    //               else
    //               QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("输入时间有误"));
    //           }
    //           else
    //           QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("输入时间有误"));
    //           delete inputtime;
       }

       }

   });
   connect(this->MenuBar->AddWords,&QAction::triggered,[=](){
           QString program = QCoreApplication::applicationDirPath()+"/bin/ffmpeg.exe";
           QString filenamesimple=videofilename.section("/",-1,-1);
           QString videofilenamesimple=filenamesimple.section(".",-2,-2);
           QWidget * datawidget=new QWidget;
           datawidget->setFixedSize(400,100);
           QTextEdit * sttm=new QTextEdit(datawidget);sttm->setFixedSize(50,40);sttm->move(0,40);
           QLabel * sttmname=new QLabel(datawidget);sttmname->setText("输入文字");sttmname->setFixedSize(50,40);sttmname->setAlignment(Qt::AlignCenter);
           QTextEdit * fonts=new QTextEdit(datawidget);fonts->setFixedSize(50,40);fonts->move(50,40);
           QLabel * fontsname=new QLabel(datawidget);fontsname->setText("文字大小");fontsname->setFixedSize(50,40);fontsname->setAlignment(Qt::AlignCenter);fontsname->move(50,0);
           QTextEdit * wordpx=new QTextEdit(datawidget);wordpx->setFixedSize(50,40);wordpx->move(100,40);
           QLabel * wordpxname=new QLabel(datawidget);wordpxname->setText("初始x");wordpxname->setFixedSize(50,40);wordpxname->setAlignment(Qt::AlignCenter);wordpxname->move(100,0);
           QTextEdit * wordpy=new QTextEdit(datawidget);wordpy->setFixedSize(50,40);wordpy->move(150,40);
           QLabel * wordpyname=new QLabel(datawidget);wordpyname->setText("初始y");wordpyname->setFixedSize(50,40);wordpyname->setAlignment(Qt::AlignCenter);wordpyname->move(150,0);
           QTextEdit * wbox=new QTextEdit(datawidget);wbox->setFixedSize(50,40);wbox->move(200,40);
           QLabel * wboxname=new QLabel(datawidget);wboxname->setText("边框");wboxname->setFixedSize(50,40);wboxname->setAlignment(Qt::AlignCenter);wboxname->move(200,0);
           QTextEdit * wcolor=new QTextEdit(datawidget);wcolor->setFixedSize(50,40);wcolor->move(250,40);
           QLabel * wcolorname=new QLabel(datawidget);wcolorname->setText("文字颜色");wcolorname->setFixedSize(50,40);wcolorname->setAlignment(Qt::AlignCenter);wcolorname->move(250,0);
           QTextEdit * wbcolor=new QTextEdit(datawidget);wbcolor->setFixedSize(50,40);wbcolor->move(300,40);
           QLabel * wbcolorname=new QLabel(datawidget);wbcolorname->setText("边框颜色");wbcolorname->setFixedSize(50,40);wbcolorname->setAlignment(Qt::AlignCenter);wbcolorname->move(300,0);
           QPushButton * getinbutton=new QPushButton("OK",datawidget);getinbutton->setFixedSize(40,20);
           getinbutton->move(180,80);datawidget->show();
           QProcess *clipProcess = new QProcess(this);
           connect(getinbutton,&QPushButton::clicked,this,[=](){
                QString t1=sttm->toPlainText();
                QString t2=fonts->toPlainText();
                QString t3=wordpx->toPlainText();
                QString t4=wordpy->toPlainText();
                QString t5="1";
                QString c1=wcolor->toPlainText();
                QString c2=wbcolor->toPlainText();
                QString outputPath = QFileInfo(videofilename).absolutePath() +"/"+videofilenamesimple+"&Words_"+t1+".mp4";
                QFile destFile(outputPath);
                if(destFile.exists()){
                    destFile.remove();
                }
                QStringList arguments;
                arguments<<"-i"<<videofilename<<"-vf"<<"drawtext=fontsize="+t2+":fontfile=lazy.ttf:text='"+t1+"':x="+t3+":y="+t4+":fontcolor="+c1+":box="+t5+":boxcolor="+c2<<outputPath;
                clipProcess->start(program, arguments);
                delete datawidget;
                QTimer * timer=new QTimer(this);
                timer->start(3000);
                connect(timer,&QTimer::timeout,[=](){
                        int rowcount=videolist->rowCount();videolist->insertRow(rowcount);
                        videolist->setItem(rowcount,0,new QTableWidgetItem(outputPath,1));
                        videofilename=outputPath;
                        mainplayer->setSource(QUrl::fromLocalFile(videofilename));
                        videonames<<videofilename;
                        pausebutton->setIcon(QIcon("://brouse.png"));
                        timer->stop();
                });
            });

   });
   connect(this->MenuBar->AddSpecialEffect,&QAction::triggered,[=](){
       QString program = QCoreApplication::applicationDirPath()+"/bin/ffmpeg.exe";
       QString filenamesimple=videofilename.section("/",-1,-1);
       QString videofilenamesimple=filenamesimple.section(".",-2,-2);
       QProcess *clipProcess = new QProcess(this);
       QString outputPath = QFileInfo(videofilename).absolutePath() +"/"+videofilenamesimple+"&FadeEffection.mp4";
       QFile destFile(outputPath);
       if(destFile.exists()){
           destFile.remove();
       }
       QStringList arguments;
       arguments<<"-i"<<videofilename<<"-vf"<<"fade=in:0:90"<<outputPath;
       clipProcess->start(program, arguments);
       QTimer * timer=new QTimer(this);
       timer->start(3000);
       connect(timer,&QTimer::timeout,[=](){
               int rowcount=videolist->rowCount();videolist->insertRow(rowcount);
               videolist->setItem(rowcount,0,new QTableWidgetItem(outputPath,1));
               videofilename=outputPath;
               mainplayer->setSource(QUrl::fromLocalFile(videofilename));
               videonames<<videofilename;
               pausebutton->setIcon(QIcon("://brouse.png"));
               timer->stop();
       });
   });

}

MainWindow::~MainWindow()
{
    delete ui;
}

