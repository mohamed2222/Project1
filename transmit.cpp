/************************************************************************************************
 * References:																					*
 *		[1] https://doc.qt.io/qt-5/qabstractbutton.html											*
 *		[2] https://doc.qt.io/qt-5/qmouseevent.html#button										*
 *		[3] https://doc.qt.io/archives/qt-4.8/qpainter.html										*
 *		[4] http://www.cplusplus.com/reference/vector/vector/									*
 *		[5] https://stackoverflow.com/questions/4629629/assign-shortcut-keys-to-buttons-qt-c	*
 *		[6] https://doc.qt.io/qt-5/qspinbox.html												*
 *		[7] https://www.programiz.com/cpp-programming/examples/binary-decimal-convert			*
 *																								*
 ************************************************************************************************/

#include "transmit.h"
#include "ui_window.h"

#include <QDebug>
#include <QtWidgets>
#include <QPixmap>

//functionality definitions
#define RIGHT   {1,0,0,0,0}
#define CLEAR   {1,0,0,0,1}
#define UNDO    {1,0,0,1,0}
//colour definitions
#define BLACK   {1,0,0,1,1}
#define BLUE    {1,0,1,0,0}
#define RED     {1,0,1,0,1}
#define MAGENTA {1,0,1,1,0}
#define GREEN   {1,0,1,1,1}
#define YELLOW  {1,1,0,0,0}

//define line thicknesses binary
#define ONE   {0,0}
#define TWO   {0,1}
#define THREE {1,0}
#define FOUR  {1,1}

Transmit::Transmit(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::Transmit)
{
    ui->setupUi(this);
    //create window from ui object

    connect(ui->clearButton, SIGNAL(released()), this, SLOT(clear()));
    connect(ui->undoButton, SIGNAL(released()), this, SLOT(undo()));
    //link window buttons with functions - SLOT(...) [5]
}

Transmit::~Transmit()
{
    delete ui;
    //delete window
}

void Transmit::mousePressEvent(QMouseEvent *event) {
    if(wait) {
        qDebug() << "WAIT -- last data transmission incomplete";
    }
    else if(event->button() == Qt::LeftButton) { //if left mouse button clicked

        x_temp.push_back(event->x());
        y_temp.push_back(event->y());
        //add coordinates of mouse click to vectors
        pen_colour.push_back(colourVal);
        lineThick_temp.push_back(ui->lineThickBox->value()); 
        //add elements to set line colour and line thickness
		//[6] for how to get value from QSpinBox

        update();
        //update window

        PackData(event->x(), event->y());
        //ensure data of coordinates is sent to receive
    }
    else if(event->button() == Qt::RightButton) { //if right mouse button clicked

        x.push_back(x_temp);
        y.push_back(y_temp);
        penColour.push_back(pen_colour);
        lineThick.push_back(lineThick_temp);
        //add new row to double vectors of coresponding single vectors

        x_temp.clear();
        y_temp.clear();
        pen_colour.clear();
        lineThick_temp.clear();
        //clear single vectors

        instructionSend("Right");
    }
    else { //if anything else clicked
        clear();
        //clear screen
        //see clear() function definition

        instructionSend("Clear");
    }
}

void Transmit::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPen pen;

    unsigned int x1, y1, x2, y2, i, j;

    for(i = 0; i < x.size(); i++) {
        for(j = 1; j < x[i].size(); j++) {
            x1 = x[i][j-1];
            y1 = y[i][j-1];
            x2 = x[i][j];
            y2 = y[i][j];
            //take coordinates of line ends

            pen.setWidth(lineThick[i][j]);
            colourSet(penColour[i][j], pen);
            painter.setPen(pen);
            //set pen characteristics of line

            painter.drawLine(x1, y1, x2, y2);
            //draw line
        }
    } //loop through all elements of double vectors

    for(i = 1; i < x_temp.size(); i++) {
        x1 = x_temp[i-1];
        y1 = y_temp[i-1];
        x2 = x_temp[i];
        y2 = y_temp[i];
        //take coordinates of line ends

        pen.setWidth(lineThick_temp[i]);
        colourSet(pen_colour[i], pen);
        painter.setPen(pen);
        //set pen characteristics of line
        painter.drawLine(x1, y1, x2, y2);
        //draw line
    } //loop through all elements of single vectors
}

void Transmit::setWindow() {
    windowTitle();
    setWindowSize();
    //introduce x and y variables
    //to switch between overloaded functions
}

void Transmit::windowTitle() {
    setWindowTitle("Transmitter");
}

