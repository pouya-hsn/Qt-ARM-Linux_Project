#include "mobsat.h"
#include "ui_mobsat.h"
#include "home.h"
#include "pckthandler.h"
#include "command.h"
#include "status.h"
#include "mobin.h"
#include "power.h"
#include "cooler.h"
#include "temp.h"
#include "leveling.h"
#include "gpio.h"
#include <QTimer>

extern home *homeObj;
extern pckthandler *handler ;
extern command *cmdObj ;
extern QString mbsTxt ;
extern status *statusObj ;
extern Mobin *mbnObj;
extern power *pwrObj ;
extern cooler *coolObj;
extern temp *tmpHmdObj ;
extern Leveling *lvlObj ;

extern QString lastStMbs;

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

Mobsat::Mobsat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mobsat)
{
    ui->setupUi(this);
    ui->mbsTxtEdit->setTextColor(QColor(70,70,70));
    gpioTim = new QTimer();
    connect(gpioTim, &QTimer::timeout, this, &Mobsat::checkGPIO);
    gpioTim->start(50);

    ui->mbsTxtEdit->setText(mbsTxt);
    connect(handler,&pckthandler::mbsChanged, this, &Mobsat::checkMobsatSetup);
}

Mobsat::~Mobsat()
{
    delete ui;
}

void Mobsat::on_mbsBackBtn_clicked()
{
    homeObj = new home();
    homeObj->show();
    this->close();
    gpioTim->stop();
}


void Mobsat::on_mbsCmdBtn_clicked()
{
    cmdObj = new command();
    cmdObj->show();
    this->close();
    gpioTim->stop();
}

void Mobsat::on_mbsSttsBtn_clicked()
{
    statusObj = new status();
    statusObj->show();
    this->close();
    gpioTim->stop();
}

void Mobsat::on_mbsMbnBtn_clicked()
{
    mbnObj = new Mobin();
    mbnObj->show();
    this->close();
    gpioTim->stop();
}

void Mobsat::on_mbsPwrBtn_clicked()
{
    pwrObj = new power();
    pwrObj->show();
    this->close();
    gpioTim->stop();
}

void Mobsat::on_mbsClrBtn_clicked()
{
    coolObj = new cooler();
    coolObj->show();
    this->close();
    gpioTim->stop();
}

void Mobsat::on_mbsTmpBtn_clicked()
{
    tmpHmdObj = new temp();
    tmpHmdObj->show();
    this->close();
    gpioTim->stop();
}

void Mobsat::on_mbsLvlBtn_clicked()
{
    lvlObj = new Leveling();
    lvlObj->show();
    this->close();
    gpioTim->stop();
}

