#include "inv.h"
#include "ui_inv.h"
#include "power.h"
#include "gpio.h"
#include "command.h"
#include "status.h"
#include "mobin.h"
#include "mobsat.h"
#include "leveling.h"
#include "cooler.h"
#include "temp.h"
#include "pckthandler.h"
#include "genfunc.h"
#include <QTimer>

extern genFunc *gen ;
extern pckthandler *handler;
extern temp *tmpHmdObj;
extern cooler *coolObj;
extern Leveling *lvlObj ;
extern Mobsat *mbsObj;
extern Mobin *mbnObj;
extern status *statusObj;
extern command *cmdObj;
extern QTimer *gpioTim;
extern power *pwrObj ;
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

inv::inv(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inv)
{
    ui->setupUi(this);
    gpioTim = new QTimer();
    connect(gpioTim,&QTimer::timeout, this, &inv::checkGPIO);
    gpioTim->start(50);

    setStatus();
    connect(handler, &pckthandler::pwrChanged, this, &inv::setStatus);

}

inv::~inv()
{
    delete ui;
}

void inv::on_backBtn_clicked()
{
    pwrObj = new power();
    pwrObj->show();
    this->close();
    gpioTim->stop();
}

void inv::on_cmdBtn_clicked()
{
    cmdObj = new command();
    cmdObj->show();
    this->close();
    gpioTim->stop();
}

void inv::on_sttsBtn_clicked()
{
   statusObj = new status() ;
   statusObj->show();
   this->close();
   gpioTim->stop();
}

void inv::on_mbnBtn_clicked()
{
    mbnObj = new Mobin() ;
    mbnObj->show();
    this->close();
    gpioTim->stop();
}

void inv::on_mbsBtn_clicked()
{
    mbsObj = new Mobsat() ;
    mbsObj->show() ;
    this->close();
    gpioTim->stop();
}

void inv::on_lvlBtn_clicked()
{
    lvlObj = new Leveling() ;
    lvlObj->show() ;
    this->close();
    gpioTim->stop();
}

void inv::on_coolerBtn_clicked()
{
    coolObj = new cooler() ;
    coolObj->show();
    this->close();
    gpioTim->stop();
}

void inv::on_tmpHmdBtn_clicked()
{
    tmpHmdObj = new temp() ;
    tmpHmdObj->show() ;
    this->close();
    gpioTim->stop();
}

void inv::checkGPIO()
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

void inv::setStatus()
{
    int freq = int(handler->powBytes[5]);
    ui->freqValLbl->setText(QString::number(freq)) ;

    int input_v = int(handler->powBytes[8]);
    ui->vinValLbl->setText(QString::number(input_v));

    int out_v = int(handler->powBytes[9]) ;
    ui->voutValLbl->setText(QString::number(out_v)) ;

    int bat_ch = int(handler->powBytes[10]) ;
    ui->batValLbl->setText(QString::number(bat_ch));

    int temp = int(handler->powBytes[11]);
    ui->tmpValLbl->setText(QString::number(temp)+ " °C") ;

    gen->defState(ui->pwrOnSttsBtn,(handler->powBytes[12] & 0x10), QSize(20,20));
    gen->defState(ui->pwrOffSttsBtn, (handler->powBytes[12] & 0x20), QSize(20,20)) ;
    gen->defState(ui->batLowSttsBtn, (handler->powBytes[12] & 0x40), QSize(20,20)) ;
    gen->defState(ui->batUsedUpSttsBtn, (handler->powBytes[12] & 0x80), QSize(20,20)) ;
}
