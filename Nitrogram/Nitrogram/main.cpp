#include <QApplication>

#include <QFile>
#include <QDir>

#include <QList>
#include <QString>
#include <QStringList>

#include <QFontDatabase>
#include <QStyleFactory>

#include <QMessageBox>

#include "MainWindow.h"
#include "LoginScreen.h"

void LoadFonts() {
    QStringList families;

    foreach(const QString &font, QDir(":/res/fonts/used/").entryList()) {
        int id = QFontDatabase::addApplicationFont(":/res/fonts/used/" + font);
        families += QFontDatabase::applicationFontFamilies(id);
    }

    QString msg;
    QFontDatabase fdb;
    foreach(const QString &curFamily, families) {
        msg += "Family: \"" + curFamily + "\"\n";

        QStringList styles = fdb.styles(curFamily);
        foreach(const QString &curStyle, styles) {
            bool isSmothyScalable = fdb.isSmoothlyScalable(curFamily, curStyle);
            msg += "Style \"" + curStyle + "\" is" + (isSmothyScalable ? " " : " NOT ") + "smoothy scalable\n";
            if(!isSmothyScalable)
                continue;

            QList<int> smoothSizes = fdb.smoothSizes(curFamily, curStyle);
            QStringList smoothSizesStr;
            foreach(int curSize, smoothSizes) {
                smoothSizesStr << QString("%1").arg(curSize);
            }

            msg += "Smooth sizes: " + smoothSizesStr.join(", ") + "\n";
        }
        msg += "----------------------------\n";
    }

    //QMessageBox::information(0, "", msg);
}

void LoadStyle() {
    QFile f(":/Nitrogram.css");
    if(f.open(QIODevice::ReadOnly)) {
        qobject_cast<QApplication*>(QApplication::instance())->setStyleSheet(f.readAll());
        f.close();
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    LoadFonts();
    LoadStyle();

    //*
    MainWindow w;
    /*/
    LoginScreen w;
    //*/

    w.show();

    return a.exec();
}
