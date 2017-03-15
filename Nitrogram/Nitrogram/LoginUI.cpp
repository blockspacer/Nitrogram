#include "LoginUI.h"
#include "LoginScreen.h"

#include <QApplication>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "UIHelper.hpp"

LoginUI::LoginUI(LoginScreen *loginScreen) :
    ls(loginScreen)
{
    ls->setObjectName("loginScreen");
}
LoginUI::~LoginUI() {
}
void LoginUI::CreateUI() {
    OBJ_NAME(ls, "loginScreenUI");

    QHBoxLayout *lay;
    #define ADD_CENTRALIZED_WIDGETS(widget1, widget2)   \
        lay = new QHBoxLayout;                          \
        lay->addStretch(1);                             \
        lay->addWidget(widget1);                        \
        if(0 != widget2) lay->addWidget(widget2);       \
        lay->addStretch(1);


    QWidget *centralWidget = OBJ_NAME(new QWidget, "loginCentralWidget");
    ls->setCentralWidget(centralWidget);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);

    auth.email = OBJ_NAME(new QLineEdit, "emailInput");
    auth.pass = OBJ_NAME(new QLineEdit, "passInput");

    centralLayout->addStretch(1);
    ADD_CENTRALIZED_WIDGETS(OBJ_NAME(new QLabel, "logo"), 0)
    centralLayout->addLayout(lay);
    centralLayout->addStretch(1);
    ADD_CENTRALIZED_WIDGETS(OBJ_NAME(LBL("Welcome."), "welcomeMessage"), 0)
    centralLayout->addLayout(lay);
    ADD_CENTRALIZED_WIDGETS(OBJ_NAME(LBL("Nitrogram requires authentication."), "authMessage"), 0)
    centralLayout->addLayout(lay);
    centralLayout->addStretch(1);

    QGridLayout *authLay = new QGridLayout;
    authLay->addWidget(OBJ_NAME(LBL("EMAIL"), "loginPassLabel"),    0, 1);
    authLay->addWidget(auth.email,                                  0, 2);
    authLay->addWidget(OBJ_NAME(LBL("PASSWORD"), "loginPassLabel"), 1, 1);
    authLay->addWidget(auth.pass,                                   1, 2);
    authLay->setColumnStretch(0, 1);
    authLay->setColumnStretch(3, 1);
    authLay->setSpacing(10);
    centralLayout->addLayout(authLay);
    centralLayout->addStretch(1);


    QPushButton *loginButton = OBJ_NAME(PBT("Login"), "loginButton");
    connect(loginButton, SIGNAL(clicked()), ls, SLOT(applyStyle()));

    QPushButton *newButton = OBJ_NAME(PBT("Create a new Account"), "newButton");
    connect(newButton, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

    lay = new QHBoxLayout;
    lay->addStretch(1);
    lay->addWidget(loginButton);
    lay->addWidget(OBJ_NAME(LBL("or"), "orLabel"));
    lay->addWidget(newButton);
    lay->addStretch(1);
    centralLayout->addLayout(lay);
    centralLayout->addStretch(1);


    QString bottomNote = tr("Note: you can create a free account when you click on "
                            "'Create a new Account'. You can choose a free trial with "
                            "all the features (no credit card required).");
    ADD_CENTRALIZED_WIDGETS(OBJ_NAME(new QLabel(bottomNote), "noteLabel"), 0)
    centralLayout->addLayout(lay);
    centralLayout->addStretch(1);
}
