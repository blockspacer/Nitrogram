#ifndef LOGINUI_H
#define LOGINUI_H

#include <QLineEdit>

class LoginScreen;

class LoginUI {
public:
    LoginUI(LoginScreen*);
    ~LoginUI();

    void CreateUI();

private:
    struct {
        QLineEdit *email;
        QLineEdit *pass;
    } auth;

    LoginScreen *ls;
};

#endif // LOGINUI_H
