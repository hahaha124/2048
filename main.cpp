#include "GameWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameWindow w;
    w.setWindowTitle("2048");
    w.resize(600, 700);
    w.show();
    return a.exec();
}
