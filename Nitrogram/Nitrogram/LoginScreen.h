#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QMainWindow>

#include "LoginUI.h"

class LoginScreen : public QMainWindow {
    Q_OBJECT

public:
    LoginScreen(QWidget *parent = 0);
    ~LoginScreen();

public slots:
    void applyStyle();

private:
    LoginUI *ui;
};

#endif // LOGINSCREEN_H
