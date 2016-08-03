/**
 *author:hanjinpeng
 *545751287@qq.com
 */
#include "snakegame.h"
#include "ui_snakegame.h"
#include <QPalette>
#include <QColor>
#include <QTime>
#include <QKeyEvent>
#include <QMessageBox>
#include<QPainter>
#include<QImage>
#include<QLCDNumber>
#include<QSound>
#include<QTextCodec>
#include<QObject>

//QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
Snakegame::Snakegame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Snakegame)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    //coder=QTextCodec::codecForName("GBK");
    //QTextCodec::setCodecForTr(coder);
    ui->setupUi(this);
    //QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    rangout=false;
    speed=30;
    maxlen=100;
    mzone=new QDialog(this);
    passtime=new QTimer();
    passtime->setInterval (1000);
    foodColorChange=new QTimer();
    foodColorChange->setInterval (20);
    lcdtime=new QLCDNumber(this);
   // palette.setColor(QPalette::WindowText,QColor(0,255,0));

   // lcdtime->setAutoFillBackground(true);
    //lcdtime->setPalette(palette);
    lcdtime->display (0);
    labtime=new QLabel(QObject::tr("using time:"),this);
    //labsec=new QLabel(tr("second"),this);
    labfood=new QLabel(tr("eat foods:"),this);
    labscore=new QLabel(tr("score:"),this);
    lcdscore=new QLCDNumber(this);
    lcdfoodcount=new QLCDNumber(this);
    eatsnd=new QSound("eatsnd.wav",this);
    endsnd=new QSound("endsnd.wav",this);
   // lcdfoodcount->resize (30,30);
    labtime->move (630,80);
    lcdtime->move (630,120);
    //labsec->move (735,120);
    labfood->move (630,180);
    lcdfoodcount->move (630,220);
    //labfoodcount->move (735,220);
    labscore->move (630,280);
    lcdscore->move (630,310);
    //labscorenum->move (735,310);
    ui->gpause->setEnabled (false);
    ui->gcontinue->setEnabled (false);
    qsrand(QTime::currentTime().msec());
    this->resize(770,620);
    mzone->resize (600,600);
    mzone->move (speed,2*speed);
    switch(qrand()%4)
    {
        case 0:
            dire=D_UP;
            break;
        case 1:
            dire=D_DOWN;
            break;
        case 2:
            dire=D_LEFT;
            break;
        case 3:
            dire=D_RIGHT;
            break;
    }

    level[0]=500;
    level[1]=300;
    level[2]=100;
    level[3]=50;
    level[4]=10;
    level_stat=level[0];
    sum_time=0;
    tmp_time=0;
    pauseflag=false;
    scorenum=0;
    foodnum=0;
    pauseonesec=0;
    keyboardflag=true;
    connect(foodColorChange,SIGNAL(timeout()),this,SLOT(processfoodcolorchange()));
    connect(passtime,SIGNAL(timeout()),this,SLOT(processTimeout()));

   // datas.push_back(getFood());
   // getFood();
   // tid=this->startTimer(300);
}

Snakegame::~Snakegame()
{
    delete ui;
}

