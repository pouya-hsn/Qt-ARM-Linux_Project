#ifndef COMMAND_H
#define COMMAND_H

#include <QWidget>
#include <QPushButton>
namespace Ui {
class command;
}

class command : public QWidget
{
    Q_OBJECT

public:
    explicit command(QWidget *parent = nullptr);
    ~command();
private:
    QByteArray pcktSend(unsigned char data,unsigned char byteNum);
    void setanIcon(QString path, QPushButton *btn);

public slots:
    void on_mbstStupBtn_clicked();

    void on_mbnStupBtn_clicked();

    void on_lvlingStupBtn_clicked();

    void on_mbstRlifBtn_clicked();

    void on_mbnRlifBtn_clicked();

    void on_lvlingRlifBtn_clicked();

    void on_nrmlStupModBtn_clicked();

    void on_nrmlRlifModBtn_clicked();

    void on_stopBtn_clicked();

    void getBackStopBtn() ;

private slots:
    void on_bckCmmndBtn_clicked();

    void on_srvicsBtn_clicked();

    void mbnSetDefState();
    void mbsSetDefState();
    void lvlSetDefState();

    void mbnRlfDefState();
    void mbsRlfDefState();
    void lvlRlfDefState();

    void checkLvl();
    void checkMbn();
    void checkMbs();
    void checkGPIO() ;

private:
    Ui::command *ui;
};

#endif // COMMAND_H
