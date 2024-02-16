#include "mainwindow.h"
#include "ui_mainwindow.h"

#define N 100
int sum=0,filtervalue=0,i,value=0,count;
float voltage=0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //通过QLabel控件来显示日期时间
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);

    beep_fd = ::open("/dev/hbzioctrldev", O_RDWR);
    led_fd = ::open("/dev/hbzLED", O_RDWR);
    relay_fd = ::open("/dev/ledmischbz", O_RDWR);
    adc_fd = ::open("/dev/adchbz", 0);
    pwm_fd = ::open("/dev/PWMhbz", O_RDWR);
    if(led_fd<0)
    {
        printf("led wrong\n");
    }
    if (beep_fd < 0)
    {
        printf("beep wrong\n");
        return;
    }
    if(adc_fd<0)
    {
        printf("adc wrong\n");
    }
    if (relay_fd < 0)
    {
        printf("relay wrong\n");
        return;
    }
    if (pwm_fd < 0)
    {
        printf("pwm wrong\n");
        return;
    }


//    qtimer=new QTimer;
//    this->connect(qtimer,SIGNAL(timeout()),this,SLOT(OnOff()));
//    qtimer->start(1000);

//    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(Relay()));//打开继电器
//    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(stopRelay()));//关闭继电器

    VoltageShow();
    on_pushButton_3_clicked();//打开继电器


}

MainWindow::~MainWindow()
{
    delete ui;
}

//打开继电器
void MainWindow::on_pushButton_3_clicked()
{
    entryTime();//进入时间

     ::ioctl(beep_fd,1,0);
  ::ioctl(relay_fd,1, 0);

  char buf[128];
  ::ioctl(relay_fd,0, 0);
  printf("write the 0x20 to the Port X5!\n");//蓝灯
  buf[0]=0x20;
  :: write (led_fd, buf, 1);
  sleep(1);
  sleep (5);
  on_pushButton_4_clicked();//关闭继电器


}

//关闭继电器
void MainWindow::on_pushButton_4_clicked()
{
        ::ioctl(beep_fd,0,0);
    ::ioctl(relay_fd,0, 0);

    sleep(5);
    on_pushButton_clicked();//打开电机

}

//打开电机
void MainWindow::on_pushButton_clicked()
{
    ::ioctl(pwm_fd,'a', 500);

    char buf[128];
    ::ioctl(relay_fd,0, 0);

    printf("write the 0x10 to the Port X4!\n");//绿灯
    buf[0]=0x10;
    ::write (led_fd, buf, 1);
    sleep (1);
    sleep(5);
    on_pushButton_2_clicked();

}
//关闭电机
void MainWindow::on_pushButton_2_clicked()
{
    ::ioctl(pwm_fd,'b', 0);

    char buf[128];
    ::ioctl(relay_fd,0, 0);
    printf("write the 0x08 to the Port X3!\n");//红灯
    buf[0]=0x08;
    ::write (led_fd, buf, 1);
    sleep (1);

    leaveTime();

}

//电压显示
void MainWindow::VoltageShow()
{
    char buffer[100];
    int value_buf[100];
    for(i=0;i<100;i++){
    int len = read(adc_fd, buffer, sizeof(buffer));//看驱动最后返回的是2个字节
    if (len > 0) {
            buffer[len] = '\0';
            sscanf(buffer, "%d", &value);// 从字符串读取格式化输入
            value_buf[i]=value;
    } else {
            perror("read ADC device:");

    }
    usleep(100* 100);
    }
    for(count=0;count<100;count++)
    sum = sum + value_buf[count];
    filtervalue = sum/100;
    voltage = 1.8/4096*filtervalue;
    ui->label_2->setText(QString::number(voltage));
    sum=0;filtervalue=0;
}

//当前时间
void MainWindow::timerUpdate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("hh:mm:ss  dddd");
    ui->dateTime->setText(str);
}

//进入时间
void MainWindow::entryTime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str2 = time.toString("hh:mm:ss");
    ui->label_4->setText(str2);
}

//离开时间
void MainWindow::leaveTime()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str3 = time.toString("hh:mm:ss");
    ui->label_5->setText(str3);

    jishi();

}

//计时
void MainWindow::jishi()
{
    QString stime=ui->label_4->text();
    QDateTime starttime=QDateTime::fromString(stime,"hh:mm:ss");
    QString etime=ui->label_5->text();
    QDateTime endtime=QDateTime::fromString(etime,"hh:mm:ss");
    uint end=endtime.toTime_t();//将时间转为时间戳
    uint start=starttime.toTime_t();
    int second=(end-start)*2;
    QString ti=QString::number(second,10);
    ui->label_8->setText(ti);
}

