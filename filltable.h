#ifndef FILLTABLE_H
#define FILLTABLE_H

#include <QObject>
#include <QtAxContainer/QAxObject>
#include <QtAxBase/QtAxBase>
#include <QtAxContainer/QAxBase>
#include <QtAxContainer/QAxBaseWidget>
#include <QLabel>
#include <QThread>
#include <QString>

class FillTable : public QThread
{
    Q_OBJECT
public:
    explicit FillTable(QObject *parent = nullptr, QString path = "");
    void run() override;

signals:
    void shapeReady(int i, int cnt);
    void cellReady(QString str, int i, int j);
    void tableFillFinished(int cnt);
    // void tableFillStarted(int shapes_count);
private:
    QString path;
    int shapes_count = 0;

};

#endif // FILLTABLE_H
