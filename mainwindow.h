#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QStandardItemModel>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QStandardItemModel *model;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void browseFile();

private slots:
    void on_fileSelectButton_clicked();

    void on_loadSort_clicked();

    void on_convertToJson_clicked();

    void on_database_clicked();

    void on_tableView_viewportEntered();

private:
    Ui::MainWindow *ui;
    QString path;
};
#endif // MAINWINDOW_H
