#include "home.h"
#include "ui_home.h"
#include "cooler.h"
#include "pckthandler.h"
#include "mobinstatus.h"
#include "command.h"
#include "status.h"
#include "mobsatstatus.h"
#include "levelingstatus.h"
#include "temp.h"
#include "power.h"
#include <QDebug>
#include <QTimer>
#include "mobin.h"
#include "mobsat.h"
#include "leveling.h"
#include "gpio.h"

extern cooler *coolObj;
extern home *homeObj ;
extern pckthandler *handler ;
extern mobinstatus *mobStatusObj;
extern command *cmdObj;
extern status *statusObj;
extern mobsatstatus *msatStatusObj;
extern levelingstatus *lvlStatusObj ;
extern temp *tmpHmdObj;
extern power *pwrObj;
extern bool kab1State ;
extern bool kab2State ;
extern Mobin *mbnObj ;
extern Mobsat *mbsObj ;
extern Leveling *lvlObj;

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

bool setLvl = true ;
bool rlfLvl = true ;
bool setMbn = true ;
bool rlfMbn = true ;
bool setMbs = true ;
bool rlfMbs = true ;

QTimer *timer = new QTimer();

home::home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::home)
{
    timer->stop();
    timer = new QTimer();
    gpioTim = new QTimer();
    connect(gpioTim, &QTimer::timeout, this, &home::checkGPIO);
    gpioTim->start(50);

    connect(timer, SIGNAL(timeout()) , this , SLOT(lvlSetDefState()));
    connect(timer, SIGNAL(timeout()) , this , SLOT(lvlRlfDefState()));
    connect(timer, SIGNAL(timeout()) , this , SLOT(mbnSetDefState()));
    connect(timer, SIGNAL(timeout()) , this , SLOT(mbnRlfDefState()));
    connect(timer, SIGNAL(timeout()) , this , SLOT(mbsSetDefState()));
    connect(timer, SIGNAL(timeout()) , this , SLOT(mbsRlfDefState()));

    timer->start(1000);
    ui->setupUi(this);

    tmpDisp(); // Temp value Display
    hmdDisp(); // Humid value Display
    kab1StDisp(); // Cabin1's cooler state Display
    kab2StDisp(); // Cabin2's cooler state Display

    connect(handler, &pckthandler::tmpHmdChanged, this, &home::tmpDisp) ; // Temp value Display
    connect(handler, &pckthandler::tmpHmdChanged, this, &home::hmdDisp) ; // Humid value Display

    connect(coolObj, &cooler::kab1CoolerStatus, this, &home::kab1StDisp) ;
    connect(coolObj, &cooler::kab2CoolerStatus, this, &home::kab2StDisp) ;

//    pwrStDisp(); // Power Section State Display
//    connect(handler, &pckthandler::pwrChanged, this, &home::pwrStDisp) ;

    XYAnglesShow();
    connect(handler, &pckthandler::sttsChanged, this, &home::XYAnglesShow) ;
}

home::~home()
{
    delete ui;
}


void home::on_clrHomeBtn_clicked()
{
    coolObj = new cooler();
    coolObj->show();
    homeObj->close();
    timer->stop();
    gpioTim->stop();
}


void home::on_cmmndHomeBtn_clicked()
{
    cmdObj = new command();
    cmdObj->show();
    homeObj->close();
    timer->stop();
    gpioTim->stop();
}


void home::on_sttsHomeBtn_clicked()
{
    statusObj = new status();
    statusObj->show();
    homeObj->close();
    timer->stop();
    gpioTim->stop();
}

void home::on_tempHomeBtn_clicked()
{
    tmpHmdObj = new temp();
    tmpHmdObj->show();
    homeObj->close();
    timer->stop();
    gpioTim->stop();
}

void home::on_pwrHomeBtn_clicked()
{
    pwrObj = new power();
    pwrObj->show();
    homeObj->close();
    timer->stop();
    gpioTim->stop();
}



void home::tmpDisp()
{
    ui->tmpValue->setStyleSheet("QLabel {color: #383637}");
    ui->tmpValue->setText(QString::number(handler->tmpHmdBytes[0]) + " °C");
}

