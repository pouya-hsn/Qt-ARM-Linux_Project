#include "power.h"
#include "ui_power.h"
#include "home.h"
#include "command.h"
#include "status.h"
#include "cooler.h"
#include "temp.h"
#include "mobinstatus.h"
#include "mobsatstatus.h"
#include "levelingstatus.h"
#include "pckthandler.h"
#include <QDebug>
#include "mobin.h"
#include "mobsat.h"
#include "leveling.h"
#include "gpio.h"
#include "inv.h"
#include "ups.h"
#include "genfunc.h"
#include <QTimer>

extern UPS *upsObj;
extern genFunc *gen;
extern pckthandler *handler ;
extern levelingstatus *lvlStatusObj;
extern mobsatstatus *msatStatusObj ;
extern mobinstatus *mobStatusObj;
extern cooler *coolObj ;
extern status *statusObj;
extern command *cmdObj ;
extern home *homeObj;
extern temp *tmpHmdObj ;
extern Leveling *lvlObj ;
extern Mobin *mbnObj ;
extern Mobsat *mbsObj ;
extern inv *invObj ;

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

power::power(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::power)
{
    ui->setupUi(this);
    gpioTim = new QTimer();
    gpioTim->start(50);
    connect(gpioTim, &QTimer::timeout, this, &power::checkGPIO);
    setStatus();
    connect(handler, &pckthandler::pwrChanged, this, &power::setStatus);
}

power::~power()
{
    delete ui;
}

void power::on_bckBtn_clicked()
{
    homeObj = new home();
    homeObj->show();
    this->close();
    gpioTim->stop();
}

void power::on_cmmndBtn_clicked()
{
    cmdObj = new command() ;
    cmdObj->show();
    this->close();
    gpioTim->stop();
}

void power::on_sttsBtn_clicked()
{
    statusObj = new status();
    statusObj->show();
    this->close();
    gpioTim->stop();
}

void power::on_clrBtn_clicked()
{
    coolObj = new cooler();
    coolObj->show();
    this->close();
    gpioTim->stop();
}

void power::on_tempClrBtn_clicked()
{
    tmpHmdObj = new temp();
    tmpHmdObj->show();
    this->close();
    gpioTim->stop();
}

void power::on_mbnBtn_clicked()
{
    mbnObj = new Mobin() ;
    mbnObj->show();
    this->close();
    gpioTim->stop();
}

void power::on_mbstBtn_clicked()
{
    mbsObj = new Mobsat() ;
    mbsObj->show();
    this->close();
    gpioTim->stop();
}

void power::on_lvlingBtn_clicked()
{
    lvlObj = new Leveling();
    lvlObj->show();
    this->close();
    gpioTim->stop();
}

void power::checkGPIO()
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

void power::on_invDetailBtn_clicked()
{
    invObj = new inv();
    invObj->show();
    this->close();
    gpioTim->stop();
}

void power::setStatus()
{
    gen->defState(ui->upsSttsBtn,(handler->powBytes[0] & 0x04),QSize(21,21));
    gen->defState(ui->directSttsBtn, (handler->powBytes[0] & 0x08), QSize(21,21));
    gen->defState(ui->inverterSttsBtn, (handler->powBytes[0] & 0x10), QSize(21,21));
    gen->defState(ui->offSttsBtn, (handler->powBytes[0] & 0x20), QSize(21,21));
    gen->defState(ui->UrbanElecGenSttsBtn, (handler->powBytes[0] & 0x40), QSize(21,21));

    if(handler->powBytes[0] & 0x80)
    {gen->setanIcon(":/carOnSttsBtn.png",ui->carSttsBtn);
     ui->carSttsLbl->setText("Car is On");}
    else
    {gen->setanIcon(":/carOffSttsBtn.png",ui->carSttsBtn);
     ui->carSttsLbl->setText("Car is Off");}
}

void power::on_upsDetailBtn_clicked()
{
    upsObj = new UPS() ;
    upsObj->show();
    this->close();
    gpioTim->stop();
}
