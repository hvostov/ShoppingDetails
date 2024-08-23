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
    explicit FillTable(QObject *parent = nullptr, QString path = "", QMutex *m = nullptr, QWaitCondition *imageDealed = nullptr);
    void run() override;

signals:
    void shapeReady(int i, int cnt);
    void cellReady(QString str, int i, int j);
    void tableFillFinished(int cnt);
    // void tableFillStarted(int shapes_count);
private:
    QString path;
    QMutex *m_;
    int shapes_count = 0;
    QWaitCondition* imageDealed_;

};

#endif // FILLTABLE_H
