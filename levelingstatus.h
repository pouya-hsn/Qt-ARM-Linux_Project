#ifndef LEVELINGSTATUS_H
#define LEVELINGSTATUS_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class levelingstatus;
}

class levelingstatus : public QWidget
{
    Q_OBJECT

public:
    explicit levelingstatus(QWidget *parent = nullptr);
    ~levelingstatus();
private slots:
    void checkGPIO() ;

    void on_cmmndBtn_clicked();

    void on_pwrBtn_clicked();

    void on_clrBtn_clicked();

    void on_tempClrBtn_clicked();

    void on_bckBtn_clicked();

    void on_mbnBtn_clicked();

    void on_mbstBtn_clicked();

    void on_lvlingBtn_clicked();

    void setStatusBytes() ;

private:
    Ui::levelingstatus *ui;
};

#endif // LEVELINGSTATUS_H