void Snakegame::snakeMove()
{

    //move ,change head direction
    int x=datas[0]->x();
    int y=datas[0]->y();
    if(x==food->x()&&y==food->y())
    {
        datas.push_back(food);
        eatsnd->play ();
        food->setPalette(QPalette(QColor(0,255,0)));
        if((datas.size ()-1)>=maxlen)
        {
            passtime->stop ();
            foodColorChange->stop ();
            this->killTimer (tid);
            QMessageBox::warning(this,"Congratulation to you! ",tr("congratulate !you are success!!!"));
            //datas.clear()
            //this->close();
            stopid=true;
            for(int i=0;i<datas.size ();i++)
            {
                datas[i]->hide ();
            }
            datas.clear ();
            food->hide ();
            keyboardflag=true;
        }else
            food=getFood();
    }

    switch(dire)
    {
        case D_UP:
             y-=speed;
             break;
        case D_DOWN:
            y+=speed;
            break;
        case D_LEFT:
            x-=speed;
            break;
        case D_RIGHT:
            x+=speed;
            break;
    }

    //edge justion
    if(x<speed||x>=(mzone->width())||y<2*speed||y>=(mzone->height()))
    {
        passtime->stop ();
        foodColorChange->stop ();
        endsnd->play ();
        pauseonesec=QTime::currentTime ().second ();
        while(pauseonesec==QTime::currentTime ().second ());
        QMessageBox::warning(this,"Game Faliure!",tr("fail to pass!!!"));
        //datas.clear()
        this->killTimer (tid);
        //this->close();
        stopid=true;
        for(int i=0;i<datas.size ();i++)
        {
            datas[i]->hide ();
        }
        datas.clear ();
        food->hide ();
        keyboardflag=true;
    }
    if(!stopid)
    {
    //follow privious label
        for(int i=datas.size()-1;i>0;i--)
        {
            datas[i]->move(datas[i-1]->x(),datas[i-1]->y());
        }

        datas[0]->move(x,y);

        //head and tail  cross over 
        for(int t=datas.size()-1;t>1;t--)
        {   //direction 
            if((datas[0]->x())==(datas[t]->x())&&(datas[0]->y())==(datas[t]->y())&&((datas[t-2]->x())!=(datas[t]->x())&&(datas[t-2]->y())!=(datas[t]->y())))
            {
                 passtime->stop ();
                 foodColorChange->stop ();
                 endsnd->play ();
                 pauseonesec=QTime::currentTime ().second ();
                 while(pauseonesec==QTime::currentTime ().second ());
                QMessageBox::warning(this,"Game Over!",tr("continue to play,wish you success!!"));
                //datas.clear()
                //this->close();
                rangout=true;
                break;
            }
        }
        if(rangout)
        {
            this->killTimer (tid);
            stopid=true;
            for(int i=0;i<datas.size ();i++)
            {
                datas[i]->hide ();
            }
            datas.clear ();
            food->hide ();
            keyboardflag=true;
        }
    }
}
QLabel* Snakegame::getFood()
{
    food=new QLabel(this);
    food->resize(speed,speed);
    food->setAutoFillBackground(true);
    food->setPalette(QPalette(QColor(color_r=qrand()%255,color_g=qrand()%255,color_b=qrand()%255)));
    //food->setPalette(QPalette(QColor(255,0,0)));
    //food->move(400,300);
    food->move(((qrand()%(mzone->width()-speed)+speed)/speed)*speed,(((qrand()%(mzone->height()-2*speed)+2*speed)/speed)*speed));
    food->show();
    return food;
}
void Snakegame::timerEvent(QTimerEvent *e)
{
    snakeMove();
}
void Snakegame::keyPressEvent(QKeyEvent *e)
{
    int i=datas.size ();
    switch(e->key())
    {
    case Qt::Key_Up:
        if(i==1)
            dire=D_UP;
        else
        {
            if(dire==D_DOWN)
                break;
            else
             dire=D_UP;
        }
        break;
    case Qt::Key_Down:
        if(i==1)
            dire=D_DOWN;
        else
        {
            if(dire==D_UP)
                break;
            else
             dire=D_DOWN;
        }
        break;
    case Qt::Key_Left:
        if(i==1)
            dire=D_LEFT;
        else
        {
            if(dire==D_RIGHT)
                break;
            else
             dire=D_LEFT;
        }
        break;
    case Qt::Key_Right:
        if(i==1)
            dire=D_RIGHT;
        else
        {
            if(dire==D_LEFT)
                break;
            else
             dire=D_RIGHT;
        }
        break;
    }
    if(keyboardflag)
    {
        if(datas.size ()>0)
        {
            tid=this->startTimer(level_stat);
            passtime->start ();
            keyboardflag=false;
        }
    }
}
void Snakegame::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    int heig=mzone->height();
    int wid=mzone->width();
    painter.setPen(Qt::black);
    painter.setBrush(Qt::blue);
    painter.drawRect(speed-5,2*speed-5,wid-speed+10,heig-2*speed+10);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(speed,2*speed,wid-speed,heig-2*speed);

    painter.setPen(Qt::lightGray);
    //painter.setPen(Qt::red);
    for(int tx=2*speed;tx<=heig;tx+=speed)
    {
        painter.drawLine(speed,tx,wid,tx);
    }
    for(int ty=speed;ty<=wid;ty+=speed)
    {
        painter.drawLine(ty,2*speed,ty,heig);
    }
    //painter.drawLine(5,25,500,500);

}


void Snakegame::on_gstart_triggered()
{

    if(datas.size ())
    {
        this->killTimer (tid);
        for(int i=0;i<datas.size ();i++)
        {
            datas[i]->hide ();
        }
        datas.clear ();
        food->hide ();
    }
    rangout=false;
    lcdtime->display (0);
    //passtime->start ();
    //s_time=QTime::currentTime().minute ()*60+QTime::currentTime ().second ();
    datas.push_back(getFood());
    getFood();
    foodColorChange->start ();

    //tid=this->startTimer(level_stat);
    stopid=false;
    ui->gcontinue->setEnabled (false);
    ui->gpause->setEnabled (true);
    pauseflag=false;
    sum_time=0;
    scorenum=0;
    foodnum=0;
    keyboardflag=true;
}

void Snakegame::on_gpause_triggered()
{
    if(datas.size ())
    {
        this->killTimer (tid);
        ui->gpause->setEnabled (false);
        ui->gstart->setEnabled (false);
        ui->gcontinue->setEnabled (true);
        foodColorChange->stop ();
        pauseflag=true;
        //keyboardflag=false;
    }

}

