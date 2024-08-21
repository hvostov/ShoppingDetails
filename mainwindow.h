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

signals:
    void havePath(QString str);

private slots:
    void on_pushButton_clicked();
    void fillTable(QString str);
    void on_profitButton_clicked();

private:
    Ui::MainWindow *ui;
    QPushButton *profitButton;
    QPushButton *exchangeButton;
    QLineEdit *profitLineEdit;
    QLineEdit *exchangeLineEdit;
    QLabel *totalCostLabel;
    QLabel *totalCostLabel1;
    int shapes_count;

};
#endif // MAINWINDOW_H
