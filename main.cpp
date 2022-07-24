#include "activeform.h"
#include "home.h"
#include "cooler.h"
#include "command.h"
#include "levelingservices.h"
#include "services.h"
#include "mobsatstatus.h"
#include "mobsatservices.h"
#include "mobinstatus.h"
#include "mobinservices.h"
#include "power.h"
#include "levelingstatus.h"
#include "pckthandler.h"
#include <QApplication>
#include "status.h"
#include "mobsatstatus.h"
#include "temp.h"
#include "mobin.h"
#include "mobsat.h"
#include "leveling.h"
#include "inv.h"
#include "gpio.h"
#include "ups.h"
#include <QTimer>
#include "genfunc.h"

class Mobin *mbnObj ;
class Mobsat *mbsObj;
class Leveling *lvlObj ;
QTimer *gpioTim ;
home *homeObj;
cooler *coolObj;
pckthandler *handler ;
command *cmdObj;
status *statusObj ;
temp *tmpHmdObj;
levelingServices *lvlSrvObj;
services *srvObj;
levelingstatus *lvlStatusObj ;
mobinstatus *mobStatusObj;
mobinservices *mobSrvObj;
power *pwrObj ;
mobsatstatus *msatStatusObj;
mobsatservices *msatSrvObj;
inv *invObj ;
UPS *upsObj;
genFunc *gen ;
QString cmdTxt;
QString lvlTxt;
QString mbnTxt;
QString mbsTxt;
QString lastStMbn;
QString lastStMbs;
QString lastStCmd;
QString lastStLvl;

field lastField = Null ;


//void defineGPIO();
gpio gp1(GPIO0_X) ; gpio gp2(GPIO1_X) ;gpio gp3(GPIO2_X) ;
gpio gp4(GPIO3_X) ; gpio gp5(GPIO4_X) ; gpio gp6(GPIO5_X) ;gpio gp7(GPIO6_X) ;
gpio gp8(GPIO7_X) ; gpio gp9(GPIO8_X) ; gpio gp10(GPIO9_X) ;

bool kab1State = false ;
bool kab2State = false ;
bool stLvl = true ;

bool upSunroofClicked = false ;
bool dwnSunroofClicked = false ;
bool upMotorClicked = false ;
bool dwnMotorClicked = false ;

bool rearMbsMotor = false ;
bool frontMbsMotor = false ;

bool upJ1 = false ; bool dwnJ1 = false ;
bool upJ2 = false ; bool dwnJ2 = false ;
bool upJ3 = false ; bool dwnJ3 = false ;
bool upJ4 = false ; bool dwnJ4 = false ;
bool upAllJack = false; bool dwnAllJack = false ;

bool mbnSetBtnState = false;
bool mbsSetBtnState = false;
bool lvlSetBtnState = false;

bool mbnRlfBtnState = false;
bool mbsRlfBtnState = false;
bool lvlRlfBtnState = false;
bool pedParkMbnClick = false ;
bool pedParkMbsClick = false ;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    handler = new pckthandler();
    coolObj = new cooler();
    homeObj = new home();
    gp1.openPin(); gp1.setDirection(gpio::In);
    gp2.openPin(); gp2.setDirection(gpio::In);
    gp3.openPin(); gp3.setDirection(gpio::In);
    gp4.openPin(); gp4.setDirection(gpio::In);
    gp5.openPin(); gp5.setDirection(gpio::In);
    gp6.openPin(); gp6.setDirection(gpio::In);
    gp7.openPin(); gp7.setDirection(gpio::In);
    gp8.openPin(); gp8.setDirection(gpio::In);
    gp9.openPin(); gp9.setDirection(gpio::In);
    gp10.openPin(); gp10.setDirection(gpio::In);
    homeObj-> show();

    return a.exec();
}

