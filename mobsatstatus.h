#ifndef MOBSATSTATUS_H
#define MOBSATSTATUS_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class mobsatstatus;
}

class mobsatstatus : public QWidget
{
    Q_OBJECT

public:
    explicit mobsatstatus(QWidget *parent = nullptr);
    ~mobsatstatus();

private slots:
    void checkGPIO() ;

    void on_cmmndBtn_clicked();

    void on_pwrBtn_clicked();

    void on_clrBtn_clicked();

    void on_tempClrBtn_clicked();

    void on_bckBtn_clicked();

    void on_mbnBtn_clicked();

    void on_lvlingBtn_clicked();

    void on_mbstBtn_clicked();

    void defStateBytes() ;

private:
    Ui::mobsatstatus *ui;
};

#endif // MOBSATSTATUS_H