void Transmit::setWindowSize() {
    setFixedSize(400, 300);
}

void Transmit::setWindowSize(int x, int y) {
    setFixedSize(x, y);
}

void Transmit::deleteFeatures() {
    ui->clearButton->~QAbstractButton();
    ui->undoButton->~QAbstractButton();
    ui->menuBar->~QMenuBar();
    ui->lineThickBox->~QAbstractSpinBox();
    ui->label->~QLabel();
    //destroy all window features
}

void Transmit::clear() {
    x.clear();
    y.clear();
    x_temp.clear();
    y_temp.clear();
    pen_colour.clear();
    penColour.clear();
    lineThick_temp.clear();
    lineThick.clear();
    //clear all vectors

    update();
    //update screen with empty vectors so that screen is blank

    instructionSend("Clear");
}

void Transmit::colourSet(int col, QPen &pen) {
    switch(col) {
        case 0:
            pen.setColor(Qt::black);
            break;
        case 1:
            pen.setColor(Qt::blue);
            break;
        case 2:
            pen.setColor(Qt::red);
            break;
        case 3:
            pen.setColor(Qt::magenta);
            break;
        case 4:
            pen.setColor(Qt::darkGreen);
            break;
        case 5:
            pen.setColor(Qt::yellow);
            break;
        default:
            pen.setColor(Qt::black);
    } //set pen colour based on col value
}

void Transmit::setWait(bool val) {
    wait = val;
}

int Transmit::getXTempSize() {
    return x_temp.size();
}

/* COMMENT AT END OF on_action..._triggered() FUNCTIONS */
void Transmit::on_actionBlack_triggered()
{
    colourVal = 0;
    instructionSend("Black");
}

void Transmit::on_actionBlue_triggered()
{
    colourVal = 1;
    instructionSend("Blue");
}

void Transmit::on_actionRed_triggered()
{
    colourVal = 2;
    instructionSend("Red");
}

void Transmit::on_actionPurple_triggered()
{
    colourVal = 3;
    instructionSend("Magenta");
}

void Transmit::on_actionGreen_triggered()
{
    colourVal = 4;
    instructionSend("Green");
}

void Transmit::on_actionYellow_triggered()
{
    colourVal = 5;
    instructionSend("Yellow");
}
//changes value of colourVal based on selection from Window menu
//colourVal is used to set line colour

void Transmit::undo() {
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
    else if(x.size()) { //if single vector's are empty but double vectors aren't
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
    else { //if all vectors are empty
        qDebug() << "Nothing to undo.";
    }

    update();
    //update window to changes

    instructionSend("Undo");
}

void Transmit::ConvToBinX(int coord){
    x_bin.clear();

    for(int i = 0; coord > 0; i++){
        x_bin.push_back(coord % 2);
        coord = coord / 2;
    } //convert integer to binary vector
}

void Transmit::ConvToBinY(int coord){
    y_bin.clear();

    for(int i = 0; coord > 0; i++){
        y_bin.push_back(coord % 2);
        coord = coord / 2;
    } //convert integer to binary vector
}
//[7] for conversion method to binary

void Transmit::PackData(int x, int y){
    ConvToBinX(x);
    ConvToBinY(y);
    //see function definitions
    set_thickSend(ui->lineThickBox->value());
}

void Transmit::instructionSend(string ident) {
    if(ident == "Right") {
        instructBin = RIGHT;
    }
    else if(ident == "Clear") {
        instructBin = CLEAR;
    }
    else if(ident == "Undo") {
        instructBin = UNDO;
    }
    else if(ident == "Black") {
        instructBin = BLACK;
    }
    else if(ident == "Blue") {
        instructBin = BLUE;
    }
    else if(ident == "Red") {
        instructBin = RED;
    }
    else if(ident == "Magenta") {
        instructBin = MAGENTA;
    }
    else if(ident == "Green") {
        instructBin = GREEN;
    }
    else if(ident == "Yellow") {
        instructBin = YELLOW;
    }
    else {
        qDebug() << "Invalid input.";
    }
}
//instructBin[0] is used to signify a new instruction
//a 1 represents a new command to the thread

void Transmit::set_thickSend(const int t) {
    switch(t) {
        case 1:
            thickSend = ONE;
            break;
        case 2:
            thickSend = TWO;
            break;
        case 3:
            thickSend = THREE;
            break;
        case 4:
            thickSend = FOUR;
            break;
        default:
            qDebug() << "Invalid value.";
    }
}
//sets binary representation of each thickness
/* binary values are NOT EQUAL to value of t */
