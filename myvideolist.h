#ifndef MYVIDEOLIST_H
#define MYVIDEOLIST_H

#include <QTableWidget>
#include <QObject>
#include "QHeaderView"
#include "QStringList"

class MyVideoList : public QTableWidget
{
    Q_OBJECT
public:
    MyVideoList(QWidget *parent = nullptr);
    QHeaderView * hheader;
    QHeaderView * vheader;
    QStringList header;
};

#endif // MYVIDEOLIST_H
