#include "genfunc.h"

genFunc::genFunc(QObject *parent) : QObject(parent)
{

}

void genFunc::onButton(QPushButton *pBtn)
{
    QPixmap pic(":/onBtn.png");
    QIcon icon(pic);
    pBtn->setIcon(icon) ;
    pBtn->setIconSize(QSize(pic.width(), pic.height()));
}

void genFunc::offButton(QPushButton *pBtn)
{
    QPixmap pic(":/offBtn.png");
    QIcon icon(pic);
    pBtn->setIcon(icon);
    pBtn->setIconSize(QSize(pic.width(),pic.height()));
}

void genFunc::defState(QPushButton *btn, bool en, QSize size)
{
    if(en)
    {
        QPixmap pic(":/crclGrnBtn.png");
        QIcon icon(pic);
        btn->setIcon(icon);
        btn->setIconSize(size);
    }
    else
    {
        QPixmap pic(":/crclRdBtn.png");
        QIcon icon(pic);
        btn->setIcon(icon);
        btn->setIconSize(size);

    }
}

void genFunc::setanIcon(QString path, QPushButton *btn)
{
    QPixmap pic(path);
    QIcon icon(pic);
    btn->setIcon(icon);
    btn->setIconSize(QSize(pic.width() , pic.height()));
}
