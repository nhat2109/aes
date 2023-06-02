#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QAxObject>
#include <QFileInfo>
#include <QAxWidget>

#include <iostream>
#include <vector>
#include <cstring>
#include <random>
#include <cmath>
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //tab1
    void on_buttonTaoKhoa_clicked();
    void on_buttonExit_clicked();

    //tab2
    void on_taiVB_2_clicked();
    void on_maHoa_clicked();
    void on_luuChuKy_clicked();
    void on_buttonExit_2_clicked();

    //tab3
    void on_taiVB_3_clicked();
    void on_xacNhan_clicked();
    void on_Chuyen_clicked();



private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
