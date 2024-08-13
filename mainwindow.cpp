#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    profitButton(new QPushButton("Рассчитать цену продажи товара")),
    // exchangeButton(new QPushButton("Рассчитать цену в рублях")),
    profitLineEdit(new QLineEdit()),
    exchangeLineEdit(new QLineEdit()),
    totalCostLabel(new QLabel(this)),
    totalCostLabel1(new QLabel(this))
{
    ui->setupUi(this);

    profitButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // exchangeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    profitButton->setMinimumWidth(200);
    // exchangeButton->setMinimumWidth(200);
    // ui->gridLayout->setSpacing(100);
    profitLineEdit->setPlaceholderText("Введите процент наценки");
    profitLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    exchangeLineEdit->setPlaceholderText("Введите курс");
    exchangeLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QGridLayout *rightLayout = new QGridLayout();
    ui->gridLayout->addLayout(rightLayout, 0, 5);
    // QSpacerItem *spacer1 = new QSpacerItem(1,1,QSizePolicy::Expanding, QSizePolicy::Fixed);
    // rightLayout->addItem(spacer1, 0, 1);
    // rightLayout->setSizeConstraint(QLayout::SetFixedSize);
    // rightLayout->setRowStretch(3,0);
    // rightLayout->setColumnStretch(1,1);
    rightLayout->setVerticalSpacing(3);
    // rightLayout->set

    QLabel *profitLabel = new QLabel(this);
    QLabel *exchangeLabel = new QLabel(this);
    //QLabel *totalCostLabel = new QLabel(this);
    // QSpacerItem *spacerItem = new QSpacerItem(40,20);
    profitLabel->setText("Наценка, %");
    exchangeLabel->setText("Курс Р к Ю");
    // profitLabel->setMaximumWidth(100);
    rightLayout->addWidget(profitLabel, 0, 1, Qt::AlignTop);
    rightLayout->addWidget(profitButton, 2, 1, 1, 2, Qt::AlignTop );
    rightLayout->addWidget(profitLineEdit, 0, 2, Qt::AlignTop);
    // rightLayout->addItem(spacerItem, 2,1,1,2);
    rightLayout->addWidget(exchangeLabel, 1, 1, Qt::AlignTop);
    rightLayout->addWidget(totalCostLabel, 7, 1, 1, 2, Qt::AlignHCenter);
    rightLayout->addWidget(totalCostLabel1, 6, 1, 1, 2, Qt::AlignHCenter);
    totalCostLabel1->setText("Общая стоимость, руб :");
    // rightLayout->addWidget(exchangeButton, 4, 1, 1, 2, Qt::AlignTop );
    rightLayout->addWidget(exchangeLineEdit, 1, 2, Qt::AlignTop);
    // rightLayout->setRowStretch(1,1);
    // rightLayout->setRowMinimumHeight(0, 10);
    rightLayout->setRowStretch(0,0);
    rightLayout->setRowStretch(4,2);
    rightLayout->setColumnStretch(0, 0);
    connect(this, &MainWindow::havePath, this, &MainWindow::fillTable);
    connect(MainWindow::profitButton, &QPushButton::clicked, this, &MainWindow::on_profitButton_clicked);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString str;
    str = QFileDialog::getOpenFileName(this, "Выбрать файл", "C:/Users/ZBook/Desktop",
                                        "XLS Files (*.xls);; XLSX Files(*.xlsx);");
    qDebug() << str;
    emit havePath(str);
}

