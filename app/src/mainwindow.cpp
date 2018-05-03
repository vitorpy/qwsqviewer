#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <cstdio>
#include <QImage>
#include <QScrollArea>
#include <QThreadPool>

#include "canvas.h"
#include "utils.h"
#include "aboutwindow.h"
#include "inc/mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"
#include "ihead.h"
#include "wsq.h"
#include "img_io.h"
#include "an2kloader.h"
#include "mindtct.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    QScrollArea *sa = new QScrollArea(this);
    _canvas = new Canvas(sa);

    _loader = new An2kLoader(_canvas);
    _loader->setAutoDelete(false);

    sa->setWidget(_canvas);

    setCentralWidget(sa);

    CHECK(connect(_ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(open())));
    CHECK(connect(_ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(about())));
    CHECK(connect(_ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(exit())));
    CHECK(connect(_ui->actionZoomPlus, SIGNAL(triggered(bool)), this, SLOT(zoomPlus())));
    CHECK(connect(_ui->actionZoomMinus, SIGNAL(triggered(bool)), this, SLOT(zoomMinus())));
    CHECK(connect(_ui->actionZoomOriginal, SIGNAL(triggered(bool)), this, SLOT(zoomOriginal())));
    CHECK(connect(_ui->actionZoomFit, SIGNAL(triggered(bool)), this, SLOT(zoomFit())));
    CHECK(connect(_ui->actionOpenAN2K, SIGNAL(triggered(bool)), this, SLOT(openAn2k())));
    CHECK(connect(_ui->actionDetectMinutiae, SIGNAL(triggered(bool)), this, SLOT(detectMinutiae())));
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::about()
{
    AboutWindow abw(this);

    abw.exec();
}

void MainWindow::exit()
{
    close();
}

void MainWindow::open()
{
    QString file = QFileDialog::getOpenFileName(this, tr("qwsqviewer"), QString(), tr("Wsq images (*.wsq)"));

    if (file.isEmpty())
        return;

    unsigned char *idata;
    int ilen;

    int ret;
    if ((ret = read_raw_from_filesize(file.toLocal8Bit().data(), &idata, &ilen))) {
        free(idata);
        QMessageBox::warning(this, tr("qwsqviewer"), tr("failed."));
        return;
    }

    unsigned char* odata;
    int width, height, depth, ppi, lossyflag;
    if ((ret = wsq_decode_mem(&odata, &width, &height, &depth, &ppi,
                           &lossyflag, idata, ilen))) {
        free(idata);
        QMessageBox::warning(this, tr("qwsqviewer"), tr("failed 2."));
        return;
    }

    QImage img(odata, width, height, QImage::Format_Grayscale8, &free, odata);
    _canvas->setImage(img);
    _canvas->update();
    _canvas->setPpi(ppi);

    free(idata);
}

void MainWindow::zoomPlus()
{
    _canvas->zoom(1.1);
}

void MainWindow::zoomMinus()
{
    _canvas->zoom(0.9);
}

void MainWindow::zoomOriginal()
{
    _canvas->original();
}

void MainWindow::zoomFit()
{
    _canvas->fit();
}

void MainWindow::openAn2k()
{
    QString file = QFileDialog::getOpenFileName(this, tr("qwsqviewer"), QString(), tr("AN2K images (*.an2)"));

    if (file.isEmpty())
        return;

    _loader->setFile(file);

    QThreadPool *threadPool = QThreadPool::globalInstance();
    threadPool->start(_loader);
}

void MainWindow::detectMinutiae()
{
    MinutiaeDetect* md = new MinutiaeDetect(_canvas);

    QThreadPool *threadPool = QThreadPool::globalInstance();
    threadPool->start(md);
}
