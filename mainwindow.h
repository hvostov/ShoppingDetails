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
    QWaitCondition *dataDealed;

signals:
    void havePath(QString str);

private slots:
    void on_loadDataButton_clicked();
    void fillImage(int i, int cnt);
    void fillCell(QString str, int i, int j);
    void countProfit();
    void startWorkInAThread(QString path);
    void statusFinished(int cnt);

private:
    Ui::MainWindow *ui;
    QPushButton *profitButton;
    QPushButton *exchangeButton;
    QLineEdit *profitLineEdit;
    QLineEdit *exchangeLineEdit;
    QLabel *totalCostLabelValue;
    QLabel *totalCostLabelName;
    QProgressBar *progressBar;
    QLabel *statusLabel;
    int shapes_count = 0;
    double progressValue = 0;
    double onePercent = 0;
    QLabel *markUpTotalCostLabelValue;
    QLabel *markUpTotalCostLabelName;
    QLabel *purchaseCostsLabel;
    QLabel *goodsLabel;
    QLabel *shippingLabel;
    QLineEdit *goodsLineEdit;
    QLineEdit *shippingLineEdit;
    QLabel *revenueLabelTitle;
    QLabel *revenueLabelValue;


};
#endif // MAINWINDOW_H
