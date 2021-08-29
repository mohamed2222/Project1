/************************************************************************************
 * References:																		*
 * 		[1] https://www.fayewilliams.com/2015/03/31/pthreads-tutorial-linux-cpp/	*
 *		[2] https://www.tutorialspoint.com/cplusplus/cpp_multithreading.htm			*
 *		[3] https://doc.qt.io/qt-5/qmutex.html		
 *		[4] https://techterms.com/definition/parity_bit
 *																					*
 * All were sources were used to gain understanding of their respective subjects	*
 * which is used throughout this code.												*
 *																					*
 ************************************************************************************/

/*****************************************************************
 * Note:                                                         *
 *      In this code and the associated classes, binary numbers  *
 *      are read from left to right rather than right to left.   *
 *          The LSF is on the left                               *
 *          The MSF is on the right                              *
 * Note:                                                         *
 *      Communication between the threads is checked using a     *
 *      positive parity check                                    *
 *                                                               *
 * Pins:                                                         *
 *      The global bool variables are representing pins on the   *
 *      pi board. They work as if one pin was connected to the   *
 *      corresponding pin non another board.                     *
 *       ___________________________________________________     *
 *      |Variable  |Job                                     |    *
 *      |**********|****************************************|    *
 *      |indicator |Used by both threads to indicate        |    *
 *      |          |when an action is completed             |    *
 *      |----------+----------------------------------------|    *
 *      |type      |Used to indicate what information       |    *
 *      |          |is being transmitted                    |    *
 *      |----------+----------------------------------------|    *
 *      |error     |Used to indicate when there is an error |    *
 *      |          |in the transmission of the data         |    *
 *      |----------+----------------------------------------|    *
 *      |wait      |Ensures transmission is complete before |    *
 *      |          |new instructions are accepted           |    *
 *      |----------+----------------------------------------|    *
 *      |p0        |                                        |    *
 *      |p1        |                                        |    *
 *      |p2        |                                        |    *
 *      |p3        |                                        |    *
 *      |p4        |Data transmission pins                  |    *
 *      |p5        |                                        |    *
 *      |p6        |                                        |    *
 *      |p7        |                                        |    *
 *      |p8        |                                        |    *
 *      |----------+----------------------------------------|    *
 *      |p9        |Parity bit [4]                             |    *
 *      |***************************************************|    *
 *                                                               *
 *****************************************************************/

#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QMutex>

#include <pthread.h>
#include <wiringPi.h>
#include <stdio.h>

#include "receive.h"
#include "transmit.h"

using std::vector;

QMutex mutex;
bool indicator, type, error, wait, p0, p1, p2, p3, p4, p5, p6, p7, p8, p9;

struct transmitComms {
    Transmit t;
    vector<bool> send;
    int count, x_size;
}; //variables specfic to transmit thread

struct receiveComms {
    Receive r;
    vector<bool> newX, newY, newThick, newInstruction;
    int numLoop;
    bool chk;
}; //variables specific to receive thread
//[2] was used for the principle of passing data into threads via a struct

void* transmitWorker(void* inputArg);
void* receiveWorker(void* inputArg);

void assignBits(const vector<bool> output, int& count);
void readBits(vector<bool> &input);
bool parityCheck(const vector<bool> v);
void initialPrint();

