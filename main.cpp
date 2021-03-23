#include "maininterface.h"

#include <QApplication>
//测试post请求
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainInterface w;
    w.show();
    return a.exec();
}
