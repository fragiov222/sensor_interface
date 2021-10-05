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

#include "serialconn.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

};
#endif // MAINWINDOW_H
