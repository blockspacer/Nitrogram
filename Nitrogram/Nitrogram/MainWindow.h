#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainUI;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void applyStyle();

private:
    MainUI *ui;
};

#endif // MAINWINDOW_H
