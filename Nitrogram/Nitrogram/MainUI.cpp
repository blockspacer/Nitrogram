#include <MainUI.h>

#include <QGraphicsOpacityEffect>

#include <MainWindow.h>

#include <QStackedWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QTreeView>
#include <QListView>
#include <QScrollBar>
#include <QToolButton>

#include <QHBoxLayout>
#include <QGridLayout>
#include <QStackedLayout>

#include <QStandardItemModel>

#include <QEvent>

#include <QIcon>
#include <QImage>
#include <QPixmap>
#include <QPainter>

#include <QString>

#include "UIHelper.hpp"

MainUI::MainUI(MainWindow *mainWindow) :
    mw(mainWindow)
{
    mw->setObjectName("mainUI");
}
MainUI::~MainUI() {
}
void MainUI::CreateUI() {
    CreateToolBar();
    CreateCentralWidget();
    CreateStatusBar();
}
void MainUI::CreateStatusBar() {
    QStatusBar *status = new QStatusBar(mw);

    mw->setStatusBar(status);
}
void MainUI::CreateCentralWidget() {
    QWidget *w = new QWidget(mw);
    w->setObjectName("centralWidget");
    mw->setCentralWidget(w);

    w->setObjectName("centralWidget");

    QHBoxLayout *lay = new QHBoxLayout(w);

    QStackedWidget *sw = new QStackedWidget(mw);

    sw->addWidget(GetAccountWidget());
    sw->addWidget(GetBotWidget());
    sw->addWidget(GetUploaderWidget());
    sw->addWidget(GetToolsWidget());
    sw->addWidget(GetSettingsWidget());
    sw->addWidget(GetHelpWidget());

    connect(mainToolBarMapper, SIGNAL(mapped(QWidget*)), sw, SLOT(setCurrentWidget(QWidget*)));

    lay->addWidget(sw);
}
void MainUI::CreateToolBar() {
    QToolBar *mainToolBar = new QToolBar(mw);

    QLabel *logoLabel = OBJ_NAME(new QLabel(mw), "logoLabel");

    QWidget *toolBarStretch = new QWidget(mw);
    toolBarStretch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mainToolBarMapper = new QSignalMapper(mw);

    QAction *action;
    QActionGroup *actionGroup = new QActionGroup(mw);
    actionGroup->setExclusive(true);

    connect(mainToolBar->addWidget(logoLabel), SIGNAL(triggered(bool)), mw, SLOT(applyStyle()));
    mainToolBar->addSeparator();

    #define ADD_ACTION(text, widget)                                                    \
        action = mainToolBar->addAction(tr(text));                                      \
        actionGroup->addAction(action);                                                 \
        action->setCheckable(true);                                                     \
        mainToolBar->widgetForAction(action)->setProperty("tool-widget-name", text);    \
        connect(action, SIGNAL(triggered()), mainToolBarMapper, SLOT(map()));           \
        mainToolBarMapper->setMapping(action, widget);

    ADD_ACTION("Accounts", GetAccountWidget());
    action->setChecked(true);
    ADD_ACTION("Bot",      GetBotWidget());
    ADD_ACTION("Uploader", GetUploaderWidget());
    ADD_ACTION("Tools",    GetToolsWidget());
    mainToolBar->addWidget(toolBarStretch);
    ADD_ACTION("Settings", GetSettingsWidget());
    ADD_ACTION("Help",     GetHelpWidget());

    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    mainToolBar->setMovable(false);

    mw->addToolBar(Qt::TopToolBarArea, mainToolBar);
}
QWidget* MainUI::GetAccountWidget() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    w = new QWidget(mw);
    QHBoxLayout *lay = new QHBoxLayout(w);
    lay->addWidget(GetAccountManagerView(), 1);
    lay->addWidget(GetLicenseInfoView());

    return w;
}
QWidget* MainUI::GetAccountManagerView() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    QSignalMapper *listGraphNotionMapper = new QSignalMapper(mw);

    ////////////      List Notion       ////////////////
    QTreeView *accountList = OBJ_NAME(new QTreeView, "accountList");
    accountList->setEditTriggers(QTreeView::NoEditTriggers);
    accountList->setItemsExpandable(false);
    accountList->setRootIsDecorated(false);
    accountList->setAllColumnsShowFocus(true);
    accountList->setCornerWidget(OBJ_PROP(new QWidget, "scroll-corner", "true"));

    QStandardItemModel *listAccountsViewModel = new QStandardItemModel(accountList);
    listAccountsViewModel->setItem(0, 0, new QStandardItem("username1"));
    listAccountsViewModel->setItem(0, 1, new QStandardItem("12.12.123.12:123"));
    listAccountsViewModel->setItem(0, 2, new QStandardItem("12345"));
    listAccountsViewModel->setItem(0, 3, new QStandardItem("Idle"));
    listAccountsViewModel->setItem(1, 0, new QStandardItem("name2"));
    listAccountsViewModel->setItem(1, 1, new QStandardItem("param2"));
    listAccountsViewModel->setItem(2, 0, new QStandardItem("name3"));
    listAccountsViewModel->setItem(2, 1, new QStandardItem("param3"));
    for(int i = 3; i < 50; i++) {
        listAccountsViewModel->setItem(i, 1, new QStandardItem("param"));
    }
    listAccountsViewModel->setHeaderData(0, Qt::Horizontal, tr("USERNAME"));
    listAccountsViewModel->setHeaderData(1, Qt::Horizontal, tr("PROXY"));
    listAccountsViewModel->setHeaderData(2, Qt::Horizontal, tr("NEW FOLL."));
    listAccountsViewModel->setHeaderData(3, Qt::Horizontal, tr("STATUS"));

    accountList->setModel(listAccountsViewModel);

    /*
    QToolBar *accountListBar = OBJ_NAME(new QToolBar, "accountListBar");
    QWidget *accountListBarSpacing = new QWidget;
    accountListBarSpacing->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    accountListBar->widgetForAction(accountListBar->addAction(tr("Add New")))->setProperty("tool-widget-name", "Text tool buttons");
    accountListBar->widgetForAction(accountListBar->addAction(tr("Import")))->setProperty("tool-widget-name", "Text tool buttons");
    accountListBar->widgetForAction(accountListBar->addAction(tr("Export...")))->setProperty("tool-widget-name", "Text tool buttons");
    accountListBar->widgetForAction(accountListBar->addAction(tr("Delete...")))->setProperty("tool-widget-name", "Text tool buttons");
    accountListBar->addWidget(accountListBarSpacing);
    QToolButton *graphNotionButton = new QToolButton;//accountListBar->addAction(tr("Account list"));
    graphNotionButton->setProperty("tool-widget-name", "Account list");
    connect(graphNotionButton, SIGNAL(clicked()), listGraphNotionMapper, SLOT(map()));
    //accountListBar->addWidget(graphNotionButton);
    accountListBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    */
    QHBoxLayout *accountListButtonLay = new QHBoxLayout;
    accountListButtonLay->addWidget(OBJ_PROP(TOOL_BT("Add New"), "tool-widget-name", "Text tool buttons"));
    accountListButtonLay->addWidget(OBJ_PROP(TOOL_BT("Import"), "tool-widget-name", "Text tool buttons"));
    accountListButtonLay->addWidget(OBJ_PROP(TOOL_BT("Export..."), "tool-widget-name", "Text tool buttons"));
    accountListButtonLay->addWidget(OBJ_PROP(TOOL_BT("Delete..."), "tool-widget-name", "Text tool buttons"));
    accountListButtonLay->addStretch(1);
    QToolButton *graphNotionButton = OBJ_PROP(new QToolButton, "tool-widget-name", "Account list");
    connect(graphNotionButton, SIGNAL(clicked()), listGraphNotionMapper, SLOT(map()));
    accountListButtonLay->addWidget(graphNotionButton);
    accountListButtonLay->setMargin(0);

    QWidget *listNotion = OBJ_NAME(new QWidget, "listNotion");
    QVBoxLayout *listNotionLay = new QVBoxLayout(listNotion);
    listNotionLay->addWidget(accountList, 1);
    //listNotionLay->addWidget(accountListBar);
    listNotionLay->addLayout(accountListButtonLay);
    listNotionLay->setMargin(0);
    listNotionLay->setSpacing(0);

    ///////////        Graphics Notion Actions      /////////////
    QSignalMapper *graphAccountMapper = new QSignalMapper(mw);

    ///////////        Graphics Add New User      /////////////
    QWidget *addNewAccountWidget = new QWidget;
    QVBoxLayout *addNewAccountWidgetLay = new QVBoxLayout(addNewAccountWidget);
    addNewAccountWidgetLay->setMargin(0);
    addNewAccount.avatar = OBJ_NAME(new QLabel, "loginAvatar");
    addNewAccount.login = OBJ_NAME(new QLineEdit, "loginUsername");
    addNewAccount.pass = OBJ_NAME(new QLineEdit, "loginPassword");

    QHBoxLayout *lay;
    #define ADD_CENTRALIZED_WIDGETS(widget1, widget2)   \
        lay = new QHBoxLayout;                          \
        lay->addStretch(1);                             \
        lay->addWidget(widget1);                        \
        if(0 != widget2) lay->addWidget(widget2);       \
        lay->addStretch(1);

    ADD_CENTRALIZED_WIDGETS(addNewAccount.avatar, 0)
    addNewAccountWidgetLay->addLayout(lay);
    ADD_CENTRALIZED_WIDGETS(addNewAccount.login, 0)
    addNewAccountWidgetLay->addLayout(lay);
    ADD_CENTRALIZED_WIDGETS(addNewAccount.pass, 0)
    addNewAccountWidgetLay->addLayout(lay);
    QCheckBox *useProxy = S_CKB("Use Proxy?");
    ADD_CENTRALIZED_WIDGETS(useProxy, S_CKB("Show Advanced Settings"))
    addNewAccountWidgetLay->addLayout(lay);
    ADD_CENTRALIZED_WIDGETS(OBJ_NAME(PBT("Login"), "loginButton"), 0)
    addNewAccountWidgetLay->addLayout(lay);

    ///////////        Graphics Detailed View      /////////////
    QWidget *detailedViewWidget = new QWidget;
    QHBoxLayout *detailedViewWidgetLay = new QHBoxLayout(detailedViewWidget);

    QVBoxLayout *avatarLay = new QVBoxLayout;
    userDetailes.avatar = OBJ_NAME(new QLabel, "userDetailedAvatar");
    avatarLay->addStretch(1);
    ADD_CENTRALIZED_WIDGETS(userDetailes.avatar, 0)
    avatarLay->addLayout(lay);
    avatarLay->addStretch(1);
    ADD_CENTRALIZED_WIDGETS(OBJ_NAME(PBT("Logout"), "logoutButton"), 0)
    avatarLay->addLayout(lay);
    avatarLay->addStretch(1);

    QVBoxLayout *detailedInfoLay = new QVBoxLayout;
    userDetailes.username = OBJ_NAME(LBL("%username%"), "userDetailedUsername");
    userDetailes.followers = OBJ_NAME(LBL("105378"), "userDetailedFollowers");
    userDetailes.following = OBJ_NAME(LBL("16"), "userDetailedFollowing");
    userDetailes.newFollowers = OBJ_NAME(B_LBL("7625"), "userDetailedNewFollowers");

    detailedInfoLay->addStretch(1);
    ADD_CENTRALIZED_WIDGETS(LBL("Hello,"), userDetailes.username)
    detailedInfoLay->addLayout(lay);
    detailedInfoLay->addStretch(1);
    QGridLayout *followersLay = new QGridLayout;
    followersLay->addWidget(userDetailes.followers,     0, 1);
    followersLay->addWidget(ITALIC(LBL("Followers")),   0, 3);
    followersLay->addWidget(userDetailes.following,     1, 1);
    followersLay->addWidget(ITALIC(LBL("Following")),   1, 3);
    followersLay->addWidget(OBJ_NAME(new QWidget, "userDetailedLine"), 2, 0, 1, 5);
    followersLay->setColumnStretch(0, 1);
    followersLay->setColumnStretch(2, 1);
    followersLay->setColumnStretch(4, 1);
    detailedInfoLay->addLayout(followersLay);
    ADD_CENTRALIZED_WIDGETS(userDetailes.newFollowers, 0)
    detailedInfoLay->addLayout(lay);
    ADD_CENTRALIZED_WIDGETS(ITALIC(LBL("New followers made")), 0)
    detailedInfoLay->addLayout(lay);
    detailedInfoLay->addStretch(1);

    detailedViewWidgetLay->addLayout(avatarLay);
    detailedViewWidgetLay->addLayout(detailedInfoLay);

    ///////////        Graphics Account List      /////////////
    QListView *graphAccountListWidget = OBJ_NAME(new QListView, "graphAccountListWidget");
    QStandardItemModel *graphAccountListModel = new QStandardItemModel(graphAccountListWidget);

    QPixmap sandGlass(":/res/icons/sandglass-white.png");
    QPixmap selectedMask(sandGlass.height() * 2, sandGlass.height() * 2);
    {
        selectedMask.fill(QColor(0, 0, 0, 150));
        QPainter painter(&selectedMask);
        painter.drawPixmap(selectedMask.width() / 2 - sandGlass.width() / 2,
                           selectedMask.height() / 2 - sandGlass.height() / 2,
                           sandGlass.width(), sandGlass.height(), sandGlass);
    }
    for(int i = 0; i < 10; i++) {
            QPixmap curPix(":/res/noprofpic.png");
            QPixmap newPix(curPix.size());
            newPix.fill(Qt::transparent);
            QPainter painter(&newPix);
            painter.setClipRegion(QRegion(newPix.rect(), QRegion::Ellipse));
            painter.drawPixmap(0, 0, curPix);

            QStandardItem *item = new QStandardItem();
            item->setIcon(newPix);

            graphAccountListModel->setItem(i, item);
    }
    QIcon curIcon = graphAccountListModel->item(0)->icon();
    QPixmap curPix(curIcon.pixmap(200));
    QPainter painter(&curPix);
    painter.setClipRegion(QRegion(curPix.rect(), QRegion::Ellipse));
    painter.drawPixmap(curPix.rect(), selectedMask);
    graphAccountListModel->item(0)->setIcon(curPix);

    graphAccountListWidget->setModel(graphAccountListModel);
    graphAccountListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ///////////        Bind them all      /////////////
    QStackedWidget *actionWidget = new QStackedWidget;
    actionWidget->addWidget(addNewAccountWidget);
    actionWidget->addWidget(detailedViewWidget);
    actionWidget->addWidget(graphAccountListWidget);

    connect(graphAccountMapper, SIGNAL(mapped(QWidget*)), actionWidget, SLOT(setCurrentWidget(QWidget*)));

    //////////// Graphics Notion Footer ///////////
    QSignalMapper *footerMapper = new QSignalMapper(mw);
    QLabel *footerText = OBJ_NAME(new QLabel, "footerText");
    connect(footerMapper, SIGNAL(mapped(const QString&)), footerText, SLOT(setText(const QString&)));

    //////////// Graphics Notion ToolBar ///////////
    QAction *action;
    QToolBar *graphTab = OBJ_NAME(new QToolBar, "graphBar");

    QLabel *accountHeader = OBJ_NAME(new QLabel(tr("Account Manager")), "accountHeader");
    accountHeader->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    graphTab->addWidget(accountHeader);

    action = graphTab->addAction("Add new");
        graphTab->widgetForAction(action)->setProperty("tool-widget-name", action->text());
        connect(action, SIGNAL(triggered()), graphAccountMapper, SLOT(map()));
        graphAccountMapper->setMapping(action, addNewAccountWidget);
        connect(action, SIGNAL(triggered()), footerMapper, SLOT(map()));
        footerMapper->setMapping(action, tr("To get started, enter your Instagram login details"));
        action->trigger();

    action = graphTab->addAction("Account list");
        graphTab->widgetForAction(action)->setProperty("tool-widget-name", action->text());
        connect(action, SIGNAL(triggered()), graphAccountMapper, SLOT(map()));
        graphAccountMapper->setMapping(action, graphAccountListWidget);
        connect(action, SIGNAL(triggered()), footerMapper, SLOT(map()));
        footerMapper->setMapping(action, tr("Please select an account you wish to manage"));

    action = graphTab->addAction("Detailed account list");
        graphTab->widgetForAction(action)->setProperty("tool-widget-name", action->text());
        connect(action, SIGNAL(triggered()), listGraphNotionMapper, SLOT(map()));
        listGraphNotionMapper->setMapping(action, listNotion);

    graphTab->setMovable(false);
    graphTab->setToolButtonStyle(Qt::ToolButtonIconOnly);

    //////////// Detailed view ////////////
    connect(graphAccountListWidget, SIGNAL(doubleClicked(const QModelIndex&)), graphAccountMapper, SLOT(map()));
    graphAccountMapper->setMapping(graphAccountListWidget, detailedViewWidget);
    connect(graphAccountListWidget, SIGNAL(doubleClicked(const QModelIndex&)), footerMapper, SLOT(map()));
    footerMapper->setMapping(graphAccountListWidget, tr("Logged in succesfully. You can now start using bot"));

    //////////// Graphics Notion ///////////
    QWidget *graphicsNotion = new QWidget;
    QVBoxLayout *graphicsNotionLay = new QVBoxLayout(graphicsNotion);
    graphicsNotionLay->addWidget(graphTab);
    graphicsNotionLay->addWidget(actionWidget);
    graphicsNotionLay->addWidget(footerText);

    graphicsNotionLay->setMargin(0);

    listGraphNotionMapper->setMapping(graphNotionButton, graphicsNotion);

    //////////// Bind them all ///////////
    QStackedWidget *accountManager = OBJ_NAME(new QStackedWidget(mw), "accountManagerWidget");
    accountManager->addWidget(graphicsNotion);
    accountManager->addWidget(listNotion);

    connect(listGraphNotionMapper, SIGNAL(mapped(QWidget*)), accountManager, SLOT(setCurrentWidget(QWidget*)));

    w = accountManager;

    ///////////// Proxy settings /////////////
    QWidget *proxyProps = OBJ_NAME(new QWidget(w), "overlayWidget");

    QVBoxLayout *overlayLay = new QVBoxLayout(proxyProps);
    ADD_CENTRALIZED_WIDGETS(OBJ_NAME(new QLabel, "proxyLogo"), OBJ_NAME(LBL("PROXY SETUP"), "proxySetupLabel"))
    overlayLay->addStretch(1);
    overlayLay->addLayout(lay);
    overlayLay->addStretch(1);

    addNewAccount.proxy.type = new QLineEdit;
    addNewAccount.proxy.ip = new QLineEdit;
    addNewAccount.proxy.user = new QLineEdit;
    addNewAccount.proxy.pass = new QLineEdit;
    QGridLayout *proxyDataLay = new QGridLayout;
    proxyDataLay->addWidget(LBL("proxy type"),          0, 1);
    proxyDataLay->addWidget(addNewAccount.proxy.type,   0, 2);
    proxyDataLay->addWidget(LBL("proxy IP"),            1, 1);
    proxyDataLay->addWidget(addNewAccount.proxy.ip,     1, 2);
    proxyDataLay->addWidget(LBL("username"),            2, 1);
    proxyDataLay->addWidget(addNewAccount.proxy.user,   2, 2);
    proxyDataLay->addWidget(LBL("password"),            3, 1);
    proxyDataLay->addWidget(addNewAccount.proxy.pass,   3, 2);
    proxyDataLay->setColumnStretch(0, 1);
    proxyDataLay->setColumnStretch(3, 1);
    overlayLay->addLayout(proxyDataLay);
    overlayLay->addStretch(1);

    QPushButton *proxySaveButton = OBJ_NAME(PBT("Save"), "proxySaveButton");
    QPushButton *proxyCancelButton = OBJ_NAME(PBT("Cancel"), "proxyCancelButton");
    ADD_CENTRALIZED_WIDGETS(proxySaveButton, proxyCancelButton)
    overlayLay->addLayout(lay);
    overlayLay->addStretch(1);

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(proxyProps);
    effect->setOpacity(0.93);
    proxyProps->setGraphicsEffect(effect);
    proxyProps->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    proxyProps->hide();

    connect(useProxy, SIGNAL(clicked(bool)), proxyProps, SLOT(setVisible(bool)));
    connect(useProxy, SIGNAL(clicked(bool)), proxyProps, SLOT(raise()));
    connect(proxySaveButton, SIGNAL(clicked()), proxyProps, SLOT(hide()));
    connect(proxyCancelButton, SIGNAL(clicked()), proxyProps, SLOT(hide()));

    return w;
}
QWidget* MainUI::GetLicenseInfoView() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    w = OBJ_NAME(new QWidget(mw), "licenseInfoWidget");

    QGridLayout *statusLay = new QGridLayout;
    licenseInfo.subscrStatus = OBJ_NAME(S_LBL("Active"), "subscrStatus");
    licenseInfo.subscrType = OBJ_NAME(S_LBL("Cool subscr"), "subscrType");
    statusLay->addWidget(S_LBL("Subscription status:"), 0, 1);
    statusLay->addWidget(licenseInfo.subscrStatus,    0, 2);
    statusLay->addWidget(S_LBL("License type:"),   1, 1);
    statusLay->addWidget(licenseInfo.subscrType,      1, 2);
    statusLay->setColumnStretch(0, 1);
    statusLay->setColumnStretch(3, 1);

    QGridLayout *datesLay = new QGridLayout;
    licenseInfo.subscrStarted = OBJ_NAME(S_LBL("02.14.2016"), "subscrStarted");
    licenseInfo.subscrEnded = OBJ_NAME(S_LBL("04.14.2016"), "subscrEnded");
    datesLay->addWidget(S_LBL("Subscription started:"), 0, 1);
    datesLay->addWidget(licenseInfo.subscrStarted,      0, 2);
    datesLay->addWidget(S_LBL("Subscription ends on:"), 1, 1);
    datesLay->addWidget(licenseInfo.subscrEnded,        1, 2);
    datesLay->setColumnStretch(0, 1);
    datesLay->setColumnStretch(3, 1);

    QHBoxLayout *buttonLay = new QHBoxLayout;
    QPushButton *manageButton = OBJ_NAME(S_PBT("Manage License"), "manageLicenseButton");
    buttonLay->addStretch(1);
    buttonLay->addWidget(manageButton);
    buttonLay->addStretch(1);

    QString tipText = tr("If you click on Manage License, you will be "
                         "redirected to the website where you can "
                         "renew or abort your license.");

    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->addWidget(OBJ_NAME(LBL("License Info"), "licenseHeader"));
    lay->addStretch(1);
    lay->addLayout(statusLay);
    lay->addStretch(1);
    lay->addLayout(datesLay);
    lay->addStretch(1);
    lay->addLayout(buttonLay);
    lay->addStretch(1);
    lay->addWidget(OBJ_NAME(new QLabel(tipText), "licenseTipText"));
    lay->addStretch(1);

    lay->setMargin(1);

    connect(manageButton, SIGNAL(clicked()), mw, SLOT(applyStyle()));

    return w;
}
QWidget* MainUI::GetBotWidget() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    QTabWidget *tab = new QTabWidget(mw);
    tab->setObjectName("botTabWidget");

    tab->addTab(GetLikerTab(),          tr("Liker"));
    tab->addTab(GetFollowerTab(),       tr("Follower"));
    tab->addTab(GetUnfollowerTab(),     tr("Unfollower"));
    tab->addTab(GetCommenterTab(),      tr("Commenter"));
    tab->addTab(GetMixedTab(),          tr("Mixed"));
    tab->addTab(GetMassActionsTab(),    tr("Mass Actions"));

    w = tab;

    return w;
}
QWidget* MainUI::GetUploaderWidget() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    w = new QLabel("GetUploaderWidget", mw);

    return w;
}
QWidget* MainUI::GetToolsWidget() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    w = new QLabel("GetToolsWidget", mw);

    return w;
}
QWidget* MainUI::GetSettingsWidget() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    w = new QLabel("GetSettingsWidget", mw);

    return w;
}
QWidget* MainUI::GetHelpWidget() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    w = new QLabel("GetHelpWidget", mw);

    return w;
}
QWidget* MainUI::GetLikerTab() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    ///////////////////////////////// Left Column /////////////////////////////////////////
    QHBoxLayout *methodLay = new QHBoxLayout;
    likerOptions.method = new QComboBox;
    likerOptions.method->addItem(tr("Hashtags"));
    likerOptions.method->addItem(tr("Username"));
    methodLay->addWidget(LBL("Method of collecting photos:"));
    methodLay->addWidget(likerOptions.method);

    QWidget *hashtagsParams = new QWidget;
    QGridLayout *hashtagsParamsLay = new QGridLayout(hashtagsParams);
    likerOptions.hashtags = new QTextEdit;
    likerOptions.hashtags->setObjectName("likerHashtags");
    likerOptions.picsPerHashtag = TXT_CNTR(S_LED("50"));
    hashtagsParamsLay->addWidget(LBL("HASHTAGS"),               0, 0, 1, 3);
    hashtagsParamsLay->addWidget(likerOptions.hashtags,         1, 0, 1, 3);
    hashtagsParamsLay->addWidget(S_LBL("Pictures per tag:"),    2, 0);
    hashtagsParamsLay->addWidget(likerOptions.picsPerHashtag,   2, 1);
    hashtagsParamsLay->setColumnStretch(2, 1);


    QWidget *usernameParams = new QWidget;
    QGridLayout *usernameParamsLay = new QGridLayout(usernameParams);
    likerOptions.usernames = new QTextEdit;
    likerOptions.usernames->setObjectName("likerUsernames");
    likerOptions.picsPerUsername = TXT_CNTR(S_LED("50"));
    usernameParamsLay->addWidget(LBL("USERNAMES"),                  0, 0, 1, 3);
    usernameParamsLay->addWidget(likerOptions.usernames,            1, 0, 1, 3);
    usernameParamsLay->addWidget(S_LBL("Pictures per username:"),   2, 0);
    usernameParamsLay->addWidget(likerOptions.picsPerUsername,      2, 1);
    usernameParamsLay->setColumnStretch(2, 1);

    QStackedLayout *methodParamsLay = new QStackedLayout;
    methodParamsLay->addWidget(hashtagsParams);
    methodParamsLay->addWidget(usernameParams);
    connect(likerOptions.method, SIGNAL(activated(int)), methodParamsLay, SLOT(setCurrentIndex(int)));

    QHBoxLayout *likesToSendLay = new QHBoxLayout;
    likerOptions.likesToSend = TXT_CNTR(new QLineEdit("1000"));
    likesToSendLay->addWidget(LBL("Likes to send:"));
    likesToSendLay->addWidget(likerOptions.likesToSend);
    likesToSendLay->addStretch(1);

    likerOptions.dontLimitLikes = S_CKB("Don't limit the number of likes to send.");

    QVBoxLayout *leftColumn = new QVBoxLayout;
    leftColumn->addLayout(methodLay);
    leftColumn->addLayout(methodParamsLay);
    leftColumn->addLayout(likesToSendLay);
    leftColumn->addWidget(likerOptions.dontLimitLikes);
    leftColumn->addStretch(1);

    ///////////////////////////////// Right Column /////////////////////////////////////////
    QGridLayout *delayLay = new QGridLayout;
    likerOptions.minDelay = TXT_CNTR(new QLineEdit("10"));
    likerOptions.maxDelay = TXT_CNTR(new QLineEdit("200"));
    delayLay->addWidget(S_LBL("min(sec)"),              0, 1);
    delayLay->addWidget(S_LBL("max(sec)"),              0, 2);
    delayLay->addWidget(LBL("Delay between likes:"),    1, 0);
    delayLay->addWidget(likerOptions.minDelay,          1, 1);
    delayLay->addWidget(likerOptions.maxDelay,          1, 2);
    delayLay->setColumnStretch(0, 1);

    QGridLayout *timedSleepLay = new QGridLayout;
    likerOptions.timedSleepEnable = S_CKB("Sleep after");
    likerOptions.timedSleepPeriod = TXT_CNTR(S_LED("400"));
    likerOptions.minTimedSleep = TXT_CNTR(S_LED("1000"));
    likerOptions.maxTimedSleep = TXT_CNTR(S_LED("5000"));
    timedSleepLay->addWidget(likerOptions.timedSleepEnable,     0, 0);
    timedSleepLay->addWidget(likerOptions.timedSleepPeriod,     0, 1);
    timedSleepLay->addWidget(S_LBL("seconds for min"),          0, 2, 1, 2);
    timedSleepLay->addWidget(likerOptions.minTimedSleep,        0, 4);
    timedSleepLay->addWidget(S_LBL("sec"),                      0, 5);
    timedSleepLay->addWidget(S_LBL("max"),                      1, 3);
    timedSleepLay->addWidget(likerOptions.maxTimedSleep,        1, 4);
    timedSleepLay->addWidget(S_LBL("sec."),                     1, 5);
    timedSleepLay->setColumnStretch(2, 1);


    QGridLayout *likesSleepLay = new QGridLayout;
    likerOptions.likesSleepEnable = S_CKB("Sleep after");
    likerOptions.likesSleepAmount = TXT_CNTR(S_LED("400"));
    likerOptions.minLikesSleep = TXT_CNTR(S_LED("1000"));
    likerOptions.maxLikesSleep = TXT_CNTR(S_LED("5000"));
    likesSleepLay->addWidget(likerOptions.likesSleepEnable,     0, 0);
    likesSleepLay->addWidget(likerOptions.likesSleepAmount,     0, 1);
    likesSleepLay->addWidget(S_LBL("likes for min"),            0, 2, 1, 2);
    likesSleepLay->addWidget(likerOptions.minLikesSleep,        0, 4);
    likesSleepLay->addWidget(S_LBL("sec"),                      0, 5);
    likesSleepLay->addWidget(S_LBL("max"),                      1, 3);
    likesSleepLay->addWidget(likerOptions.maxLikesSleep,        1, 4);
    likesSleepLay->addWidget(S_LBL("sec."),                     1, 5);
    likesSleepLay->setColumnStretch(2, 1);

    QVBoxLayout *rightColumn = new QVBoxLayout;
    rightColumn->addWidget(BOLD(LBL("Sleeping and Delays")));
    rightColumn->addLayout(delayLay);
    rightColumn->addLayout(timedSleepLay);
    rightColumn->addLayout(likesSleepLay);
    rightColumn->addStretch(1);

    ///////////////////////////////// Button Layout ////////////////////////////////////////
    QHBoxLayout *buttonLay = new QHBoxLayout;
    QPushButton *startLiker = B_PBT("Start Liker");
    QPushButton *stopLiker = B_PBT("Stop Liker");
    buttonLay->addStretch(1);
    buttonLay->addWidget(startLiker);
    buttonLay->addWidget(stopLiker);
    buttonLay->addStretch(1);

    startLiker->setObjectName("startLikerPB");
    stopLiker->setObjectName("stopLikerPB");

    w = new QWidget(mw);
    w->setObjectName("likerTab");
    QGridLayout *lay = new QGridLayout(w);
    lay->addLayout(leftColumn,      0, 0);
    lay->addLayout(rightColumn,     0, 2);
    lay->addLayout(buttonLay,       1, 0, 1, 3);
    lay->setColumnStretch(1, 1);

    return w;
}
QWidget* MainUI::GetFollowerTab() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    w = new QLabel("GetFollowerTab", mw);

    return w;
}
QWidget* MainUI::GetUnfollowerTab() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    w = new QLabel("GetUnfollowerTab", mw);

    return w;
}
QWidget* MainUI::GetCommenterTab() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    w = new QLabel("GetCommenterTab", mw);

    return w;
}
QWidget* MainUI::GetMixedTab() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    w = new QLabel("GetMixedTab", mw);

    return w;
}
QWidget* MainUI::GetMassActionsTab() {
    static QWidget *w = 0;

    if(0 != w)
        return w;

    w = new QLabel("GetMassActionsTab", mw);

    return w;
}
