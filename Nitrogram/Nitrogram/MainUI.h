#ifndef MAINUI_H
#define MAINUI_H

class MainWindow;

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QLabel>

#include <QSignalMapper>

class MainUI {
public:
    MainUI(MainWindow *mainWindow);
    ~MainUI();

    void CreateUI();

private:
    void CreateCentralWidget();
    void CreateToolBar();
    void CreateStatusBar();

    QWidget* GetAccountWidget();
    QWidget* GetBotWidget();
    QWidget* GetUploaderWidget();
    QWidget* GetToolsWidget();
    QWidget* GetSettingsWidget();
    QWidget* GetHelpWidget();

    QWidget* GetLikerTab();
    QWidget* GetFollowerTab();
    QWidget* GetUnfollowerTab();
    QWidget* GetCommenterTab();
    QWidget* GetMixedTab();
    QWidget* GetMassActionsTab();

    QWidget* GetAccountManagerView();
    QWidget* GetLicenseInfoView();

    QSignalMapper *mainToolBarMapper;

    struct {
        QComboBox *method;
        QTextEdit *hashtags;
        QLineEdit *picsPerHashtag;
        QTextEdit *usernames;
        QLineEdit *picsPerUsername;
        QLineEdit *likesToSend;
        QCheckBox *dontLimitLikes;
        QLineEdit *minDelay;
        QLineEdit *maxDelay;
        QCheckBox *timedSleepEnable;
        QLineEdit *timedSleepPeriod;
        QLineEdit *minTimedSleep;
        QLineEdit *maxTimedSleep;
        QCheckBox *likesSleepEnable;
        QLineEdit *likesSleepAmount;
        QLineEdit *minLikesSleep;
        QLineEdit *maxLikesSleep;
    } likerOptions;

    struct {
        QLabel *subscrStatus;
        QLabel *subscrType;
        QLabel *subscrStarted;
        QLabel *subscrEnded;
    } licenseInfo;

    struct {
        QLabel *avatar;
        QLineEdit *login;
        QLineEdit *pass;

        struct {
            QLineEdit *type;
            QLineEdit *ip;
            QLineEdit *user;
            QLineEdit *pass;
        } proxy;
    } addNewAccount;

    struct {
        QLabel *avatar;
        QLabel *username;
        QLabel *followers;
        QLabel *following;
        QLabel *newFollowers;
    } userDetailes;

    MainWindow *mw;
};

#endif // MAINUI_H