void Snakegame::on_gameover_triggered()
{
    if(tid>0)
        this->killTimer (tid);
    passtime->stop ();
    foodColorChange->stop ();
    pauseflag=true;
    switch(QMessageBox::question(this,"Game Over!",tr("do you want to end the game?"),QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
    {
        case QMessageBox::Ok:
            this->close();
            break;
        case QMessageBox::Cancel:
            tid=this->startTimer (level_stat);
            passtime->start();
            foodColorChange->start();
            pauseflag=false;
            break;
        default:
            break;
    }
}

void Snakegame::on_grestart_triggered()
{
    if(datas.size ())
    {
        this->killTimer (tid);
        passtime->stop ();
        ui->gstart->setEnabled (true);
        for(int i=0;i<datas.size ();i++)
        {
            //datas[i]->destroy ();
            datas[i]->hide ();
        }
        datas.clear ();
        food->hide ();
        //food->destroy ();
    }
    lcdtime->display (0);
    //passtime->start ();
    //s_time=QTime::currentTime().minute ()*60+QTime::currentTime ().second ();
    //connect(passtime,SIGNAL(timeout()),this,SLOT(processTimeout()));
    rangout=false;
    datas.push_back(getFood());
    getFood();
    foodColorChange->start ();
    stopid=false;
    //tid=this->startTimer(level_stat);
    ui->gcontinue->setEnabled (false);
    ui->gpause->setEnabled (true);
    pauseflag=false;
    sum_time=0;
    scorenum=0;
    foodnum=0;
    keyboardflag=true;

}

void Snakegame::on_gcontinue_triggered()
{
    if(datas.size ())
    {
        tid=this->startTimer(level_stat);
        ui->gpause->setEnabled (true);
        ui->gstart->setEnabled (true);
        ui->gcontinue->setEnabled (false);
        foodColorChange->start ();
        pauseflag=false;
        //s_time=QTime::currentTime().minute ()*60+QTime::currentTime ().second ();

    }
}

void Snakegame::on_level_basic_triggered()
{
    level_stat=level[0];
}

void Snakegame::on_level_middle_triggered()
{
    level_stat=level[1];
}

void Snakegame::on_level_senior_triggered()
{
    level_stat=level[2];
}

void Snakegame::on_level_major_triggered()
{
    level_stat=level[3];
}

void Snakegame::on_level_perfect_triggered()
{
    level_stat=level[4];
}

void Snakegame::processTimeout ()
{
    //end_time=QTime::currentTime().minute ()*60+QTime::currentTime ().second ();
    //tmp_time=end_time-s_time;
    if(!pauseflag)
        //sum_time=tmp_time;
        sum_time+=1;
    lcdtime->display (sum_time);
   // sprintf (foodcounts,"%d",datas.size ()-1);
}
void Snakegame::processfoodcolorchange ()
{
    if(qrand ()%2==0)
        //food->setPalette(QPalette(QColor(color_r,color_g,color_b)));
        food->setPalette(QPalette(QColor(0,0,255)));
    else
        food->setPalette(QPalette(QColor(255,0,0)));
    lcdfoodcount->display ((datas.size ()-1)<0?0:datas.size ()-1);
    foodnum=datas.size ()-1;
    if(foodnum<=10)
        scorenum=(foodnum<0?0:foodnum)*10;
    else if(foodnum<=20)
        scorenum=10*10+(foodnum-10)*20;
    else if(scorenum<=30)
        scorenum=10*(10+20)+(foodnum-20)*30;
    else if(foodnum<=40)
        scorenum=10*(10+20+30)+(foodnum-30)*40;
    else if(foodnum<=50)
        scorenum=10*(10+20+30)+(foodnum-40)*50;
    else if(foodnum<=60)
        scorenum=10*(10+20+30+40+50)+(foodnum-50)*60;
    else if(foodnum<=70)
        scorenum=10*(10+20+30+40+50+60)+(foodnum-60)*70;
    else if(foodnum<=80)
        scorenum=10*(10+20+30+40+50+60+70)+(foodnum-70)*80;
    else if(foodnum<=90)
        scorenum=10*(10+20+30+40+50+60+70+80)+(foodnum-80)*90;
    else
        scorenum=10*(10+20+30+40+50+60+70+80+90)+(foodnum-90)*100;
    lcdscore->display (scorenum);
}

void Snakegame::on_instruction_triggered()
{
    QMessageBox::information(this,"instruction direct document",tr("control by down up lef right key to move！"));
}
void Snakegame::on_aboutme_triggered()
{
    QMessageBox::about(this,"About Me ",tr("snake alpha v0.1，write by hanjinpeng，welcome your use..."));
    //QMessageBox::aboutQt(this,tr("本软件是贪吃蛇 alpha 0.1 版，由韩锦鹏倾力开发，欢迎你使用愉快，感谢你的支持，谢谢！"));
}
