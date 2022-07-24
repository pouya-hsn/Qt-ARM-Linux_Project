#include "cooler.h"
#include "ui_cooler.h"
#include "home.h"
#include "command.h"
#include "status.h"
#include "power.h"
#include "temp.h"
#include "mobinstatus.h"
#include "mobsatstatus.h"
#include "levelingstatus.h"
#include "pckthandler.h"
#include "mobin.h"
#include "mobsat.h"
#include "leveling.h"
#include <QTimer>
#include "gpio.h"

extern QTimer *gpioTim;
extern pckthandler *handler ;
extern levelingstatus *lvlStatusObj;
extern mobsatstatus *msatStatusObj ;
extern mobinstatus *mobStatusObj;
extern home *homeObj;
extern cooler *coolObj;
extern command *cmdObj ;
extern status *statusObj ;
extern power *pwrObj ;
extern temp *tmpHmdObj;
extern Leveling *lvlObj ;
extern Mobin *mbnObj ;
extern Mobsat *mbsObj ;

extern gpio gp1;
extern gpio gp2;
extern gpio gp3;
extern gpio gp4;
extern gpio gp5;
extern gpio gp6;
extern gpio gp7;
extern gpio gp8;
extern gpio gp9;
extern gpio gp10;

extern bool kab1State ;
extern bool kab2State ;

cooler::cooler(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cooler)
{
    ui->setupUi(this);
    gpioTim = new QTimer() ;
    connect(gpioTim, &QTimer::timeout, this, &cooler::checkGPIO);
    gpioTim->start(50);

    QFont font;
    font.setBold(true);
    font.setPointSize(12);
    font.setFamily("Cambria");
    ui->kab1TmpVal->setFont(font);
    ui->kab2TmpVal->setFont(font);
    showTemp();
    connect(handler, &pckthandler::tmpHmdChanged, this, &cooler::showTemp);


    if(kab1State)
        onButton(ui->kab1Btn);
    else
        offButton(ui->kab1Btn);

    if(kab2State)
        onButton(ui->kab2Btn);
    else
        offButton(ui->kab2Btn);
}

cooler::~cooler()
{
    delete ui;
}

void cooler::on_bckBtn_clicked()
{
    homeObj = new home();
    homeObj->show();
    coolObj->close();
    gpioTim->stop();
}

void cooler::on_cmmndBtn_clicked()
{
    cmdObj = new command();
    cmdObj->show();
    coolObj->close();
    gpioTim->stop();
}


void cooler::on_sttsBtn_clicked()
{
    statusObj = new status() ;
    coolObj->close();
    statusObj->show();
    gpioTim->stop();
}

void cooler::on_pwrBtn_clicked()
{
    pwrObj = new power();
    coolObj->close();
    pwrObj->show();
    gpioTim->stop();
}

void cooler::on_tempClrBtn_clicked()
{
    tmpHmdObj = new temp();
    coolObj->close();
    tmpHmdObj->show();
    gpioTim->stop();
}

void cooler::on_mbnBtn_clicked()
{
    mbnObj = new Mobin() ;
    this->close();
    mbnObj->show();
    gpioTim->stop();
}

void cooler::on_mbstBtn_clicked()
{
    mbsObj = new Mobsat() ;
    mbsObj->show();
    mbnObj->close();
    gpioTim->stop();
}

void cooler::on_lvlingBtn_clicked()
{
    lvlObj = new Leveling();
    lvlObj->show();
    mbnObj->close();
    gpioTim->stop();
}

QByteArray cooler::pcktSend(unsigned char data,unsigned char byteNum)
{
    QByteArray pckt ;
    pckt.append(coolHeader);
    for(unsigned char i = 2 ; i < 4 ; i++)
    {
        if(i==byteNum)
            pckt.append(data);
        else
            pckt.append(char(0x00));
    }
    return pckt;
}

void cooler::on_kab1Btn_clicked()
{
    if(!kab1State)
    {
        handler->serialSend(pcktSend(0xA6,2));
        onButton(ui->kab1Btn);
        kab1State = true ;
    }
    else
    {
        handler->serialSend(pcktSend(0xB6,2));
        offButton(ui->kab1Btn);
        kab1State = false ;
    }
    emit kab1CoolerStatus();
}


void cooler::on_kab2Btn_clicked()
{
    if(!kab2State)
    {
        handler->serialSend(pcktSend(0xA6,3));
        onButton(ui->kab2Btn);
        kab2State = true ;

    }
    else
    {
        handler->serialSend(pcktSend(0xB6,3));
        offButton(ui->kab2Btn);
        kab2State = false ;
    }
    emit kab2CoolerStatus();
}
void cooler::showTemp()
{
    ui->kab1TmpVal->setText(QString::number(handler->tmpHmdBytes[5])+" °C");
    ui->kab2TmpVal->setText(QString::number(handler->tmpHmdBytes[6])+" °C");
}
void cooler::onButton(QPushButton *p)
{
    QPixmap pic(":/onBtn.png");
    QIcon icon(":/onBtn.png");
    p->setIcon(icon) ;
    p->setIconSize(QSize(pic.width(), pic.height()));
}

void cooler::offButton(QPushButton *p)
{
    QPixmap pic(":/offBtn.png");
    QIcon icon(":/offBtn.png");
    p->setIcon(icon);
    p->setIconSize(QSize(pic.width(), pic.height()));
}

void cooler::checkGPIO()
{
    if(!gp1.getState()) cmdObj->on_mbstStupBtn_clicked();
    if(!gp2.getState()) cmdObj->on_mbnStupBtn_clicked();
    if(!gp3.getState()) cmdObj->on_lvlingStupBtn_clicked();
    if(!gp4.getState()) cmdObj->on_nrmlStupModBtn_clicked() ;
    if(!gp5.getState()) cmdObj->on_stopBtn_clicked();
//    if(!gp6.getState()) cmdObj->on_dfctionModBtn_clicked();
    if(!gp7.getState()) cmdObj->on_nrmlRlifModBtn_clicked();
    if(!gp8.getState()) cmdObj->on_lvlingRlifBtn_clicked();
    if(!gp9.getState()) cmdObj->on_mbnRlifBtn_clicked();
    if(!gp10.getState()) cmdObj->on_mbstRlifBtn_clicked() ;
}
