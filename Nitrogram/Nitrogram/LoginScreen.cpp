#include "LoginScreen.h"

LoginScreen::LoginScreen(QWidget *parent) :
    QMainWindow(parent, Qt::SplashScreen),
    ui(new LoginUI(this))
{
    ui->CreateUI();
}
LoginScreen::~LoginScreen() {
    delete ui;
}

#include <QApplication>
#include <QFile>
#include <QFont>

void LoginScreen::applyStyle() {
    QFile f("../../Nitrogram/Nitrogram.css");
    if(!f.open(QIODevice::ReadOnly))
        return;

    qobject_cast<QApplication *>(QApplication::instance())->setStyleSheet(f.readAll());

    f.close();
}
