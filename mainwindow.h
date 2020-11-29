#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bluetoothproxy.h"
#include "runthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(BluetoothProxy *_blue_window,QWidget *parent = nullptr);
    ~MainWindow();
    BluetoothProxy *blue_window;
private slots:
    void on_pushButton_blue_clicked();


    void on_horizontalSlider_c0_valueChanged(int value);

    void on_horizontalSlider_c1_valueChanged(int value);

    void on_horizontalSlider_c2_valueChanged(int value);

    void on_button_left_clicked();

    void on_button_rigtht_clicked();

    void on_horizontalSlider_armx_valueChanged(int value);

    void on_horizontalSlider_army_valueChanged(int value);

    void on_horizontalSlider_armz_valueChanged(int value);


private:
    Ui::MainWindow *ui;
    RunThread* run_thread;
    bool choose_l_r;

    void setTextXYZ();
    void setTextC123();
};
#endif // MAINWINDOW_H
