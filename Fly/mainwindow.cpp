#include "mainwindow.h"
#include "Manager.h"
#include "ui_mainwindow.h"
#include "map_painter.h"
#include<QMessageBox>
#include<QPixmap>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Fly");
    QPixmap logo (":/image/images/logo.png");
    this->setWindowIcon(logo);

    ui->tableWidget->setRowCount(2); //строки
    ui->tableWidget->setColumnCount(4);//столбцы
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Model" << "Place" << "ID" << "Status");
    QString styleSheet = "::section {" // "QHeaderView::section {"
            "spacing: 10px;"
            "background-color: #4f4f4f;"
            "color: white;"
            "border: 1px solid #383838;"
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
    ui->tableWidget->setVerticalScrollBar(ui->verticalScrollBar);
    ui->tableWidget->setFocusPolicy(Qt::TabFocus);


    connect(ui->tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(change_list_context(int )));

    int num =0; //число, элемент ячейки
    for(int i=0; i< ui->tableWidget->rowCount(); i++)

        for (int j=0; j<ui->tableWidget->columnCount(); j++)
        {
            QTableWidgetItem *itm = new QTableWidgetItem(tr("%1").arg(num)); //создание ячейки + преобразование числа в строку
            ui->tableWidget-> setItem(i,j,itm); //помещаем в таблицу элемент
            itm->setBackgroundColor(QColor(QString::fromUtf8("#454545")));
            itm->setTextColor(Qt::white);
            ui->tableWidget->row(itm);
        }
    for(int i=0; i< ui->tableWidget->rowCount(); i+=2)
        for (int j=0; j<ui->tableWidget->columnCount(); j++)
            ui->tableWidget->item(i, j)->setBackground(QColor(QString::fromUtf8("#383838")));

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setVisible(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_menager(Manager* m)
{
    manager = m;
}
void MainWindow::set_painter(map_painter* p)
{
    painter = p;
}
void MainWindow::on_tableWidget_cellPressed(int row, int column)
{

}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (!ui->tableWidget->itemAt(event->pos()))
    {
        ui->tableWidget->clearSelection();
        ui->listWidget->clear();
        painter->set_track(0);
    }
}

void MainWindow::change_list_context(int row)//клик на ячейку
{
    painter->set_track((manager->fill_field(*this, row))->get_track());
}
QListWidget* MainWindow::get_list()
{
    return ui->listWidget;
}
QTableWidget* MainWindow::get_table()
{
    return ui->tableWidget;
}
QWidget* MainWindow::get_GL()
{
    return ui->wigetGL;
}
int MainWindow::get_GL_w()
{
    return ui->wigetGL->width();
}
int MainWindow::get_GL_h()
{
    return ui->wigetGL->height();
}
