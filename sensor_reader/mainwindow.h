#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include "QPixmap"
#include <QString>
#include <QImage>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "serialconn.h"
#include <QChartView>
#include <QLineSeries>
#include <QAreaSeries>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace QtCharts;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void MW_connectBtnHandle();
    void MW_ComboConnIndexChanged(int index);
    void MW_receiveInitMsgInfo(VL53LX_INIT_MSG *rcv_msg_init);
    void MW_receiveDataMsgInfo(VL53LX_DATA_MSG *rcv_msg_data);

private:
    Ui::MainWindow *ui;

    // Layout
    QGridLayout *receiver_layout;
    QWidget *receiver_widget;
    QHBoxLayout *layout_h_conn;
    QHBoxLayout *data_layout;
    QVBoxLayout *layout_v_sensors;

    // Serial connection
    SerialConn *my_serial;

    //Charts
    QChartView *chartView_left;
    QChartView *chartView_right;

    QLineSeries *series0_left[64];
    QLineSeries *series1_left[64];
    QLineSeries *series0_right[64];
    QLineSeries *series1_right[64];
    QLineSeries *series;

    QAreaSeries *areas_left[64];
    QAreaSeries *areas_right[64];
    QAreaSeries *area;

    QChart *chart_left;
    QChart *chart_right;


    // Connection widgets
    QFormLayout *connection_layout;
    QLabel *lbl_serial_port;
    QComboBox *combo_serial_port;
    QPushButton *connect_button;
    QFormLayout *table_layout;

    // Initialization GroupBox
    QGroupBox *box_init_msg;
    QVBoxLayout *layout_init_msg;
    QPixmap redscaled;
    QPixmap greenscaled;

    QLabel *led_tof1;
    QLabel *led_tof2;
    QLabel *led_tof3;
    QLabel *led_tof4;
    QLabel *led_tof5;
    QLabel *led_tof6;
    QLabel *led_tof7;
    QLabel *led_tof8;

    QLabel *lbl_tof1;
    QLabel *lbl_tof2;
    QLabel *lbl_tof3;
    QLabel *lbl_tof4;
    QLabel *lbl_tof5;
    QLabel *lbl_tof6;
    QLabel *lbl_tof7;
    QLabel *lbl_tof8;

    QFormLayout *led_layout_tof1;
    QFormLayout *led_layout_tof2;
    QFormLayout *led_layout_tof3;
    QFormLayout *led_layout_tof4;
    QFormLayout *led_layout_tof5;
    QFormLayout *led_layout_tof6;
    QFormLayout *led_layout_tof7;
    QFormLayout *led_layout_tof8;

    void mw_boxInitMsgConfig();
    QColor GetColorFromDist(uint16);

};
#endif // MAINWINDOW_H
