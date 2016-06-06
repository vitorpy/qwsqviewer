#include <cstdio>

#include "mainwindow.h"
#include <QApplication>

int debug;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow mw;
    mw.show();

    return app.exec();
}
