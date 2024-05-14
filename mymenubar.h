#ifndef MYMENUBAR_H
#define MYMENUBAR_H

#include <QMenuBar>

class MyMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    MyMenuBar(QWidget *parent = nullptr);
    QMenu * FileMenu;
    QMenu * ToolMenu;
    QAction * InputPic;
    QAction * InputFiles;
    QAction * DivideAction;
    QAction * BuildTimeLine;
    QAction * ComBineVideo;
    QAction * AddCaptions;
    QAction * AddPicture;
    QAction * AddWords;
    QAction * AddSpecialEffect;
};

#endif // MYMENUBAR_H