void home::hmdDisp()
{
    ui->hmdValue->setStyleSheet("QLabel {color: #383637}");
    ui->hmdValue->setText(QString::number(handler->tmpHmdBytes[1]) + "%");
}

void home::kab1StDisp()
{
    if (kab1State) {
        ui->kab1stLbl->setStyleSheet("QLabel {color: #1EC81E}");
        ui->kab1stLbl->setText("on");
    }
    else {
        ui->kab1stLbl->setStyleSheet("QLabel {color: #2255FF}");
        ui->kab1stLbl->setText("off");
    }
}

void home::kab2StDisp()
{
    if (kab2State)
    {
        ui->kab2stLbl->setStyleSheet("QLabel {color: #1EC81E}");
        ui->kab2stLbl->setText("on");
    }
    else
    {
        ui->kab2stLbl->setStyleSheet("QLabel {color: #2255FF}");
        ui->kab2stLbl->setText("off");
    }
}

//void home::pwrStDisp()
//{
//    ui->pwrStLbl->setAlignment(Qt::AlignCenter);
//    ui->batPercent->setAlignment(Qt::AlignCenter);

//    int percent = int((float(handler->powBytes[1])/64)*100) ;
//    switch (handler->powBytes[0])
//    {
//    case 0x11: {ui->pwrStLbl->setText("on the battery"); ui->batPercent->setText(QString::number(percent) +"%"); break;}
//    case 0x12: {ui->pwrStLbl->setText("on the dynamo");ui->batPercent->clear();break;}
//    case 0x13: {ui->pwrStLbl->setText("on the urban electricity");ui->batPercent->clear() ; break;}
//    default: ui->pwrStLbl->setText("");
//    }
//}

void home::lvlSetDefState()
{
    if(handler->lvlBytes[1] & 0x02) //  setuping blinking
    {
        if(setLvl)
        {
            setanIcon(":/lvlingGrnHomeBtn.png",ui->lvlingHomeBtn);
            setLvl = false ;
        }
        else
        {
            setanIcon(":/lvlingHomeBtn.png",ui->lvlingHomeBtn);
            setLvl = true ;
        }
    }

    if(handler->lvlBytes[1] & 0x01) // leveled
        setanIcon(":/lvlingGrnHomeBtn.png",ui->lvlingHomeBtn);

    if (handler->lvlBytes[1] & 0x04)  //fault
        setanIcon(":/lvlingRdHomeBtn.png",ui->lvlingHomeBtn);
}

void home::lvlRlfDefState()
{
    if(handler->lvlBytes[3] & 0x08) //blinking
    {
        if(rlfLvl)
        {
            setanIcon(":/lvlingBluHomeBtn.png",ui->lvlingHomeBtn);
            rlfLvl = false ;
        }
        else
        {
            setanIcon(":/lvlingHomeBtn.png",ui->lvlingHomeBtn);
            rlfLvl = true ;
        }
    }
    if(handler->lvlBytes[3] & 0x04) //reliefed
        setanIcon(":/lvlingBluHomeBtn.png",ui->lvlingHomeBtn);

    if (handler->lvlBytes[3] & 0x10)  //fault
        setanIcon(":/lvlingRdHomeBtn.png",ui->lvlingHomeBtn);


}


void home::mbnSetDefState()
{
    if(handler->mbnBytes[1] & 0x40) //  setuping blinking
    {
        if(setMbn)
        {
            setanIcon(":/mbnGrnHomeBtn.png",ui->mbnHomeBtn);
            setMbn = false ;
        }
        else
        {
            setanIcon(":/mbnHomeBtn.png",ui->mbnHomeBtn);
            setMbn = true ;
        }
    }

    if(handler->mbnBytes[1] & 0x20) // setuped
        setanIcon(":/mbnGrnHomeBtn.png",ui->mbnHomeBtn);

    if (handler->mbnBytes[1] & 0x08)  //fault
        setanIcon(":/mbnRdHomeBtn.png",ui->mbnHomeBtn);
}

