#include "filltable.h"

FillTable::FillTable(QObject *parent, QString path, QMutex *m)
    : QThread{parent}, path(path), m_(m)
{}

void FillTable::run()
{
     // m_->lock();
    auto excel     = new QAxObject("Excel.Application");
    auto workbooks = excel->querySubObject("Workbooks");
    auto workbook  = workbooks->querySubObject("Open(const QString&)",path);
    auto sheets    = workbook->querySubObject("Worksheets");
    auto sheet     = sheets->querySubObject("Item(int)", 1);
    auto shapes    = sheet->querySubObject("Shapes");
    auto shapesCnt = shapes->property("Count");
    this->shapes_count   = shapesCnt.toInt() - 2;
    // auto shapesCnt = shapes->querySubObject("Count");
    //qDebug() << "SHAPES CNT" << shapesCnt.toInt();
    // emit tableFillStarted(shapes_count);
    QDeadlineTimer *timer = new QDeadlineTimer(1000);
    for(int i = 0, j = 3; i < shapes_count; ++i) {
        // while(!m_->tryLock()) {}
        // m_->unlock();
        // if (m_->tryLock(1000)) {
            auto picture = sheet->querySubObject("Shapes(int)", j);
            picture->dynamicCall("Copy()");
            // picture->querySubObject("Copy()");
            // const QClipboard *clipboard = QApplication::clipboard();
            // const QMimeData *mimeData = clipboard->mimeData();

            // emit shapeReady(i, shapes_count);
            ++j;
            // m_->unlock();
            emit shapeReady(i, shapes_count);
             msleep(100);
        // }
        // m_->unlock();
    }

    for (int row = 19, i = 0; row <= 19 + shapes_count; ++row, ++i)
    {
        for(int col = 4, j = 0; col < 14; ++col) {
            if (col != 7) {
                auto cCell = sheet->querySubObject("Cells(int,int)", row, col);
                QString str = cCell->dynamicCall("Value()").toString();
                emit cellReady(str, i, j);
                ++j;
            }

        }
        //qApp->processEvents();
    }
    workbook->dynamicCall("Close (Boolean)", true);
    excel->dynamicCall("Quit (void)");

    delete excel;
    emit tableFillFinished(shapes_count);

}
