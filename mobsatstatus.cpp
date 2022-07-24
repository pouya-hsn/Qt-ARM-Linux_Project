#include "mobsatstatus.h"
#include "ui_mobsatstatus.h"
#include "home.h"
#include "status.h"
#include "command.h"
#include "power.h"
#include "cooler.h"
#include "temp.h"
#include "mobinstatus.h"
#include "levelingstatus.h"
#include <QDebug>
#include "pckthandler.h"
#include "mobin.h"
#include "mobsat.h"
#include "leveling.h"
#include "gpio.h"
#include "genfunc.h"
#include <QTimer>

extern genFunc *gen;
extern pckthandler *handler ;
extern levelingstatus *lvlStatusObj;
extern mobinstatus *mobStatusObj;
extern power *pwrObj ;
extern status *statusObj;
extern command *cmdObj ;
extern cooler *coolObj ;
extern temp *tmpHmdObj ;
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

mobsatstatus::mobsatstatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mobsatstatus)
{
    ui->setupUi(this);
    gen = new genFunc() ;
    gpioTim = new QTimer();
    gpioTim->start(50);
    connect(gpioTim, &QTimer::timeout, this, &mobsatstatus::checkGPIO);

    defStateBytes();
    connect(handler, &pckthandler::sttsChanged, this, &mobsatstatus::defStateBytes);
}

mobsatstatus::~mobsatstatus()
{
    delete ui;
}

void mobsatstatus::on_cmmndBtn_clicked()
{
    cmdObj = new command() ;
    cmdObj->show();
    this->close();
    gpioTim->stop();
}

void mobsatstatus::on_pwrBtn_clicked()
{
    pwrObj = new power() ;
    pwrObj->show();
    this->close();
    gpioTim->stop();
}

void mobsatstatus::on_clrBtn_clicked()
{
    coolObj = new cooler() ;
    coolObj->show();
    this->close();
    gpioTim->stop();
}

void mobsatstatus::on_tempClrBtn_clicked()
{
    tmpHmdObj = new temp() ;
    tmpHmdObj->show();
    this->close();
    gpioTim->stop();
}

void mobsatstatus::on_bckBtn_clicked()
{
    statusObj = new status() ;
    statusObj->show();
    this->close();
    gpioTim->stop();
}

void mobsatstatus::on_mbnBtn_clicked()
{
    mbnObj = new Mobin() ;
    mbnObj->show();
    this->close();
    gpioTim->stop();
}

void mobsatstatus::on_lvlingBtn_clicked()
{
    lvlObj = new Leveling();
    lvlObj->show();
    this->close();
    gpioTim->stop();
}

void mobsatstatus::on_mbstBtn_clicked()
{
    mbsObj = new Mobsat() ;
    mbsObj->show();
    this->close();
    gpioTim->stop();
}

void mobsatstatus::defStateBytes()
{
    gen->defState(ui->clpPrxyPedBtn,(0x80 & (handler->stBytes[2])),QSize(20,20));
    gen->defState(ui->frtMicSwBtn, (0x40 & (handler->stBytes[2])),QSize(20,20));
    gen->defState(ui->rerMicSwBtn,(0x20 & (handler->stBytes[2])),QSize(20,20));
    gen->defState(ui->frtClpPrxyBtn,(0x10 & (handler->stBytes[2])),QSize(20,20));
    gen->defState(ui->rerClpPrxyBtn,(0x08 & (handler->stBytes[2])),QSize(20,20));
    gen->defState(ui->dr1MicSwBtn, (0x04 & (handler->stBytes[2])),QSize(20,20));
    gen->defState(ui->dr2MicSwBtn, (0x02 & (handler->stBytes[2])),QSize(20,20));
    gen->defState(ui->mbsMotBtn, (0x01 & handler->stBytes[2]),QSize(20,20));
    gen->defState(ui->dirPrxyBtn,(0x80 & (handler->stBytes[3])),QSize(20,20));
    gen->defState(ui->frtClpBtn,(0x40 & (handler->stBytes[3])),QSize(20,20));
    gen->defState(ui->rerclpBtn,(0x20 & handler->stBytes[3]),QSize(20,20));
    gen->defState(ui->pedClpBtn, (0x10 & (handler->stBytes[3])),QSize(20,20));
    gen->defState(ui->pedParkSttsMbsBtn, (0x08 & (handler->stBytes[3])),QSize(20,20)) ;
    gen->defState(ui->pedPwrSttsMbsBtn, (0x04 & (handler->stBytes[3])),QSize(20,20)) ;
}

void mobsatstatus::checkGPIO()
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
