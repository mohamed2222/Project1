/****************************************************************
 * References:													*
 *		[1] https://doc.qt.io/archives/qt-4.8/qpainter.html		*
 *		[2] http://www.cplusplus.com/reference/vector/vector/	*
 *																*
 *	All were used to gather information about the various		*
 *  classes and their member functions							*
 *																*
 ****************************************************************/

#include "receive.h"

#include <QDebug>
#include <QPushButton>

//functionality definitions
static vector<bool> RIGHT   = {1,0,0,0,0};
static vector<bool> CLEAR   = {1,0,0,0,1};
static vector<bool> UNDO    = {1,0,0,1,0};
//colour definitions
static vector<bool> BLACK   = {1,0,0,1,1};
static vector<bool> BLUE    = {1,0,1,0,0};
static vector<bool> RED     = {1,0,1,0,1};
static vector<bool> MAGENTA = {1,0,1,1,0};
static vector<bool> GREEN   = {1,0,1,1,1};
static vector<bool> YELLOW  = {1,1,0,0,0};

//define line thicknesses binary
static vector<bool> ONE   = {0,0};
static vector<bool> TWO   = {0,1};
static vector<bool> THREE = {1,0};
static vector<bool> FOUR  = {1,1};

Receive::Receive()
{
    colourVal = 0;
    clear();
    //set default values of member variables
}

void Receive::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPen pen;

    unsigned int x1, y1, x2, y2, i, j;

    for(i = 0; i < x.size(); i++) {
        for(j = 1; j < x[i].size(); j++) {
            x1 = x[i][j-1];
            y1 = y[i][j-1];
            x2 = x[i][j];
            y2 = y[i][j];
            //take vector variables

            colourSet(penColour[i][j], pen);
            pen.setWidth(lineThick[i][j]);
            painter.setPen(pen);
            //set pen characteristics of line

            painter.drawLine(x1, y1, x2, y2);
            //draw line
        }
    }
    //loop through double vectors to draw all lines

    for(i = 1; i < x_temp.size(); i++) {
        x1 = x_temp[i-1];
        y1 = y_temp[i-1];
        x2 = x_temp[i];
        y2 = y_temp[i];
        //take vector variables

        colourSet(pen_colour[i], pen);
        pen.setWidth(lineThick_temp[i]);
        painter.setPen(pen);
        //set pen characteristics of line

        painter.drawLine(x1, y1, x2, y2);
        //draw line
    }
    //loop through single vectors to draw all lines
}

void Receive::clear() {
    x.clear();
    y.clear();
    lineThick.clear();
    penColour.clear();
    x_temp.clear();
    y_temp.clear();
    lineThick_temp.clear();
    pen_colour.clear();
    //clear vectors

    update();
    //update window display
    //vectors are empty so screen will be blank
}

void Receive::undo() {
    if(x_temp.size() == 2) { //if two entries in single vectors
            x_temp.clear();
            y_temp.clear();
            pen_colour.clear();
            lineThick_temp.clear();
            //clear single vectors
    }
    else if(x_temp.size()) { //if single vectors contain any number of values
        x_temp.pop_back();
        y_temp.pop_back();
        pen_colour.pop_back();
        lineThick_temp.pop_back();
        //remove last element from single vectors
    }
    else if(x.size()) { //if single vectors are empty but double vectors aren't
        x_temp = x[x.size() - 1];
        y_temp = y[y.size() - 1];
        pen_colour = penColour[penColour.size() - 1];
        lineThick_temp = lineThick[lineThick.size() - 1];
        //save last row of double vectors to single vectors

        x.pop_back();
        y.pop_back();
        penColour.pop_back();
        lineThick.pop_back();
        //remove last row from double vectors
    }

    update();
    //update window to changes
}

void Receive::setWindow() {
    windowTitle();
    setWindowSize();
    //introduce x and y variables
    //to switch between overloaded function
    deleteFeatures();
} //reimplement from Transmit to use new windowTitle() function

void Receive::windowTitle() {
    setWindowTitle("Receiver");
}

void Receive::ConvToDecX(const vector<bool>x1){
    int decimal = 0;
    unsigned int i;

    for(i = 0; i < x1.size(); i++) {
        decimal += x1[i] * (pow(2, i));
        //equation converts binary digit to decimal
        //and adds it onto decimal
    }

    x_temp.push_back(decimal);
    //add coordinate to x_temp
}

void Receive::ConvToDecY(const vector<bool>y1){
    int decimal = 0;
    unsigned int i;

    for(i = 0; i < y1.size(); i++) {
        decimal += y1[i] * (pow(2, i));
        //equation converts binary digit to decimal
        //and adds it onto decimal
    }

    y_temp.push_back(decimal);
    //add coordinate to y_temp
}

void Receive::unPackData(const vector<bool> x_in, const vector<bool> y_in, const vector<bool> thickness){

    ConvToDecX(x_in);
    ConvToDecY(y_in);

    setThickness(thickness);
    pen_colour.push_back(colourVal);
    //add element to pen_colour

    update();
} //run code to take in new coordinates and update window

void Receive::instructionReceive(const vector<bool> in) {

    if(in == RIGHT) {
        vectorTransfer();
    }
    else if(in == CLEAR) {
        clear();
    }
    else if(in == UNDO) {
        undo();
    }
    else if(in == BLACK) {
        colourVal = 0;
    }
    else if(in == BLUE) {
        colourVal = 1;
    }
    else if(in == RED) {
        colourVal = 2;
    }
    else if(in == MAGENTA) {
        colourVal = 3;
    }
    else if(in == GREEN) {
        colourVal = 4;
    }
    else if(in == YELLOW) {
        colourVal = 5;
    }
    else {
        qDebug() << "Error in transmission.";
    }
} //selects functionality based on input from transmitter

void Receive::vectorTransfer() {
    x.push_back(x_temp);
    y.push_back(y_temp);
    lineThick.push_back(lineThick_temp);
    penColour.push_back(pen_colour);
    //add new row to double vectors of coresponding single vectors

    x_temp.clear();
    y_temp.clear();
    lineThick_temp.clear();
    pen_colour.clear();
    //clear single vectors
}

void Receive::setThickness(const vector<bool> thickness) {

    if(thickness == ONE) {
        lineThick_temp.push_back(1);
    }
    else if(thickness == TWO) {
        lineThick_temp.push_back(2);
    }
    else if(thickness == THREE) {
        lineThick_temp.push_back(3);
    }
    else if(thickness == FOUR) {
        lineThick_temp.push_back(4);
    }
    else {
        qDebug() << "Error in transmission.";
    }
} //adds new lineThick_temp element based on thickness input from transmitter
