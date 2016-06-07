#include <cstdio>

#include "mainwindow.h"
#include <QApplication>

int debug = 0;
int verbose = 0;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow mw;
    mw.show();

    return app.exec();
}
