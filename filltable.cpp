#include "filltable.h"

FillTable::FillTable(QObject *parent, QString path)
    : QThread{parent}, path(path)
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
    // auto shapesCnt = shapes->querySubObject("Count");
    //qDebug() << "SHAPES CNT" << shapesCnt.toInt();
    // emit tableFillStarted(shapes_count);
    for(int i = 0, j = 3; i < shapes_count; ++i) {
        auto picture = sheet->querySubObject("Shapes(int)", j);
        picture->dynamicCall("Copy()");
        // const QClipboard *clipboard = QApplication::clipboard();
        // const QMimeData *mimeData = clipboard->mimeData();
        emit shapeReady(i, shapes_count);
        ++j;
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
    emit tableFillFinished(shapes_count);
}