int main(int argc, char *argv[])
{
    // setup GPIO interface - uncomment when needed
    // needs to run with root via sudo in terminal.
    //wiringPiSetup();
    //pinMode (0, OUTPUT);

    // setup Qt GUI
    QApplication a(argc, argv);

    initialPrint();

    pthread_t transmit_thread, receive_thread;
    //create transmit and receive threads

    indicator = 0;
    //set initial value of 'indicator'

    struct transmitComms transmitter;
    //create thread 'transmitter'

    transmitter.t.setWindow();
    transmitter.t.show();
    //setup transmit window

    transmitter.x_size = transmitter.t.getXTempSize(); //set x_size

    // starting worker thread(s)
    int rc;
    rc = pthread_create(&transmit_thread, NULL, transmitWorker, (void*)&transmitter);
    if (rc) {
        qDebug() << "Unable to start worker thread.";
        exit(1);
    }

/**********************************************************************************************************/

    struct receiveComms receiver;
    //create thread 'reciever'

    receiver.r.setWindow();
    receiver.r.show();
    //setup receive window
    receiver.numLoop = 0; //set 'numLoop' default value

    // starting worker thread(s)
    rc = pthread_create(&receive_thread, NULL, receiveWorker, (void*)&receiver);
    if (rc) {
        qDebug() << "Unable to start worker thread.";
        exit(1);
    }

/**********************************************************************************************************/

    // start window event loop
    qDebug() << "Starting event loop...";
    int ret = a.exec();
    qDebug() << "Event loop stopped.";

    // cleanup pthreads
    pthread_exit(NULL);

    // exit
    return ret;
}


void* transmitWorker(void* inputArg)
{
    unsigned int i;
    struct transmitComms * transmitter;
    transmitter = (struct transmitComms *) inputArg;
    //create new struct and give it values of struct passed in to function [2]

    if(transmitter->t.instructBin[0]) { //check if there's a new instruction from transmit window
        transmitter->x_size = transmitter->t.getXTempSize(); //reset 'x_size' in case it's changed
        mutex.lock();
        indicator = type = 1;
        mutex.unlock();
        //communicate with receive thread
        while(indicator);
        //wait for receive thread to respond

        do {
            transmitter->send.clear(); //clear 'send' vector ready for reading instruction
            for(i = 0; i < transmitter->t.instructBin.size(); i++) { //read instruction from transmit class
                transmitter->send.push_back(transmitter->t.instructBin[i]);
                //save instruction to 'send' vector
                if(transmitter->t.instructBin[i])
                    transmitter->count++;
                    //record number of 1s in instruction for parity bit
            }

            for(i = 0; i < (9 - transmitter->t.instructBin.size()); i++)
                transmitter->send.push_back(0);
                //increase size of 'send' vector to 9 elements

            assignBits(transmitter->send, transmitter->count); //send instruction
            while(indicator);
            //wait for receive thread to respond
        } while(error); //repeat process if there's an error

        mutex.lock();
        indicator = 1;
        mutex.unlock();
        //communicate with receive thread

        transmitter->t.instructBin = {0};
        //reset class variable 'instructBin'
    }

    else if(transmitter->x_size != transmitter->t.getXTempSize()) { //if new coordinate
        transmitter->x_size = transmitter->t.getXTempSize(); //reset 'x_size'
        mutex.lock();
        indicator = 1;
        type = 0;
        mutex.unlock();
        //communicate with receive thread
        while(indicator);
        //wait for receive thread to respond

        do {
            transmitter->send.clear(); //clear 'send' vector ready for x coordinate
            for(i = 0; i < transmitter->t.x_bin.size(); i++) { //read x coordinate
                transmitter->send.push_back(transmitter->t.x_bin[i]);
                //save x coordinate to 'send' vector
                if(transmitter->t.x_bin[i])
                    transmitter->count++;
                    //count number of 1s for parity
            }

            for(i = 0; i < (9 - transmitter->t.x_bin.size()); i++)
                transmitter->send.push_back(0);
                //increase size of 'send' vector to 9 elements

            assignBits(transmitter->send, transmitter->count); //send coordinate
            while(indicator);
            //wait for receive thread to respond
        } while(error); //repeat process if error

        do {
            transmitter->send.clear(); //clear 'send' vector ready for y coordinate
            for(i = 0; i < transmitter->t.y_bin.size(); i++) { //read y coordinate
                transmitter->send.push_back(transmitter->t.y_bin[i]);
                //save y coordinate to 'send' vector
                if(transmitter->t.y_bin[i])
                    transmitter->count++;
                    //count number of 1s for parity
            }

            for(i = 0; i < (9 - transmitter->t.y_bin.size()); i++)
                transmitter->send.push_back(0);
                //increase size of 'send' vector to 9 elements

            assignBits(transmitter->send, transmitter->count); //send coordinate
            while(indicator);
            //wait for receive thread to respond
        } while(error); //repeat process if error

        do {
            transmitter->send.clear(); //clear 'send' vector ready for thickness data
            for(i = 0; i < transmitter->t.thickSend.size(); i++) { //read thickness data
                transmitter->send.push_back(transmitter->t.thickSend[i]);
                //save thickness data to 'send' vector
                if(transmitter->t.thickSend[i])
                    transmitter->count++;
                    //count number of 1s for parity
            }

            for(i = 0; i < (9 - transmitter->t.thickSend.size()); i++)
                transmitter->send.push_back(0);
                //increase size of 'send' vector to 9 elements

            assignBits(transmitter->send, transmitter->count); //send data
            while(indicator);
            //wait for receive thread to respond
        } while(error); //repeat process if error

        mutex.lock();
        indicator = 1;
        mutex.unlock();
        //communicate with receive thread
    } 

    while(wait); //wait for receiver thread to finish
    transmitter->t.setWait(0); //allow transmit window to take in new instructions

    while(!(transmitter->t.instructBin[0]) && transmitter->x_size == transmitter->t.getXTempSize());
    //wait for new instruction or new coordinates
    transmitter->t.setWait(1); //prevent transmit window from accepting new instructions

    // starting worker thread(s)
    pthread_t transmit_thread;
    int rc = pthread_create(&transmit_thread, NULL, transmitWorker, (void*) transmitter);
    if (rc) {
        qDebug() << "Unable to start transmitter thread.";
        qDebug() << "PROGRAM CRASHED";
        exit(1);
    }
    //create new thread to handle change

    pthread_exit(NULL);
    // end thread
}


