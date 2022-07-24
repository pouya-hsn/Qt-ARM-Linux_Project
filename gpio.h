/*to use gpio class you just declare like this:
 * GPIO test = new GPIO(PIN32);
 * test->openPin(); // open pin
 * test->setDirection(GPIO::Out); // set direction to output
 * test->setState(false); // set value to low.
 * test->closePin();//close pin
 * delete test; // delete pointer to avoid leak memory.
*/

#ifndef GPIO_H
#define GPIO_H

#include <QObject>
#include <qstring.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Header pin number definitions
#define GPIO0_X 966
#define GPIO1_X 967
#define GPIO2_X 968
#define GPIO3_X 969
#define GPIO4_X 970
#define GPIO5_X 971
#define GPIO6_X 972
#define GPIO7_X 973
#define GPIO8_X 974
#define GPIO9_X 975
//#define GPIO10_X 976


class gpio : public QObject
{
    Q_OBJECT
public:
    gpio(int pin, QObject *parent = NULL);
    ~gpio();

    enum Direction {In, Out, Err};
    int openPin();
    int closePin();
    int setDirection(Direction direction);
    int getDirection(); //returns direction
    int setState(bool state);
    void setValue(bool value);
    bool getState();
signals:
    void getCommand(int _pin) ;
private:
       Direction _direction;
       int _state;
       int _pinNumber ;
       QString _directionString ;
       QString _valueString ;
       QString _strPin;
};

#endif // GPIO_H
