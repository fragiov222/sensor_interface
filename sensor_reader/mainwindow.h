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

    // Sensor data receiver Connection widgets
    QFormLayout *connection_layout;
    QLabel *lbl_serial_port;
    QComboBox *combo_serial_port;
    QPushButton *connect_button;
    QFormLayout *table_layout;


};
#endif // MAINWINDOW_H
