#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "redblack.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->on_pushButton_4_clicked();
    srand(time(0));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_4_clicked()
{
    ui->canvasWidget->setMargins(ui->levelMarginSpinBox->value(),
                                 ui->sibMarginSpinBox->value(),
                                 ui->marginSpinBox->value(),
                                 ui->leftSpinBox->value());
    ui->canvasWidget->setRadius(ui->radiusSpinBox->value());
    ui->canvasWidget->repaint();
}

void MainWindow::on_pushButton_clicked()
{
    ui->canvasWidget->setIsFindRst(false);
    ui->canvasWidget->rb.insert(ui->dataLineEdit->text().toInt());
    ui->canvasWidget->repaint();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!ui->canvasWidget->rb.remove(ui->dataLineEdit->text().toInt()))
    {
        QMessageBox::information(this, "提示", "删除失败，元素不存在！", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    ui->canvasWidget->setIsFindRst(false);
    ui->canvasWidget->repaint();
}

void MainWindow::on_pushButton_3_clicked()
{
    BinNodePosi(int) rst = ui->canvasWidget->rb.search(ui->dataLineEdit->text().toInt());
    if(rst)
    {
        ui->canvasWidget->setIsFindRst(true);
        ui->canvasWidget->setFindRst(rst);
        ui->canvasWidget->repaint();
    }
    else
        QMessageBox::information(this, "提示", "元素不存在！", QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_exitAction_triggered()
{
    this->close();
}

void MainWindow::on_randomAction_triggered()
{
    int n = rand() % 5;
    for(int i = 0; i < n; ++i)
        ui->canvasWidget->rb.insert(rand() % 100);
    ui->canvasWidget->repaint();
}
