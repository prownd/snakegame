//#include <QtGui/QApplication>
#include <QApplication>
#include "snakegame.h"
#include "startwindow.h"
#include <QTextCodec>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    //QTextCodec * coder=QTextCodec::codecForName("GBK");
    //QTextCodec::setCodecForTr(coder);
    startwindow s;
    s.show();
    //Snakegame w;
    //w.show();
    //w.hide ();
    return a.exec();
}
