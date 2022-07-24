#ifndef GENFUNC_H
#define GENFUNC_H

#include <QObject>
#include <QPushButton>

class genFunc : public QObject
{
    Q_OBJECT
public:
    explicit genFunc(QObject *parent = nullptr);
    void onButton(QPushButton *pBtn);
    void offButton(QPushButton *pBtn) ;
    void defState(QPushButton *btn , bool en, QSize size);
    void setanIcon(QString path, QPushButton *btn);

signals:

};

#endif // GENFUNC_H
