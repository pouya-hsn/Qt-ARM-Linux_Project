#ifndef MOBINSTATUS_H
#define MOBINSTATUS_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class mobinstatus;
}

class mobinstatus : public QWidget
{
    Q_OBJECT

public:
    explicit mobinstatus(QWidget *parent = nullptr);
    ~mobinstatus();

private slots:
    void checkGPIO() ;

    void on_cmmndBtn_clicked();

    void on_pwrBtn_clicked();

    void on_clrBtn_clicked();

    void on_tempClrBtn_clicked();

    void on_bckBtn_clicked();

    void on_mbstBtn_clicked();

    void on_lvlingBtn_clicked();

    void on_mbnBtn_clicked();

    void setStatusBytes();

private:
    Ui::mobinstatus *ui;
};

#endif // MOBINSTATUS_H
