#ifndef FIFO_H
#define FIFO_H

class FIFO
{
    public:
        int Cnt;  //Cnter of number of full buffer element
        FIFO(unsigned char size); //constructor
        void put(unsigned char _input_);    //entering a byte
        unsigned char get(); //getting the first input and delete it
        void read(unsigned char *p);    //reading the entire buffer without deleting in
    private:
        unsigned char *_array_;   //array of bytes to save data on that
        int InIDX;    //the index of input to know how full the buffer is
        int OutIDX;   // the index of output to know how empty the buffer is
        unsigned char buff_size ;
};

#endif // FIFO_H
