#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <cstdio>
#include <QImage>

#include "canvas.h"
#include "utils.h"
#include "aboutwindow.h"
#include "inc/mainwindow.h"
#include "ui_mainwindow.h"
#include "util.h"
#include "ihead.h"
#include "wsq.h"
#include "img_io.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _canvas = new Canvas(this);

    setCentralWidget(_canvas);

    CHECK(connect(_ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(open())));
    CHECK(connect(_ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(about())));
    CHECK(connect(_ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(exit())));
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
    QString file = QFileDialog::getOpenFileName(this);

    if (file.isEmpty())
        return;

    unsigned char *idata;
    int ilen;

    int ret;
    if (ret = read_raw_from_filesize(file.toLocal8Bit().data(), &idata, &ilen)) {
        free(idata);
        QMessageBox::warning(this, tr("qwsqviewer"), tr("failed."));
        return;
    }

    unsigned char* odata;
    int width, height, depth, ppi, lossyflag;
    if (ret = wsq_decode_mem(&odata, &width, &height, &depth, &ppi,
                           &lossyflag, idata, ilen)) {
        free(idata);
        QMessageBox::warning(this, tr("qwsqviewer"), tr("failed 2."));
        return;
    }

    QImage img(odata, width, height, QImage::Format_Grayscale8, &free, odata);
    _canvas->setImage(img);
    _canvas->update();

    free(idata);
}
