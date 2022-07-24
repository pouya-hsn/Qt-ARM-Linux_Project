#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QPushButton>
QT_BEGIN_NAMESPACE
namespace Ui { class home; }
QT_END_NAMESPACE

class home : public QMainWindow
{
    Q_OBJECT

public:
    home(QWidget *parent = nullptr);
    ~home();

private:
    void tmpDisp();
    void hmdDisp();
    void kab1StDisp();
    void kab2StDisp();
//    void pwrStDisp();
    void setanIcon(QString path,QPushButton *btn) ;



private slots:
    void checkGPIO();

    void on_clrHomeBtn_clicked();

    void on_cmmndHomeBtn_clicked();

    void on_sttsHomeBtn_clicked();

    void on_tempHomeBtn_clicked();

    void on_pwrHomeBtn_clicked();

    void on_mbnHomeBtn_clicked();

    void on_mbstHomeBtn_clicked();

    void on_lvlingHomeBtn_clicked();

    void lvlSetDefState();

    void lvlRlfDefState();

    void mbnSetDefState();

    void mbnRlfDefState();

    void mbsSetDefState();

    void mbsRlfDefState();

    void XYAnglesShow();

private:
    Ui::home *ui;
};
#endif // HOME_H
