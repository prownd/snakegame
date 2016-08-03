/**
 *author:hanjinpeng
 *545751287@qq.com
 */
#ifndef STARTWINDOW_H
#define STARTWINDOW_H
#include <QMainWindow>
#include<QPainter>
#include"snakegame.h"
#include<QSound>

namespace Ui {
class startwindow;
}

class startwindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit startwindow(QWidget *parent = 0);
    ~startwindow();
    void paintEvent (QPaintEvent * e);
    
private:
    Ui::startwindow *ui;
    Snakegame snake;
    QSound * snd;
public slots:
    void startprogram();

};

#endif // STARTWINDOW_H
