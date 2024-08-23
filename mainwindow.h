#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QtAxContainer/QAxObject>
#include <QtAxBase/QtAxBase>
#include <QtAxContainer/QAxBase>
#include <QtAxContainer/QAxBaseWidget>
#include <QHBoxLayout>
#include <QClipboard>
#include <QPushButton>
#include <QLineEdit>
#include <QSpacerItem>
#include <cmath>
#include <QProgressBar>
#include "filltable.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QMutex *m;
    QWaitCondition *imageDealed;
    // static QWaitCondition imageCopied;
    // static QMutex mutex;

signals:
    void havePath(QString str);

private slots:
    void on_pushButton_clicked();
    //void fillTable(QString str);
    void fillImage(int i, int cnt);
    void fillCell(QString str, int i, int j);
    // void on_profitButton_clicked();
    void countProfit();
    void startWorkInAThread(QString path);
    void statusFinished(int cnt);
    // void onTableFillStarted(int cnt);

private:
    Ui::MainWindow *ui;
    QPushButton *profitButton;
    QPushButton *exchangeButton;
    QLineEdit *profitLineEdit;
    QLineEdit *exchangeLineEdit;
    QLabel *totalCostLabel;
    QLabel *totalCostLabel1;
    QProgressBar *progressBar;
    QLabel *statusLabel;
    int shapes_count = 0;
    double progressValue = 0;
    double onePercent = 0;
    QLabel *markUpTotalCostLabel;
    QLabel *markUpTotalCostLabel1;


};
#endif // MAINWINDOW_H
