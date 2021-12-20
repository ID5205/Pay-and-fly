#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QAbstractScrollArea>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(10); //строки
    ui->tableWidget->setColumnCount(4);//столбцы

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Model" << "Place" << "ID" << "Status");
    QString styleSheet = "::section {" // "QHeaderView::section {"
                         "spacing: 10px;"
                         "background-color: #8F8F8F;"
                         "color: white;"
                         "border: 1px solid #8F8F8F;"
                         "margin: 1px;"
                         "text-align: right;"
                         "font-family: arial;"
                         "font-size: 12px; }";
    QString styleSheet1 = "::section {" // "QHeaderView::section {"
            "background-color: #2b2b2b;"
            "color: white;"
            "text-align: right;"
            "font-family: arial;"
            "font-size: 12px; }";

     ui->tableWidget->horizontalHeader()->setStyleSheet(styleSheet);
     ui->tableWidget->verticalHeader()->setStyleSheet(styleSheet1);

     ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
     ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
     ui->tableWidget->resizeColumnsToContents();
      ui->tableWidget->setStyleSheet("QTableCornerButton::section {background-color:#8F8F8F}");


    ui->listWidget->addItem("Airplane");
    ui->listWidget->addItem("Sooooo");
   // ui->listWidget->currentItem()->text();//текущий элемент



    int num =0; //число, элемент ячейки
    for(int i=0; i< ui->tableWidget->rowCount(); i++)
        for (int j=0; j<ui->tableWidget->columnCount(); j++)
        {
            QTableWidgetItem *itm = new QTableWidgetItem(tr("%1").arg(num)); //создание ячейки + преобразование числа в строку
            ui->tableWidget-> setItem(i,j,itm); //помещаем в таблицу элемент
            itm->setBackgroundColor(QColor(QString::fromUtf8("#444141")));
            itm->setTextColor(Qt::white);
        }

     ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()//клик на ячейку
{

    QTableWidgetItem *itm = ui->tableWidget->currentItem(); //текущий элемент ячейки
    QMessageBox::information(this, "Info", itm->text()); //инфа об ячейке
}

