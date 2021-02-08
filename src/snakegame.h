/**
 *author:hanjinpeng
 *545751287@qq.com
 */
#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include <QtGlobal>
#if QT_VERSION >= 0x050000
	#include <QtWidgets/QMainWindow>
#else
	#include <QMainWindow>
#endif
#include <QLabel>
#include<QList>
#include<QImage>
#include<QDialog>
#include<QLCDNumber>
#include<QTimer>
#include<QLabel>
#include<QSound>
#include<QTextCodec>
#include<QPalette>
enum Direction{D_UP,D_DOWN ,D_RIGHT,D_LEFT};

namespace Ui {
class Snakegame;
}

class Snakegame : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Snakegame(QWidget *parent = 0);
    ~Snakegame();
private:
    QList<QLabel*> datas;
    QLabel * food;
    QDialog *mzone;
    QLCDNumber *lcdtime;
    QTimer * passtime;
    QTimer * foodColorChange;
    QLabel * labtime;
    QLabel * labfood;
    QLCDNumber * lcdfoodcount;
    QLabel * labsec;
    QLabel * labfoodcount;
    QLabel * labscorenum;
    QLabel * labscore;
    QLCDNumber * lcdscore;
    QSound * eatsnd;
    QSound * endsnd;
    QTextCodec * coder;
    QPalette palette;
  // char foodcounts[50];
    Direction dire;
    int speed;
    int maxlen;
    int tid;
    bool stopid;
    bool rangout;
    int level[5];
    int level_stat;
    int color_r;
    int color_g;
    int color_b;
    int s_time;
    int end_time;
    int sum_time;
    int tmp_time;
    bool pauseflag;
    int scorenum;
    int foodnum;
    int pauseonesec;
    bool keyboardflag;
public:

      void snakeMove();
      QLabel * getFood();
      //control move
       void timerEvent(QTimerEvent *e);

       //control direction
       void keyPressEvent(QKeyEvent *e);
       void paintEvent(QPaintEvent *e);
    
private slots:
    void on_gstart_triggered();

    void on_gpause_triggered();

    void on_gameover_triggered();

    void on_grestart_triggered();

    void on_gcontinue_triggered();

    void on_level_basic_triggered();

    void on_level_middle_triggered();

    void on_level_senior_triggered();

    void on_level_perfect_triggered();

    void on_level_major_triggered();
    void processTimeout();
    void processfoodcolorchange();

    void on_instruction_triggered();

    void on_aboutme_triggered();

private:
    Ui::Snakegame *ui;
};

#endif // SNAKEGAME_H
