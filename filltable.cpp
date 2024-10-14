#include "filltable.h"

FillTable::FillTable(QObject *parent, QString path, QMutex *m, QWaitCondition *dataDealed)
    : QThread{parent}, path(path), m_(m), dataDealed_(dataDealed)
{}

void FillTable::run()
{
    auto excel     = new QAxObject("Excel.Application");
    auto workbooks = excel->querySubObject("Workbooks");
    auto workbook  = workbooks->querySubObject("Open(const QString&)",path);
    auto sheets    = workbook->querySubObject("Worksheets");
    auto sheet     = sheets->querySubObject("Item(int)", 1);
    auto shapes    = sheet->querySubObject("Shapes");
    auto shapesCnt = shapes->property("Count");
    this->shapes_count   = shapesCnt.toInt() - 2;

    for(int i = 0, j = 3; i < shapes_count; ++i) {
        m_->lock();
        auto picture = sheet->querySubObject("Shapes(int)", j);
        picture->dynamicCall("Copy()");
        ++j;
        emit shapeReady(i, shapes_count);
        dataDealed_->wait(m_);
        m_->unlock();
    }

    for (int row = 19, i = 0; row <= 19 + shapes_count; ++row, ++i)
    {
        for(int col = 4, j = 0; col < 14; ++col) {
            if (col != 7) {
                m_->lock();
                auto cCell = sheet->querySubObject("Cells(int,int)", row, col);
                QString str = cCell->dynamicCall("Value()").toString();
                emit cellReady(str, i, j);
                ++j;
                dataDealed_->wait(m_);
                m_->unlock();
            }
        }
    }
    workbook->dynamicCall("Close (Boolean)", true);
    excel->dynamicCall("Quit (void)");

    delete excel;
    emit tableFillFinished(shapes_count);

}
