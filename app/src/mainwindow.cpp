#include "inc/mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QString>
#include "canvas.h"
#include "util.h"
#include "aboutwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _canvas = new Canvas(this);

    setCentralWidget(_canvas);

    CHECK(connect(_ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(open())));
    CHECK(connect(_ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(about)));
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

    if (!file.isEmpty())
        QMessageBox::warning(this, tr("qwsqviewwer"), file);
}
