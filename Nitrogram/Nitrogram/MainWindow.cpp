#include "MainWindow.h"

#include <MainUI.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new MainUI(this))
{
    ui->CreateUI();
}
MainWindow::~MainWindow() {
    delete ui;
}

#include <QApplication>
#include <QFile>
#include <QFont>

void MainWindow::applyStyle() {
    QFile f("../../Nitrogram/Nitrogram.css");
    if(!f.open(QIODevice::ReadOnly))
        return;

    qobject_cast<QApplication *>(QApplication::instance())->setStyleSheet(f.readAll());

    f.close();
}