void home::mbnRlfDefState()
{
    if(handler->mbnBytes[3] & 0x08) //blinking
    {
        if(rlfMbn)
        {
            setanIcon(":/mbnBluHomeBtn.png",ui->mbnHomeBtn);
            rlfMbn = false ;
        }
        else
        {
            setanIcon(":/mbnHomeBtn.png",ui->mbnHomeBtn);
            rlfMbn = true ;
        }
    }
    if(handler->mbnBytes[3] & 0x04) //reliefed
        setanIcon(":/mbnBluHomeBtn.png",ui->mbnHomeBtn);

    if (handler->mbnBytes[3] & 0x10)  //fault
        setanIcon(":/mbnRdHomeBtn.png",ui->mbnHomeBtn);

}

void home::mbsSetDefState()
{
    if(handler->mbsBytes[1] & 0x20) //  setuping blinking
    {
        if(setMbs)
        {
            setanIcon(":/mbstGrnHomeBtn.png",ui->mbstHomeBtn);
            setMbs = false ;
        }
        else
        {
            setanIcon(":/mbstHomeBtn.png",ui->mbstHomeBtn);
            setMbs = true ;
        }
    }

    if(handler->mbsBytes[1] & 0x10) // setuped
        setanIcon(":/mbstGrnHomeBtn.png",ui->mbstHomeBtn);

    if (handler->mbsBytes[1] & 0x40) //fault
        setanIcon(":/mbstRdHomeBtn.png",ui->mbstHomeBtn);

}

void home::mbsRlfDefState()
{
    if(handler->mbsBytes[3] & 0x20) //blinking
    {
        if(rlfMbs)
        {
            setanIcon(":/mbstBluHomeBtn.png",ui->mbstHomeBtn);
            rlfMbs = false ;
        }
        else
        {
            setanIcon(":/mbstHomeBtn.png",ui->mbstHomeBtn);
            rlfMbs = true ;
        }
    }
    if(handler->mbsBytes[3] & 0x10) //reliefed
        setanIcon(":/mbstBluHomeBtn.png",ui->mbstHomeBtn);

    if (handler->mbsBytes[3] & 0x40)  //fault
        setanIcon(":/mbstRdHomeBtn.png",ui->mbstHomeBtn);
}

void home::on_mbnHomeBtn_clicked()
{
    mbnObj = new Mobin();
    mbnObj->show();
    homeObj->close();
    timer->stop();
    gpioTim->stop();
}

void home::on_mbstHomeBtn_clicked()
{
    mbsObj = new Mobsat();
    mbsObj->show();
    homeObj->close();
    timer->stop();
    gpioTim->stop();
}


void home::on_lvlingHomeBtn_clicked()
{
    lvlObj = new Leveling();
    lvlObj->show();
    homeObj->close();
    timer->stop() ;
    gpioTim->stop();
}

void home::XYAnglesShow()
{
    // showing the X,Y Angles on Leveling Icon in Home Page
    long int xBytes = ((handler->stBytes[7])& 0x000000FF) | ((handler->stBytes[8] << 8)& 0x0000FF00) |
            ((handler->stBytes[9] << 16)& 0x00FF0000) | ((handler->stBytes[10] << 24)&0xFF000000);
    float x = float(xBytes)/1000 ;

    long int yBytes = ((handler->stBytes[11])& 0x000000FF) | ((handler->stBytes[12] << 8)& 0x0000FF00) |
            ((handler->stBytes[13] << 16)& 0x00FF0000) | ((handler->stBytes[14] <<24) & 0xFF000000);
    float y = float(yBytes)/1000 ;
    ui->xlbl->setText(QString("X = %1").arg(x));
    ui->ylbl->setText(QString("Y = %1").arg(y));
}

void home::setanIcon(QString path,QPushButton *btn)
{
    QPixmap pic(path);
    QIcon icon(pic);
    btn->setIcon(icon);
    btn->setIconSize(QSize(pic.width() , pic.height()));
}

void home::checkGPIO()
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
