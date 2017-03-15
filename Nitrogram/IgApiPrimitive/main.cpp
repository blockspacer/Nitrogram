#include <QCoreApplication>

#define IG_USERNAME         "hoxixepovi"
#define IG_PASSWORD         "Qwerty_123$"

#include "HumanEmulator.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    IgAccount igAccount;
    igAccount.username = IG_USERNAME;
    igAccount.password = IG_PASSWORD;
    igAccount.igDataPath = "c:\\projects\\NitrogramRepo\\DesktopApp\\Nitrogram\\_out\\temp\\";

    HumanEmulator humanEmulator(igAccount);
    humanEmulator.start();

    return a.exec();
}
