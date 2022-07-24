#include "fifo.h"

FIFO::FIFO(unsigned char size){
    Cnt = 0 ;
    InIDX = 0 ;
    OutIDX = 0;
    _array_ = new unsigned char[size] ;
    buff_size = size ;
}

void FIFO::put(unsigned char _input_){
    if(Cnt<= (buff_size)){
        _array_[InIDX] = _input_ ;    //entering the input
        if(InIDX == (buff_size-1)) {InIDX = 0 ;}
        else {
            InIDX++ ;
        }
        Cnt++ ;
    }
}

unsigned char FIFO::get(){
    unsigned char temp ;
    if(Cnt == 0){
        return 0xFF ;
    }
    else{
        temp = _array_[OutIDX] ; //temporary variable
        if(OutIDX == (buff_size-1)) {OutIDX = 0;}
        else {
            OutIDX++ ;
        }
        Cnt-- ;
        return temp ;
    }
}

void FIFO::read(unsigned char *p){
    if(Cnt == 0){
        //qDebug() << "Sorrry! Buffer is Empty.\n" ;
    }
    else{
        for (int i=0; i< Cnt ;i++) {
            p[i] = _array_[OutIDX+i] ;
        }
    }
}
