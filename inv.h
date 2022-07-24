#ifndef INV_H
#define INV_H

#include <QWidget>

namespace Ui {
class inv;
}

class inv : public QWidget
{
    Q_OBJECT

public:
    explicit inv(QWidget *parent = nullptr);
    ~inv();

private slots:
    void on_backBtn_clicked();

    void on_cmdBtn_clicked();

    void on_sttsBtn_clicked();

    void on_mbnBtn_clicked();

    void on_mbsBtn_clicked();

    void on_lvlBtn_clicked();

    void on_coolerBtn_clicked();

    void on_tmpHmdBtn_clicked();

    void setStatus();
    void checkGPIO() ;

private:
    Ui::inv *ui;
};

#endif // INV_H
