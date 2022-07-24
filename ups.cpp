#include "ups.h"
#include "ui_ups.h"
#include "power.h"
#include "gpio.h"
#include "command.h"
#include "status.h"
#include "mobin.h"
#import "mobsat.h"
#include "leveling.h"
#include "cooler.h"
#include "temp.h"
#include "genfunc.h"
#include "pckthandler.h"
#include <QTimer>

extern genFunc *gen;
extern pckthandler *handler;
extern temp *tmpHmdObj;
extern cooler *coolObj;
extern Leveling *lvlObj;
extern Mobsat *mbsObj;
extern Mobin *mbnObj;
extern status *statusObj;
extern command *cmdObj;
extern QTimer *gpioTim;
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
extern power *pwrObj;

UPS::UPS(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UPS)
{
    ui->setupUi(this);
    gpioTim = new QTimer();
    connect(gpioTim, &QTimer::timeout, this, &UPS::checkGPIO) ;
    setStatus();
    connect(handler, &pckthandler::pwrChanged, this, &UPS::setStatus);
}

UPS::~UPS()
{
    delete ui;
}

void UPS::on_backBtn_clicked()
{
    pwrObj = new power() ;
    pwrObj->show();
    this->close();
    gpioTim->stop();
}

void UPS::checkGPIO()
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

void UPS::on_cmdBtn_clicked()
{
    cmdObj = new command() ;
    cmdObj->show();
    this->close();
    gpioTim->stop();
}

void UPS::on_statusBtn_clicked()
{
   statusObj = new status() ;
   statusObj->show();
   this->close();
   gpioTim->stop();
}

void UPS::on_mbnBtn_clicked()
{
    mbnObj = new Mobin() ;
    mbnObj->show();
    this->close();
    gpioTim->stop();
}

void UPS::on_mbsBtn_clicked()
{
    mbsObj = new Mobsat() ;
    mbsObj->show();
    this->close();
    gpioTim->stop();
}

void UPS::on_lvlBtn_clicked()
{
    lvlObj = new Leveling() ;
    lvlObj->show();
    this->close();
    gpioTim->stop();
}

void UPS::on_coolerBtn_clicked()
{
    coolObj = new cooler() ;
    coolObj->show();
    this->close();
    gpioTim->stop();
}

void UPS::on_tmpHmdBtn_clicked()
{
    tmpHmdObj = new temp() ;
    tmpHmdObj->show();
    this->close();
    gpioTim->stop();
}

void UPS::setStatus()
{
//    int input_v = int(handler->powBytes[1]) ;
    ui->vinValLbl->setText(QString::number(handler->powBytes[1])) ;

//    int out_v = int(handler->powBytes[2]) ;
    ui->voValLbl->setText(QString::number(handler->powBytes[2])) ;

//    int load_per = int(handler->powBytes[3]);
    ui->loadValLbl->setText(QString::number(handler->powBytes[3])) ;

//    int temp = int(handler->powBytes[4]);
    ui->tmpValLbl->setText(QString::number(handler->powBytes[4]) + " °C") ;

//    int freq = int(handler->powBytes[5]);
    ui->freqUPSValLbl->setText(QString::number(handler->powBytes[5])) ;

//    int bat_v = int(handler->powBytes[6]);
    ui->batVoltValLbl->setText(QString::number(handler->powBytes[6])) ;

    gen->defState(ui->serConnSttsBtn,(handler->powBytes[7] & 0x01), QSize(20,20));
    gen->defState(ui->shutDwnSttsBtn,(handler->powBytes[7] & 0x02), QSize(20,20));
    gen->defState(ui->testPrgSttsBtn,(handler->powBytes[7] & 0x04), QSize(20,20));
    gen->defState(ui->upsFailSttsBtn,(handler->powBytes[7] & 0x08), QSize(20,20));
    gen->defState(ui->utilFailSttsBtn,(handler->powBytes[7] & 0x10), QSize(20,20));
    gen->defState(ui->bpssModeSttsBtn,(handler->powBytes[7] & 0x20), QSize(20,20));
    gen->defState(ui->batLowSttsBtn,(handler->powBytes[7] & 0x40), QSize(20,20));
    gen->defState(ui->powInDisSttsBtn,(handler->powBytes[7] & 0x80), QSize(20,20));

}
