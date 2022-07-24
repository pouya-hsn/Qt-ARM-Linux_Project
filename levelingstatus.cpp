#include "levelingstatus.h"
#include "ui_levelingstatus.h"
#include "home.h"
#include "command.h"
#include "power.h"
#include "cooler.h"
#include "temp.h"
#include "status.h"
#include "mobinstatus.h"
#include "mobsatstatus.h"
#include "pckthandler.h"
#include "mobin.h"
#include "mobsat.h"
#include "leveling.h"
#include "gpio.h"
#include "genfunc.h"
#include <QTimer>

extern genFunc *gen;
extern pckthandler *handler;
extern mobsatstatus *msatStatusObj ;
extern mobinstatus *mobStatusObj;
extern status *statusObj;
extern cooler *coolObj;
extern power *pwrObj;
extern command *cmdObj ;
extern temp *tmpHmdObj;
extern Leveling *lvlObj ;
extern Mobin *mbnObj ;
extern Mobsat *mbsObj ;

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

levelingstatus::levelingstatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::levelingstatus)
{
    ui->setupUi(this);
    gpioTim = new QTimer();
    connect(gpioTim, &QTimer::timeout, this, &levelingstatus::checkGPIO);
    gpioTim->start(50);

    setStatusBytes() ;
    connect(handler, &pckthandler::sttsChanged, this, &levelingstatus::setStatusBytes) ;
}

levelingstatus::~levelingstatus()
{
    delete ui;
}

void levelingstatus::on_cmmndBtn_clicked()
{
    cmdObj = new command();
    cmdObj->show();
    this->close();
    gpioTim->stop();
}

void levelingstatus::on_pwrBtn_clicked()
{
    pwrObj = new power();
    pwrObj->show();
    this->close();
    gpioTim->stop();
}

void levelingstatus::on_clrBtn_clicked()
{
    coolObj = new cooler();
    coolObj->show();
    this->close();
    gpioTim->stop();
}

void levelingstatus::on_tempClrBtn_clicked()
{
    tmpHmdObj = new temp();
    tmpHmdObj->show();
    this->close();
    gpioTim->stop();
}

void levelingstatus::on_bckBtn_clicked()
{
    statusObj = new status();
    statusObj->show();
    this->close();
    gpioTim->stop();
}

void levelingstatus::on_mbnBtn_clicked()
{
    mbnObj = new Mobin() ;
    mbnObj->show();
    this->close();
    gpioTim->stop();
}

void levelingstatus::on_mbstBtn_clicked()
{
    mbsObj = new Mobsat() ;
    mbsObj->show();
    this->close();
    gpioTim->stop();
}

void levelingstatus::on_lvlingBtn_clicked()
{
    lvlObj = new Leveling();
    lvlObj->show();
    this->close();
    gpioTim->stop();
}

void levelingstatus::setStatusBytes()
{
    gen->defState(ui->jck1PrxyBtn,(0x80 &(handler->stBytes[4])),QSize(22,24));
    gen->defState(ui->jck2PrxyBtn,(0x40 &(handler->stBytes[4])),QSize(22,24));
    gen->defState(ui->jck3PrxyBtn,(0x20 &(handler->stBytes[4])),QSize(22,24));
    gen->defState(ui->jck4PrxyBtn,(0x10 &(handler->stBytes[4])),QSize(22,24));
    gen->defState(ui->pwrMotBtn,(0x40 &(handler->stBytes[5])),QSize(22,24));
    gen->defState(ui->hiSpdValBtn,(0x10 &(handler->stBytes[5])),QSize(22,24));
    gen->defState(ui->loSpdValBtn,(0x08 &(handler->stBytes[5])),QSize(22,24));
    gen->defState(ui->disValBtn,(0x20 &(handler->stBytes[5])),QSize(22,24));
    gen->defState(ui->prsSw1Btn,(0x08 &(handler->stBytes[4])),QSize(22,24));
    gen->defState(ui->prsSw2Btn,(0x04 &(handler->stBytes[4])),QSize(22,24));
    gen->defState(ui->prsSw3Btn,(0x02 &(handler->stBytes[4])),QSize(22,24));
    gen->defState(ui->prsSw4Btn,(0x01 &(handler->stBytes[4])),QSize(22,24));
    gen->defState(ui->mnPrsSwBtn,(0x80 &(handler->stBytes[5])),QSize(22,24));
}

void levelingstatus::checkGPIO()
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
