#ifndef MOBSATSERVICES_H
#define MOBSATSERVICES_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class mobsatservices;
}

class mobsatservices : public QWidget
{
    Q_OBJECT

public:
    explicit mobsatservices(QWidget *parent = nullptr);
    ~mobsatservices();
    QByteArray pcktSend(unsigned char data, unsigned char byteNum);

private slots:
    void checkGPIO() ;

    void getBackStopBtn() ;

    void getBackStopFrontBtn() ;
    void getBackStopRearBtn() ;

    void on_bckMbstBtn_clicked();

    void on_rearBtn_clicked();

    void on_stopLvlBtn_clicked();

    void on_frontBtn_clicked();

    void setStatusBytes() ;

    void on_pedClmpBtn_clicked();

    void on_rearClmpBtn_clicked();

    void on_frontClmpBtn_clicked();

    void on_pedParkMbsBtn_clicked();

    void pedParkMbsCheck() ;

    void on_pedPwrMbsBtn_clicked();

private:
    Ui::mobsatservices *ui;
};

#endif // MOBSATSERVICES_H
