#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "poly.h"
#include "polytable.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tables->removeTab(0);
    ui->tables->removeTab(0);

}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::on_addPoly_clicked()
{
    ui->tables->addTab(new PolyTable(ui->tables), QString("Polynomial %1").arg(ui->tables->count() + 1));
}



void MainWindow::on_tables_tabCloseRequested(int index)
{
    ui->tables->removeTab(index);
}

