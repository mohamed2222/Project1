#ifndef TRANSMIT
#define TRANSMIT

#include <QColor>
#include <QPen>
#include <QWidget>
#include <QMainWindow>
#include <QtGui>
//#include <QtCore>
#include <QMouseEvent>
#include <QPainter>
#include <QAbstractButton>

using std::vector;
using std::string;

namespace Ui {
class Transmit;
class Receive;
}

class Transmit : public QMainWindow
{
    Q_OBJECT

public:
    explicit Transmit(QWidget *parent = 0);
    ~Transmit();

    void setWindow();
    void windowTitle();
    void setWindowSize();
    void setWindowSize(int x, int y);

    void deleteFeatures();
    void colourSet(int col, QPen &pen);
	
    void setWait(bool val);
    int getXTempSize();

    vector<bool>x_bin, y_bin, thickSend;
    vector<bool>instructBin = {0};
    //Give instructBin default value

public slots:
    void clear();
    void undo();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
	
	void PackData(int x, int y);
    void ConvToBinX(int coord);
    void ConvToBinY(int coord);

private slots:
    void on_actionBlack_triggered();
    void on_actionBlue_triggered();
    void on_actionRed_triggered();
    void on_actionPurple_triggered();
    void on_actionGreen_triggered();
    void on_actionYellow_triggered();

private:
    vector<int> x_temp, y_temp, pen_colour, lineThick_temp;
    vector<vector<int>>x, y, penColour, lineThick;
    int colourVal = 0;
    bool wait = 0;
    //give colourVal and wait default values

    void instructionSend(string ident);
    void set_thickSend(const int t);

    Ui::Transmit *ui;
};

#endif
