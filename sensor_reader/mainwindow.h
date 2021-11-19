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
#include <QRadioButton>
#include <QCheckBox>
#include <QTimer>

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
    void MW_TmrHandle();
    void MW_saveBtnHandle();

    void MW_CheckConfigHandle(int check_state);

    void MW_4x4RadioBtnHandle();
    void MW_8x8RadioBtnHandle();

    void MW_frequencyIndexChanged(int index);
    void MW_sharpenerIndexChanged(int index);

    void MW_startBtnHandle();
    void MW_stopBtnHandle();

    void MW_receiveInitMsgInfo(VL53LX_INIT_MSG *rcv_msg_init);
    void MW_receiveData16MsgInfo(VL53LX_DATA_16_MSG *rcv_msg_data16);
    void MW_receiveData64MsgInfo(VL53LX_DATA_64_MSG *rcv_msg_data64);

private:
    Ui::MainWindow *ui;

    // Layout
    QGridLayout *receiver_layout;
    QWidget *receiver_widget;
    QHBoxLayout *layout_h_conn;
    QVBoxLayout *config_layout;
    QVBoxLayout *layout_v_sensors;
    QHBoxLayout * chart_layout_row1;
    QHBoxLayout * chart_layout_row2;
    QHBoxLayout *layout_hue;


    // Serial connection
    SerialConn *my_serial;

    //Charts
    QChartView *chartView_1;
    QChartView *chartView_2;
    QChartView *chartView_3;
    QChartView *chartView_4;
    QChartView *chartView_5;
    QChartView *chartView_6;
    QChartView *chartView_7;
    QChartView *chartView_8;

    QLineSeries *seriesA_1[64];
    QLineSeries *seriesB_1[64];
    QLineSeries *seriesA_2[64];
    QLineSeries *seriesB_2[64];
    QLineSeries *seriesA_3[64];
    QLineSeries *seriesB_3[64];
    QLineSeries *seriesA_4[64];
    QLineSeries *seriesB_4[64];
    QLineSeries *seriesA_5[64];
    QLineSeries *seriesB_5[64];
    QLineSeries *seriesA_6[64];
    QLineSeries *seriesB_6[64];
    QLineSeries *seriesA_7[64];
    QLineSeries *seriesB_7[64];
    QLineSeries *seriesA_8[64];
    QLineSeries *seriesB_8[64];
    QLineSeries *seriesA;
    QLineSeries *seriesB;

    QAreaSeries *areas_1[64];
    QAreaSeries *areas_2[64];
    QAreaSeries *areas_3[64];
    QAreaSeries *areas_4[64];
    QAreaSeries *areas_5[64];
    QAreaSeries *areas_6[64];
    QAreaSeries *areas_7[64];
    QAreaSeries *areas_8[64];
    QAreaSeries *area;

    QChart *chart_1;
    QChart *chart_2;
    QChart *chart_3;
    QChart *chart_4;
    QChart *chart_5;
    QChart *chart_6;
    QChart *chart_7;
    QChart *chart_8;

    // Connection widgets
    QFormLayout *connection_layout;
    QLabel *lbl_serial_port;
    QComboBox *combo_serial_port;
    QPushButton *connect_button;
    QFormLayout *table_layout;

    QTimer * tmr_acquisition;
    QPushButton *save_csv_button;
    QString textData;

    // Initialization GroupBox
    QGroupBox *box_init_msg;
    QVBoxLayout *layout_init_msg;
    QPixmap redscaled;
    QPixmap greenscaled;
    QPixmap hue_wheel_scaled;

    QLabel *led_tof1;
    QLabel *led_tof2;
    QLabel *led_tof3;
    QLabel *led_tof4;
    QLabel *led_tof5;
    QLabel *led_tof6;
    QLabel *led_tof7;
    QLabel *led_tof8;
    QLabel *hue_left;
    QLabel *hue_right;

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

    // Configuration GroupBox
    QGroupBox *box_configuration;
    QGridLayout *grid_layout_config;
    QCheckBox *check_configuration_default;
    QHBoxLayout *layout_configuration;
    QVBoxLayout *layout_btn_configuration;
    QVBoxLayout *layout_complete;
    QGroupBox *box_resolution;
    QGroupBox *box_frequency;
    QGroupBox *box_sharpener;
    QRadioButton *radio_resolution_8x8;
    QRadioButton *radio_resolution_4x4;
    QComboBox *combo_frequency;
    QLabel * lbl_warning_frequency;
    QComboBox *combo_sharpener;
    QPushButton *start_button;
    QPushButton *stop_button;
    QFormLayout *layout_resolution;
    QFormLayout *layout_frequency;
    QFormLayout *layout_sharpener;


    void mw_boxInitMsgConfig();
    void mw_boxChartConfig();
    void mw_initDefaultConfig();
    QColor GetColorFromDist(uint16, uint8);

};
#endif // MAINWINDOW_H
