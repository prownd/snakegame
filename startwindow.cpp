#include "startwindow.h"
#include "ui_startwindow.h"
#include<QPainter>
#include<QImage>
#include<QTime>
//#include <Phonon>
//#include"snakegame.h"
startwindow::startwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::startwindow)
{


    //painter.drawImage (0,20,img,0,0,800,600);
    ui->setupUi(this);
    this->resize (800,540);
    snd=new QSound("dream.wav",this);
    snd->play ();
   // int t=QTime::currentTime ().minute ();
   // while(t==QTime::currentTime ().minute ());
    connect (ui->btnstart,SIGNAL(clicked()),this,SLOT(startprogram()));
    connect (ui->btnend,SIGNAL(clicked()),this,SLOT(close()));

}

startwindow::~startwindow()
{
    delete ui;
}
void startwindow::paintEvent (QPaintEvent *e)
{
    QPainter painter(this);
    QImage img;
    img.load ("background1.jpg");
    painter.drawImage (0,0,img);
}
void startwindow::startprogram ()
{
    snd->stop ();
    //QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    snake.show ();
    this->close();
}
