#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    profitButton(new QPushButton("Рассчитать цену продажи товара")),
    profitLineEdit(new QLineEdit()),
    exchangeLineEdit(new QLineEdit()),
    totalCostLabelValue(new QLabel(this)),
    totalCostLabelName(new QLabel(this)),
    progressBar(new QProgressBar(this)),
    statusLabel(new QLabel(this)),
    markUpTotalCostLabelValue(new QLabel(this)),
    markUpTotalCostLabelName(new QLabel(this)),
    m(new QMutex()),
    dataDealed(new QWaitCondition()),
    purchaseCostsLabel(new QLabel(this)),
    goodsLabel(new QLabel(this)),
    shippingLabel(new QLabel(this)),
    revenueLabelTitle(new QLabel(this)),
    revenueLabelValue(new QLabel(this)),
    goodsLineEdit(new QLineEdit(this)),
    shippingLineEdit(new QLineEdit(this))
{

    ui->setupUi(this);

    QGridLayout *rightLayout = new QGridLayout();
    ui->gridLayout->addLayout(rightLayout, 0, 5);
    rightLayout->setVerticalSpacing(10);
    rightLayout->setRowMinimumHeight(3, 100);
    rightLayout->setRowStretch(4,2);
    rightLayout->setColumnStretch(0, 0);
    rightLayout->setRowMinimumHeight(7, 80);
    rightLayout->setRowMinimumHeight(8, 80);

    QHBoxLayout * statusHlayout = new QHBoxLayout();

    QWidget * statusWidget = new QWidget;
    statusWidget->setLayout(statusHlayout);
    ui->statusbar->addPermanentWidget(statusWidget, 2);

    purchaseCostsLabel->setText("Затраты на покупку товара:");
    rightLayout->addWidget(purchaseCostsLabel, 4, 1, 1, 2, Qt::AlignBottom);

    profitButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    profitButton->setMinimumWidth(200);
    rightLayout->addWidget(profitButton, 2, 1, 1, 2);
    connect(profitButton, &QPushButton::clicked, this, &MainWindow::countProfit);

    profitLineEdit->setPlaceholderText("Введите процент наценки");
    profitLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rightLayout->addWidget(profitLineEdit, 0, 2);

    exchangeLineEdit->setPlaceholderText("Введите курс");
    exchangeLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rightLayout->addWidget(exchangeLineEdit, 1, 2);

    statusLabel->setText("");
    statusLabel->setMinimumSize(200,20);
    statusHlayout->addWidget(statusLabel);

    goodsLabel->setText("Товар");
    rightLayout->addWidget(goodsLabel, 5, 1);

    goodsLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rightLayout->addWidget(goodsLineEdit, 5, 2);

    shippingLabel->setText("Доставка");
    rightLayout->addWidget(shippingLabel, 6, 1);

    shippingLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rightLayout->addWidget(shippingLineEdit, 6, 2);

    revenueLabelTitle->setText("Выручка:");
    rightLayout->addWidget(revenueLabelTitle, 14, 1, 1, 2, Qt::AlignBottom);

    rightLayout->addWidget(totalCostLabelValue, 11, 1, 1, 2, Qt::AlignBottom);

    rightLayout->addWidget(markUpTotalCostLabelValue, 13, 1, 1, 2, Qt::AlignBottom);

    rightLayout->addWidget(revenueLabelValue, 15, 1, 1, 2, Qt::AlignBottom);

    totalCostLabelName->setText("Общая стоимость, руб :");
    rightLayout->addWidget(totalCostLabelName, 10, 1, 1, 2, Qt::AlignBottom);

    markUpTotalCostLabelName->setText("Общая стоимость с наценкой, руб :");
    rightLayout->addWidget(markUpTotalCostLabelName, 12, 1, 1, 2, Qt::AlignBottom);

    ui->statusbar->addPermanentWidget(progressBar);
    progressBar->setRange(0, 100);
    progressBar->setTextVisible(true);
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setInvertedAppearance(true);
    statusHlayout->addWidget(progressBar);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    QLabel *profitLabel = new QLabel(this);
    profitLabel->setText("Наценка, %");
    rightLayout->addWidget(profitLabel, 0, 1);

    QLabel *exchangeLabel = new QLabel(this);
    exchangeLabel->setText("Курс Р к Ю");
    rightLayout->addWidget(exchangeLabel, 1, 1);

    connect(this, &MainWindow::havePath, this, &MainWindow::startWorkInAThread);

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadDataButton_clicked()
{
    QString str;
    str = QFileDialog::getOpenFileName(this, "Выбрать файл", "C:/Users/ZBook/Desktop",
                                        "XLS Files (*.xls);; XLSX Files(*.xlsx);");
    qDebug() << str;
    if (!str.isEmpty()) {
        emit havePath(str);
        progressValue = 0;
        progressBar->setValue(progressValue);
        progressBar->show();
        statusLabel->setText("Загружаем данные");
        ui->loadDataButton->setEnabled(false);
    }
}

void MainWindow::fillImage(int i, int cnt)
{
    m->lock();
    //statusLabel->setText("Загружаем картинки");
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    QLabel *label = new QLabel(this);
    label->setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
    label->setScaledContents(true);
    int row = shapes_count + i;
    if(row > 49) {
        ui->tableWidget->insertRow( ui->tableWidget->rowCount() );
    }
    ui->tableWidget->setCellWidget(row,2, label);

    onePercent = 50.0/static_cast<double>(cnt);
    progressValue += onePercent;
    progressBar->setValue(ceil(progressValue));
    dataDealed->wakeAll();
    m->unlock();
}

void MainWindow::fillCell(QString str, int i, int j)
{
    m->lock();
    //statusLabel->setText("Загружаем данные");
    int row = shapes_count + i;
    QTableWidgetItem *pCell = ui->tableWidget->item(row, j);
    if(!pCell)
    {
        pCell = new QTableWidgetItem;
        ui->tableWidget->setItem(row, j, pCell);
    }
    pCell->setText(str);

    if (j == 0) {
        progressValue += onePercent;
        progressBar->setValue(ceil(progressValue));
    }
    dataDealed->wakeAll();
    m->unlock();
}

void MainWindow::countProfit()/*on_profitButton_clicked()*/ {
    double mult = 1 + profitLineEdit->text().toDouble()/100;
    double exchange = exchangeLineEdit->text().toDouble();
    double total = 0;
    double markUpTotal = 0;
    for(int row = 0; row < shapes_count; ++row) {
        double priceOfOneProduct = ui->tableWidget->item(row, 6)->text().toDouble();
        double priceOfAllProducts = ui->tableWidget->item(row, 8)->text().toDouble();

        total += priceOfAllProducts*exchange;
        priceOfOneProduct = std::round(priceOfOneProduct*exchange*10)/10;
        QString str = QString::number(priceOfOneProduct);
            QTableWidgetItem *pCell = ui->tableWidget->item(row, 7);
            if(!pCell)
            {
                pCell = new QTableWidgetItem;
                ui->tableWidget->setItem(row, 7, pCell);
            }
            pCell->setText(str);

            priceOfOneProduct = std::round(priceOfOneProduct*mult*10)/10;
            str = QString::number(priceOfOneProduct);
            pCell = ui->tableWidget->item(row, 9);
            if(!pCell)
            {
                pCell = new QTableWidgetItem;
                ui->tableWidget->setItem(row, 9, pCell);
            }
            pCell->setText(str);
    }
    totalCostLabelValue->setText(QString::number(total));
    markUpTotalCostLabelValue->setText(QString::number(total*mult));
    revenueLabelValue->setText(QString::number(total*mult - goodsLineEdit->text().toDouble()
                                               - shippingLineEdit->text().toDouble()));

}

void MainWindow::startWorkInAThread(QString path)
{
    FillTable *filler = new FillTable(nullptr, path, m, dataDealed);
    connect(filler, &FillTable::shapeReady, this, &MainWindow::fillImage);
    connect(filler, &FillTable::cellReady, this, &MainWindow::fillCell);
    connect(filler, &FillTable::tableFillFinished, this, &MainWindow::statusFinished);
    // connect(filler, &FillTable::tableFillStarted, this, &MainWindow::onTableFillStarted);
    connect(filler, &FillTable::finished, filler, &FillTable::deleteLater);
    filler->start();
}

void MainWindow::statusFinished(int cnt)
{
    statusLabel->setText("Таблица успешно загружена");
    progressBar->hide();
    shapes_count += cnt;
    ui->loadDataButton->setEnabled(true);
}


