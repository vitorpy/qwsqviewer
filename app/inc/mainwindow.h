#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "an2kloader.h"

class An2kLoader;
class Canvas;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void about();
    void exit();
    void open();
    void zoomPlus();
    void zoomMinus();
    void zoomOriginal();
    void zoomFit();
    void openAn2k();
    void detectMinutiae();

private:
    Ui::MainWindow *_ui;
    Canvas* _canvas;
    An2kLoader* _loader;
};

#endif // MAINWINDOW_H
