#ifndef RECEIVE_H
#define RECEIVE_H

#include "transmit.h"

class Receive : public Transmit
{
public:
    Receive();
    void clear();
    void undo();

    void setWindow();
    void windowTitle();

    void unPackData(const vector<bool> x_in, const vector<bool> y_in, const vector<bool> thickness);
    void instructionReceive(const vector<bool> in);

protected:
    void paintEvent(QPaintEvent * event);
	
    void ConvToDecX(const vector<bool>x1);
    void ConvToDecY(const vector<bool>y1);

private:
    vector<vector<int>> x, y, penColour, lineThick;
    vector<int> x_temp, y_temp, pen_colour, lineThick_temp;
    int colourVal;
    //give colourVal default value

    void vectorTransfer();
    void setThickness(const vector<bool> thickness);
};

#endif // RECEIVE_H
