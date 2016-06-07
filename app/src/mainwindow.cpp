#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <cstdio>
#include <QImage>
#include <QScrollArea>

#include "canvas.h"
#include "utils.h"
#include "aboutwindow.h"
#include "inc/mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"
#include "ihead.h"
#include "wsq.h"
#include "img_io.h"
#include "dpyan2k.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    QScrollArea *sa = new QScrollArea(this);
    _canvas = new Canvas(sa);

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
    QString file = QFileDialog::getOpenFileName(this, tr("qwsqviewer"), QString(), tr("AN2K images (*.an2k)"));

    int ret;

    ANSI_NIST *ansi_nist;
    if ((ret = read_ANSI_NIST_file(file.toLocal8Bit().data(), &ansi_nist))){
       QMessageBox::warning(this, tr("qwsqviewer"), tr("ERROR : dpyan2k : reading ANSI_NIST file ") + file);
       return;
    }

    /* Initialize image displayed counter. */
    int num_images = 0;
    for(int record_i = 1; record_i < ansi_nist->num_records; record_i++){
       /* Set current record. */
       RECORD *record = ansi_nist->records[record_i];

       if (image_record(record->type)){
           ;
       }
    }

    free_ANSI_NIST(ansi_nist);
}
