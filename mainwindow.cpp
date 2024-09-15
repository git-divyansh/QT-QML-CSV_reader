#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include<QMessageBox>
#include "csvfiles.h"

CSVfiles obj;
QSqlDatabase db;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("E:/C++/QT/Test/test_db");

    if (!db.open()) {
        qDebug() << "Failed to connect to SQLite database.";

    }

    qDebug() << "Successfully connected to SQLite database!";
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_fileSelectButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/",
                                                    tr("Comma Separated Files (*.csv);;All Files (*.*)"));
    this->path = fileName;
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        QString onlyFileName = fileInfo.fileName();
        ui->AfterFileChooseLabel->setText(onlyFileName);
        QMessageBox::information(this, tr("File Selected"), tr("You selected: %1").arg(fileName));
    } else {
        // If no file is selected, inform the user
        QMessageBox::warning(this, tr("No File Selected"), tr("No file was selected."));
    }
}




void MainWindow::on_loadSort_clicked()
{
    if(!this->path.length()){
        qDebug() << "CSV file is not loaded";
        return;
    }
    obj.read(this->path);
    obj.showInTableView(this->model);
}


void MainWindow::on_convertToJson_clicked()
{
    obj.convertToJson();
}


void MainWindow::on_database_clicked()
{
    if(!obj.createTable(db)){
        qDebug() << "Problem occured";
    }
}




void MainWindow::on_tableView_viewportEntered()
{
    ui->tableView->setModel(this->model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

