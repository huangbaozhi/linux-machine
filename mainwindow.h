#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//显示时间
#include <QTimer>
#include <QMessageBox>
#include <QPainter>
#include <QTime>
#include <QApplication>

#include <QTimer>
extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <asm/types.h>
#include <linux/fb.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include<string.h>
#include<unistd.h>
#include<sys/mman.h>
#include <sys/poll.h>
}

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //QTimer *qtimer;

private:
    Ui::MainWindow *ui;
    int beep_fd;
    int led_fd;
    int adc_fd;
    int relay_fd;
    int pwm_fd;

    QTimer *timer;//定义时间

private slots:

    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void RGBlight();

    void VoltageShow();//电压显示

    //时间
    void timerUpdate();//当前时间
    void entryTime();//进入时间
    void leaveTime();//离开时间
    void jishi();


    //void OnOff();




};

#endif // MAINWINDOW_H