void* receiveWorker(void* inputArg)
{
    struct receiveComms * receiver;
    receiver = (struct receiveComms *) inputArg;
    //create new struct and give it values of struct passed in to function [2]

    if(type) {
        mutex.lock();
        indicator = 0;
        wait = 1;
        mutex.unlock();
        //respond to transmit thread
        while(!indicator); //wait for signal from transmit thread

        do {
            readBits(receiver->newInstruction); //read instruction
            receiver->chk = parityCheck(receiver->newInstruction); //run parity check on data
            while(!indicator); //wait for signal from transmit thread
        } while(receiver->chk);
        //if error returned from parity check repeat process

        receiver->newInstruction.erase(receiver->newInstruction.begin() +5, receiver->newInstruction.end());
        //resize vector for receive class
        receiver->r.instructionReceive(receiver->newInstruction);
        //pass instruction on to receive window
    }

    else if(!type && receiver->numLoop) {
        mutex.lock();
        indicator = 0;
        wait = 1;
        mutex.unlock();
        //respond to transmit thread
        while(!indicator); //wait for signal from transmit thread

        do {
            readBits(receiver->newX); //read x coordinate
            receiver->chk = parityCheck(receiver->newX); //run parity check on data
            while(!indicator); //wait for signal from transmit thread
        } while(receiver->chk);
        //if error returned from parity check repeat process

        do {
            readBits(receiver->newY); //read y coordinate
            receiver->chk = parityCheck(receiver->newY); //run parity check on data
            while(!indicator); //wait for signal from transmit thread
        } while(receiver->chk);
        //if error returned from parity check repeat process

        do {
            readBits(receiver->newThick); //read thickness data
            receiver->chk = parityCheck(receiver->newThick); //run parity check on data
            while(!indicator); //wait for signal from transmit thread
        } while(receiver->chk);
        //if error returned from parity check repeat process

        receiver->newThick.erase(receiver->newThick.begin() +2, receiver->newThick.end());
        //resize thickness vector for receive class
        receiver->r.unPackData(receiver->newX, receiver->newY, receiver->newThick);
        //pass coordinates and thickness data onto teh receive window
    }

    mutex.lock();
    indicator = wait = 0;
    mutex.unlock();
    //respond to transmit thread

    receiver->numLoop = 1; //set 'numLoop'
    while(!indicator); //wait for signal from transmit thread

    // starting worker thread(s)
    pthread_t receive_thread;
    int rc = pthread_create(&receive_thread, NULL, receiveWorker, (void*) receiver);
    if (rc) {
        qDebug() << "Unable to start receiver thread.";
        qDebug() << "PROGRAM CRASHED";
        exit(1);
    }
    //create new thread to handle change

    pthread_exit(NULL);
    //end thread
}