void Mobsat::checkMobsatSetup()
{
    if(!ui->mbsTxtEdit->toPlainText().isEmpty())
    {
        QStringList st = ui->mbsTxtEdit->toPlainText().split('\n',QString::SkipEmptyParts) ;
        lastStMbs =  st.last() ;
    }
    //===================== Setup =======================//
    QString str = "MOBSAT PEDSTAL IS SETUPING..." ;
    if(((handler->mbsBytes[0]) & 0x80) && (lastStMbs != str))
        ui->mbsTxtEdit->append(str);


    str = "DOOR IS OPEN" ;
    if((handler->mbsBytes[0] & 0x40)&& (lastStMbs != str))
        ui->mbsTxtEdit->append(str);


    str = "FRONT AND REAR CLAMP ARE OPEN" ;
    if((handler->mbsBytes[0] & 0x20) && (lastStMbs != str))
        ui->mbsTxtEdit->append(str);

    str = "PEDSTAL LOCK IS OPEN" ;
    if((handler->mbsBytes[0] & 0x10) && (lastStMbs != str))
        ui->mbsTxtEdit->append(str);

    QString strtemp= "MOBSAT PEDSTAL IS COMMING BACK" ;
    if((handler->mbsBytes[0] & 0x08) && (lastStMbs != strtemp))
        ui->mbsTxtEdit->append(strtemp);

    str = "MOBSAT PEDSTAL IS IN OK POSITION" ;
    if((handler->mbsBytes[0] & 0x04) && (lastStMbs != str))
    {
        mbsTxt = ui->mbsTxtEdit->toPlainText();
        mbsTxt.remove(strtemp);
        mbsTxt.remove(mbsTxt.lastIndexOf("\n"),1);
        ui->mbsTxtEdit->setText(mbsTxt);
        ui->mbsTxtEdit->append(str);
    }

    str = "FRONT CLAMP IS CLOSE" ;
    if((handler->mbsBytes[0] & 0x02) && (lastStMbs != str))
        ui->mbsTxtEdit->append(str);

    mbsTxt = ui->mbsTxtEdit->toPlainText();
    str = "MOBSAT PEDSTAL SETUPED √" ;
    if((handler->mbsBytes[0] & 0x01) && (lastStMbs != str))
    {    ui->mbsTxtEdit->clear();
         ui->mbsTxtEdit->setTextColor(QColor(120,120,120));
         ui->mbsTxtEdit->setText(mbsTxt);
         ui->mbsTxtEdit->setTextColor(QColor(70,70,70));
         ui->mbsTxtEdit->append(str);
    }

    str = "MOBSAT PEDSTAL SETUPING STOPPED" ;
    if((handler->mbsBytes[1] & 0x80) && (lastStMbs != str))
        ui->mbsTxtEdit->append(str);

    str = "SETUP FAULT" ;
    if((handler->mbsBytes[1] & 0x40) && (lastStMbs != str))
        ui->mbsTxtEdit->append(str);

    //===================== Relief =======================//
    str = "MOBSAT PEDSTAL IS RELIEFING..." ;
    if((handler->mbsBytes[2] & 0x80)  && (lastStMbs != str))
        ui->mbsTxtEdit->append(str);


    str = "FRONT & REAR ARE OPEN" ;
    if((handler->mbsBytes[2] & 0x40) && (lastStMbs != str))
        ui->mbsTxtEdit->append(str);

    str = "PEDSTAL LOCK IS CLOSE" ;
    if((handler->mbsBytes[2] & 0x20) && (lastStMbs != str))
        ui->mbsTxtEdit->append(str);


    strtemp = "MOBSAT PEDSTAL IS GOING FORWARD" ;
    if((handler->mbsBytes[2] & 0x10) && (lastStMbs != strtemp))
        ui->mbsTxtEdit->append(strtemp);


    str = "MOBSAT PEDSTAL IS IN OK POSITION" ;
    if((handler->mbsBytes[2] & 0x08) && (lastStMbs != str))
    {
        mbsTxt = ui->mbsTxtEdit->toPlainText();
        mbsTxt.remove(strtemp);
        mbsTxt.remove(mbsTxt.lastIndexOf("\n"),1);
        ui->mbsTxtEdit->setText(mbsTxt);
        ui->mbsTxtEdit->append(str);
    }

    str = "REAR CLOSE IS CLOSE" ;
    if((handler->mbsBytes[2] & 0x04)&& (lastStMbs != str))
        ui->mbsTxtEdit->append(str);

    str = "PEDSTAL LOCK IS CLOSE" ;
    if((handler->mbsBytes[2] & 0x02)&& (lastStMbs != str))
        ui->mbsTxtEdit->append(str);


    str = "DOOR IS CLOSE" ;
    if((handler->mbsBytes[2] & 0x02)&& (lastStMbs != str))
        ui->mbsTxtEdit->append(str);

    mbsTxt = ui->mbsTxtEdit->toPlainText();
    str = "MOBSAT PEDSTAL RELIEFED √" ;
    if((handler->mbsBytes[2] & 0x01)&& (lastStMbs != str))
    {    ui->mbsTxtEdit->clear();
         ui->mbsTxtEdit->setTextColor(QColor(120,120,120));
         ui->mbsTxtEdit->setText(mbsTxt);
         ui->mbsTxtEdit->setTextColor(QColor(70,70,70));
         ui->mbsTxtEdit->append(str);
    }

    str = "MOBSAT PEDSTAL RELIEFING STOPPED" ;
    if((handler->mbsBytes[3] & 0x80)&& (lastStMbs != str))
        ui->mbsTxtEdit->append(str);


    str = "RELIEF FAULT" ;
    if((handler->mbsBytes[3] & 0x40) && (lastStMbs != str))
        ui->mbsTxtEdit->append(str);


    mbsTxt = ui->mbsTxtEdit->toPlainText();
    if(ui->mbsTxtEdit->document()->lineCount()>7)
    {
        QString txt = ui->mbsTxtEdit->toPlainText();
        mbsTxt.clear();
        ui->mbsTxtEdit->clear();
        QStringList ls =  txt.split('\n',QString::SkipEmptyParts);
        ui->mbsTxtEdit->setText(ls.at(7));
        mbsTxt.append(ls.at(7));
    }
}


void Mobsat::checkGPIO()
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
