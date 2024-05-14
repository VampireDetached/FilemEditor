#include "mymenubar.h"
#include <QMenuBar>

MyMenuBar::MyMenuBar(QWidget *parent)
{
    this->setParent(parent);
    FileMenu=new QMenu("文件",this);
    ToolMenu=new QMenu("工具",this);
    this->addMenu(FileMenu);this->addMenu(ToolMenu);

    InputPic=new QAction("导入图片文件",this);
    InputFiles=new QAction("导入视频文件",this);
    FileMenu->addAction(InputFiles);FileMenu->addAction(InputPic);

    DivideAction=new QAction("裁剪",this);
    BuildTimeLine=new QAction("构建时间轴",this);
    ComBineVideo=new QAction("合并视频",this);
    AddCaptions=new QAction("添加字幕",this);
    AddPicture=new QAction("贴图",this);
    AddWords=new QAction("贴文字",this);
    AddSpecialEffect=new QAction("淡入特效",this);
    ToolMenu->addAction(DivideAction);ToolMenu->addAction(BuildTimeLine);
    ToolMenu->addAction(ComBineVideo);ToolMenu->addAction(AddCaptions);
    ToolMenu->addAction(AddPicture);ToolMenu->addAction(AddWords);
    ToolMenu->addAction(AddSpecialEffect);
}
