#ifndef MOBIN_H
#define MOBIN_H

#include <QWidget>

namespace Ui {
class Mobin;
}

class Mobin : public QWidget
{
    Q_OBJECT

public:
    explicit Mobin(QWidget *parent = nullptr);
    ~Mobin();

private slots:
    void checkGPIO() ;

    void on_mbnBackBtn_clicked();

    void on_mbnCmdBtn_clicked();

    void on_mbnSttsBtn_clicked();

    void on_mbnMbsBtn_clicked();

    void on_mbnPwrBtn_clicked();

    void on_mbnClrBtn_clicked();

    void on_mbnTmpBtn_clicked();

    void on_mbnLvlBtn_clicked();

    void checkMobinSetup();

private:
    Ui::Mobin *ui;
};

#endif // MOBIN_H