void MainWindow::on_profitButton_clicked() {
    // qDebug() << "WOIERJFOA3FEAJIRGOIAEJ";
    double mult = 1 + profitLineEdit->text().toDouble()/100;
    double exchange = exchangeLineEdit->text().toDouble();
    // qDebug() << mult;
    // QString s = ui->tableWidget->item(1, 6)->text();
    // //QTableWidgetItem *widgetItem = ui->tableWidget->item(1, 9);

    // qDebug() << s;
    // // int itemValue = widgetItem->data(Qt::DisplayRole).toInt();
    // double itemValue = s.toDouble();
    // qDebug() << itemValue;
    // itemValue *= mult;
    // QString str = QString::number(itemValue);
    // qDebug() << str;
    double total = 0;
    for(int row = 0; row < 50; ++row) {
        double itemValue = ui->tableWidget->item(row, 6)->text().toDouble();
        double itemValue1 = ui->tableWidget->item(row, 8)->text().toDouble();

        total += itemValue1*exchange;
        itemValue = std::round(itemValue*exchange*10)/10;
        QString str = QString::number(itemValue);
            QTableWidgetItem *pCell = ui->tableWidget->item(row, 7);
            if(!pCell)
            {
                pCell = new QTableWidgetItem;
                ui->tableWidget->setItem(row, 7, pCell);
            }
            pCell->setText(str);

            itemValue = std::round(itemValue*mult*10)/10;
            str = QString::number(itemValue);
            pCell = ui->tableWidget->item(row, 9);
            if(!pCell)
            {
                pCell = new QTableWidgetItem;
                ui->tableWidget->setItem(row, 9, pCell);
            }
            pCell->setText(str);
    }
    totalCostLabel->setText(QString::number(total));

}

void MainWindow::fillTable(QString str)
{
    auto excel     = new QAxObject("Excel.Application");
    auto workbooks = excel->querySubObject("Workbooks");
    auto workbook  = workbooks->querySubObject("Open(const QString&)",str);
    auto sheets    = workbook->querySubObject("Worksheets");
    auto sheet     = sheets->querySubObject("Item(int)", 1);
    auto shapes    = sheet->querySubObject("Shapes");
    auto shapesCnt = shapes->property("Count");
    // auto shapesCnt = shapes->querySubObject("Count");
    qDebug() << "SHAPES CNT" << shapesCnt.toInt();

    for(int i = 0, j = 3; i < 2; ++i) {
        //const QString s = "Рисунок " + QString::number(i);
        auto picture = sheet->querySubObject("Shapes(int)", j);
        picture->querySubObject("Copy()");
        const QClipboard *clipboard = QApplication::clipboard();
        const QMimeData *mimeData = clipboard->mimeData();
        if (mimeData->hasImage()) {
            QLabel *label = new QLabel(this);
            label->setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
            label->setScaledContents(true);
            ui->tableWidget->setCellWidget(i,2, label);
            ++j;
        }
        else if (i > 0) {
            --i;
        }
    }

    for (int row = 19, i = 0; (row <= 68); ++row, ++i)
    {
        for(int col = 4, j = 0; col < 14; ++col
             ) {
            if (col != 7) {
                auto cCell = sheet->querySubObject("Cells(int,int)", row, col);
                //ui->tableWidget->//setCellWidget(i, j, cCell->dynamicCall("Value()").toString());
                QString str = cCell->dynamicCall("Value()").toString();
                QTableWidgetItem *pCell = ui->tableWidget->item(i, j);
                if(!pCell)
                {
                    pCell = new QTableWidgetItem;
                    ui->tableWidget->setItem(i, j, pCell);
                }
                pCell->setText(str);
                ++j;
            }

        }
    }

    // const QString s = "Рисунок 4";
    // //auto picture4 = sheet->querySubObject("Shapes(const QString&)", s);
    // auto picture4 = sheet->querySubObject("Shapes(int)", 2);
    // picture4->querySubObject("Copy()");

    // const QClipboard *clipboard = QApplication::clipboard();
    // const QMimeData *mimeData = clipboard->mimeData();

    // QLabel *label2 = new QLabel(this);

    //if (mimeData->hasImage()) {
        // label2->setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
        // label2->setScaledContents(true);
        // ui->tableWidget->setCellWidget(1,1, label2);
    // }
    // else {
    //     qDebug() << "no image!!!!!!!!!!";
    // }



    // read the first cells in row 1..5
    for (int r = 6; (r <= 12); ++r)
    {
        auto cCell = sheet->querySubObject("Cells(int,int)",r,5);
        qDebug() << cCell->dynamicCall("Value()").toString();//toInt();
    }
    // QLabel *label1 = new QLabel(this);
    // QPixmap pixMap("C:/Users/ZBook/Desktop/excelForQt/Рисунок1.png");
    // label1->setPixmap(pixMap);
    // label1->setScaledContents(true);
    // ui->tableWidget->setCellWidget(2,2, label1);

}