void assignBits(const vector<bool> output, int& count) {
    mutex.lock();

    p0 = output[0];
    p1 = output[1];
    p2 = output[2];
    p3 = output[3];
    p4 = output[4];
    p5 = output[5];
    p6 = output[6];
    p7 = output[7];
    p8 = output[8];
    //assign passed in vector to data transmission pins

    if(count % 2)
        p9 = 1;
    else
        p9 = 0;
    //decide on value of parity bit
    //if count is odd: p9 = 1
    //else:            p9 = 0

    indicator = 1;
    count = 0;
    //communicate with receive thread

    mutex.unlock();
}

void readBits(vector<bool>& input) {
    input.clear(); //clear vector passed in

    mutex.lock();

    input.push_back(p0);
    input.push_back(p1);
    input.push_back(p2);
    input.push_back(p3);
    input.push_back(p4);
    input.push_back(p5);
    input.push_back(p6);
    input.push_back(p7);
    input.push_back(p8);
    //add bits to vector from data transmission pins

    mutex.unlock();
}

bool parityCheck(const vector<bool> v) {
    mutex.lock();
    unsigned int i, count;
    count = 0; //set 'count' variable

    for(i = 0; i < v.size(); i++) {
        if(v[i])
            count++;
            //increment 'count' when value of v is 1
    }

    if(p9)
        count++;
        //increment 'count' if parity bit is 1

    if(count % 2) {
        qDebug() << "Error";
        error = 1;
        indicator = 0;
        mutex.unlock();
        //communicate with transmit thread
        return 1;
        //return an error if count is odd
    } else {
        error = indicator = 0;
        mutex.unlock();
        //communicate with transmit thread
        return 0;
        //return no error if count is even
    }
}

void initialPrint() {
    qDebug() << " Instructions For Using Communication Windows: ";
    qDebug() << "_________________________________________________________________________________";
    qDebug() << "1. Clicking on receiver window will do nothing.";
    qDebug() << "2. Left click to define a point.";
    qDebug() << "     -- Lines are drawn between these points.";
    qDebug() << "3. Right click to create a break between lines.";
    qDebug() << "4. Clicking the 'Clear' button at the bottom of the window will clear the screen.";
    qDebug() << "     -- Clciking the scroll wheel on the mouse will also clear the screen.";
    qDebug() << "     -- Clearing the screen cannot be undone.";
    qDebug() << "5. To change the line thickness select the desired number on the spin box and";
    qDebug() << "   proceed to draw your line. The value in the box is read every time the left";
    qDebug() << "   mouse button is clicked.";
    qDebug() << "7. Pressing the 'Undo' button will remove only one line from the screen.";
    qDebug() << "8. The 'Undo' button can be used until the window is clear.";
    qDebug() << "9. To change the line colour select your desired colour and then proceed to draw.";
    qDebug() << "     -- The line colour will remain consistent until a new colour is selected.";
    qDebug() << " ";
    qDebug() << "Error messages and instructions will appear here when appropriate.";
    qDebug() << "_________________________________________________________________________________";

    //message printed once at initiation of thread
}
