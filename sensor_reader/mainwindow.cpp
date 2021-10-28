#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "msg_struct.h"


#define         WINDOW_WIDTH                        1500
#define         WINDOW_HEIGTH                       600
#define         CHART_DIM                           300

/*
 *******************************************************************************
 *> Global variables
 *******************************************************************************
 */
static bool flag_conn = 0;          // true if the connection is established
QString serial_port_name = "";
static QPixmap *green;
static QPixmap *red;
static QPixmap *hue_wheel;
static uint8 resolution_val = VL53L5CX_RESOLUTION_64;
static uint8 frequency_val;
static uint8 sharpener_val;

/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MainWindow(QWidget *parent)
 *
 * \brief
 *
 * \n<b>Description</b>:\n
* 		Costruttore della classe MainWindow
 * \n   Rappresenta il main del CSCI. Vengono esegui nell'ordine:
 * \n   - inizializzazione variabili globali alla classe
 * \n   - creazione istanze classi serialconn
 * \n   - gestione eventi
 * \n<b>Parameters</b>:\n
 * 		QWidget *parent:    istanza della classe QWidget
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 22/09/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("SENSOR DATA RECEIVER");
    this->setMaximumWidth(WINDOW_WIDTH);
    this->setMaximumHeight(WINDOW_HEIGTH);
    this->setMinimumWidth(WINDOW_WIDTH);
    this->setMinimumHeight(WINDOW_HEIGTH);


    // Dichiarazione del widget principale
    receiver_widget = new QWidget(this);
    receiver_widget->setMaximumWidth(WINDOW_WIDTH);
    receiver_widget->setMaximumHeight(WINDOW_HEIGTH);
    receiver_widget->setMinimumWidth(WINDOW_WIDTH);
    receiver_widget->setMinimumHeight(WINDOW_HEIGTH);

    // Dichiarazione Send Keep Alive Message GroupBox e configurazione
    box_configuration = new QGroupBox("ToF Configuration:");
    mw_boxChartConfig();

    // Dichiarazione label di connessione
    lbl_serial_port = new QLabel("Serial port:");

    connect_button = new QPushButton("CONNECT");
    connect_button->setEnabled(false);
    connect_button->setMaximumWidth(100);
    connect_button->setMinimumWidth(100);
    connect_button->setMaximumHeight(30);
    connect_button->setMinimumHeight(30);

    combo_serial_port = new QComboBox;
    combo_serial_port->setMaximumWidth(100);
    combo_serial_port->setMinimumWidth(100);
    combo_serial_port->setMaximumHeight(30);
    combo_serial_port->setMinimumHeight(30);
    combo_serial_port->addItem(0,"");
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
            combo_serial_port->addItem("/dev/"+port.portName());
    }

    //layout di connessione
    layout_h_conn = new QHBoxLayout;
    layout_h_conn->addWidget(lbl_serial_port);
    layout_h_conn->addWidget(combo_serial_port);
    layout_h_conn->addWidget(connect_button);
    layout_h_conn->setAlignment(connect_button,Qt::AlignVCenter);
    layout_h_conn->addStretch();

    config_layout = new QVBoxLayout;

    // Dichiarazione Received Message GroupBox e configurazione
    box_init_msg = new QGroupBox("Sensors available");
    mw_boxInitMsgConfig();

    layout_hue = new QHBoxLayout;
    layout_hue->addWidget(box_init_msg);
    layout_hue->addWidget(hue_left);

    layout_complete = new QVBoxLayout();
    layout_complete->addWidget(box_configuration);
    layout_complete->addLayout(chart_layout);

    // Inserisco gli oggetti all interno del layout
    receiver_layout = new QGridLayout();
    receiver_layout->addLayout(layout_h_conn,0,0);
    //receiver_layout->addWidget(box_init_msg,1,0);
    receiver_layout->addLayout(layout_hue,1,0);
    receiver_layout->addLayout(layout_complete,1,1);

    //receiver_layout->addWidget(chartView_1,1,1);
    //receiver_layout->addWidget(chartView_2,1,2);
    //receiver_layout->addWidget(chartView_3,1,3);
    //receiver_layout->addWidget(chartView_4,1,4);
    //receiver_layout->addWidget(hue_left,2,1);
    //receiver_layout->addWidget(hue_right,2,2);

    // Inserisco il layout che ho formattato all interno del widget principale
    receiver_widget->setLayout(receiver_layout);

    // Dichiarazione SerialConn
    my_serial = new SerialConn();

    // Gestione dei click sul pulsante "connect" per la connessione alla porta seriale
    connect(this->connect_button, SIGNAL(clicked(bool)),this,SLOT(MW_connectBtnHandle()));
    connect(combo_serial_port, SIGNAL(currentIndexChanged(int)),this,SLOT(MW_ComboConnIndexChanged(int)));

    // // Gestione dei click sui checkButton per la scelta della configurazione manuale o automatica dei ToF
    connect(check_configuration_default,SIGNAL(stateChanged(int)),this,SLOT(MW_CheckConfigHandle(int)));

    // Gestione dei click sui radioButton per la scelta della configurazione dei tof
    connect(radio_resolution_4x4,SIGNAL(clicked()),this,SLOT(MW_4x4RadioBtnHandle()));
    connect(radio_resolution_8x8,SIGNAL(clicked()),this,SLOT(MW_8x8RadioBtnHandle()));

    // Gestione dei click sulle combobox della configurazione dei tof
    connect(combo_frequency, SIGNAL(currentIndexChanged(int)),this,SLOT(MW_frequencyIndexChanged(int)));
    connect(combo_sharpener, SIGNAL(currentIndexChanged(int)),this,SLOT(MW_SharpenerIndexChanged(int)));

    // Gestione del click sui pulsanti start e stop
    connect(start_button, SIGNAL(clicked(bool)),this,SLOT(MW_startBtnHandle()));
    connect(stop_button, SIGNAL(clicked(bool)),this,SLOT(MW_stopBtnHandle()));

    connect(my_serial, SIGNAL(SC_sendInitMsgInfo(VL53LX_INIT_MSG*)), this,
                                    SLOT(MW_receiveInitMsgInfo(VL53LX_INIT_MSG*)));
    connect(my_serial, SIGNAL(SC_sendData16MsgInfo(VL53LX_DATA_16_MSG*)), this,
                                    SLOT(MW_receiveData16MsgInfo(VL53LX_DATA_16_MSG*)));
    connect(my_serial, SIGNAL(SC_sendData64MsgInfo(VL53LX_DATA_64_MSG*)), this,
                                    SLOT(MW_receiveData64MsgInfo(VL53LX_DATA_64_MSG*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_connectBtnHandle()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce l'evento click del bottone
 *     	CONNECT sulla GUI
 *
 * \n<b>Parameters</b>:\n
 * 		Nessuno
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 22/09/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::MW_connectBtnHandle()
{

    if(flag_conn == 0)
    {
        //invio a SerialConn il comando di stabilire la connesione
        my_serial->SC_establishConn(serial_port_name);
        connect_button->setText("DISCONNECT");
        //qDebug() << "\n----------- CONNECTION ESTABLISHED ------------\n\n";
        flag_conn = 1;
        box_configuration->setEnabled(true);
        start_button->setEnabled(true);
    }

    else if(flag_conn == 1)
    {
        //invio a SerialConn il comando di stabilire la connesione
        my_serial->SC_dismissConn();
        connect_button->setText("CONNECT");
        //qDebug() << "\n----------- CONNECTION DISMISSED ------------\n\n";
        flag_conn = 0;
        box_configuration->setEnabled(false);
        start_button->setEnabled(false);

    }

}  //MW_connectBtnHandle()

/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_ComboConnIndexChanged(int index)
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce il cambiamento di indice per la
 *      combobox relativa al campo "Sonar Mode" del messaggio Keep Alive
 *
 * \n<b>Parameters</b>:\n
 * 		- int index: attuale indice della combobox;\n
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 *
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 04/01/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::MW_ComboConnIndexChanged(int index)
{

    if(index == 0)
        connect_button->setEnabled(false);

    else
        connect_button->setEnabled(true);

    serial_port_name = combo_serial_port->currentText();

} // MW_ComboConnIndexChanged()


/*!
 * *******************************************************************************
 *
 * \fn MainWindow::mw_boxInitMsgConfig()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione configura la groupBox di ricezione messaggi init
 *
 * \n<b>Parameters</b>:\n
 *
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 05/10/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::mw_boxInitMsgConfig()
{
    box_init_msg->setMinimumWidth(250);
    box_init_msg->setMaximumWidth(250);
    box_init_msg->setMinimumHeight(550);
    box_init_msg->setMaximumHeight(550);

    layout_init_msg = new QVBoxLayout();

    lbl_tof1 = new QLabel("ToF #1");
    lbl_tof2 = new QLabel("ToF #2");
    lbl_tof3 = new QLabel("ToF #3");
    lbl_tof4 = new QLabel("ToF #4");
    lbl_tof5 = new QLabel("ToF #5");
    lbl_tof6 = new QLabel("ToF #6");
    lbl_tof7 = new QLabel("ToF #7");
    lbl_tof8 = new QLabel("ToF #8");

    green = new QPixmap(":/images/green_dot.png");
    red = new QPixmap(":/images/red_dot.png");
    hue_wheel =new QPixmap(":/images/hsv_final.png");
    greenscaled = green->scaled(QSize(20,20));
    redscaled   = red->scaled(QSize(20,20));
    hue_wheel_scaled = hue_wheel->scaled(QSize(100,500));

    led_tof1 = new QLabel;
    led_tof2 = new QLabel;
    led_tof3 = new QLabel;
    led_tof4 = new QLabel;
    led_tof5 = new QLabel;
    led_tof6 = new QLabel;
    led_tof7 = new QLabel;
    led_tof8 = new QLabel;
    hue_left = new QLabel;
    hue_right = new QLabel;

    led_tof1->setPixmap(redscaled);
    led_tof2->setPixmap(redscaled);
    led_tof3->setPixmap(redscaled);
    led_tof4->setPixmap(redscaled);
    led_tof5->setPixmap(redscaled);
    led_tof6->setPixmap(redscaled);
    led_tof7->setPixmap(redscaled);
    led_tof8->setPixmap(redscaled);
    hue_left->setPixmap(hue_wheel_scaled);
    hue_left->setAlignment(Qt::AlignCenter);
    hue_right->setPixmap(hue_wheel_scaled);
    hue_right->setAlignment(Qt::AlignCenter);

    led_layout_tof1 = new QFormLayout();
    led_layout_tof2 = new QFormLayout();
    led_layout_tof3 = new QFormLayout();
    led_layout_tof4 = new QFormLayout();
    led_layout_tof5 = new QFormLayout();
    led_layout_tof6 = new QFormLayout();
    led_layout_tof7 = new QFormLayout();
    led_layout_tof8 = new QFormLayout();

    led_layout_tof1 -> addRow(led_tof1,lbl_tof1);
    led_layout_tof2 -> addRow(led_tof2,lbl_tof2);
    led_layout_tof3 -> addRow(led_tof3,lbl_tof3);
    led_layout_tof4 -> addRow(led_tof4,lbl_tof4);
    led_layout_tof5 -> addRow(led_tof5,lbl_tof5);
    led_layout_tof6 -> addRow(led_tof6,lbl_tof6);
    led_layout_tof7 -> addRow(led_tof7,lbl_tof7);
    led_layout_tof8 -> addRow(led_tof8,lbl_tof8);

    layout_init_msg ->addLayout(led_layout_tof1);
    layout_init_msg ->addLayout(led_layout_tof2);
    layout_init_msg ->addLayout(led_layout_tof3);
    layout_init_msg ->addLayout(led_layout_tof4);
    layout_init_msg ->addLayout(led_layout_tof5);
    layout_init_msg ->addLayout(led_layout_tof6);
    layout_init_msg ->addLayout(led_layout_tof7);
    layout_init_msg ->addLayout(led_layout_tof8);

    box_init_msg->setLayout(layout_init_msg);
    box_init_msg->setMinimumWidth(120);
    box_init_msg->setMaximumWidth(120);

}




/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_receiveInitMsgInfo()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce
 *
 * \n<b>Parameters</b>:\n
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 05/10/2021
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::MW_receiveInitMsgInfo(VL53LX_INIT_MSG *rcv_msg_init)
{
    uint8 msg_id = static_cast<uint8>(rcv_msg_init->header.msg_id);
    uint8 tof_id = static_cast<uint8>(rcv_msg_init->tof_id);
    uint8 tof_status = static_cast<uint8>(rcv_msg_init->tof_status);



    if(msg_id == ID_VL53LX_INIT_MSG)
    {
        switch(tof_id)
        {
        case 1:

            if(tof_status == 0)
            {
                led_tof1->setPixmap(redscaled);
            }
            else if(tof_status == 1)
            {
                led_tof1->setPixmap(greenscaled);
            }
            else
            {
                qDebug() << "WRONG ToF STATUS";
                led_tof1->setPixmap(redscaled);
            }

            break;

        case 2:

            if(tof_status == 0)
                led_tof2->setPixmap(redscaled);
            else if(tof_status == 1)
                led_tof2->setPixmap(greenscaled);
            else
            {
                qDebug() << "WRONG ToF STATUS";
                led_tof2->setPixmap(redscaled);
            }

            break;

        case 3:

            if(tof_status == 0)
                led_tof3->setPixmap(redscaled);
            else if(tof_status == 1)
                led_tof3->setPixmap(greenscaled);
            else
            {
                qDebug() << "WRONG ToF STATUS";
                led_tof3->setPixmap(redscaled);
            }

            break;

        case 4:

            if(tof_status == 0)
                led_tof4->setPixmap(redscaled);
            else if(tof_status == 1)
                led_tof4->setPixmap(greenscaled);
            else
            {
                qDebug() << "WRONG ToF STATUS";
                led_tof4->setPixmap(redscaled);
            }

            break;

        case 5:

            if(tof_status == 0)
                led_tof5->setPixmap(redscaled);
            else if(tof_status == 1)
                led_tof5->setPixmap(greenscaled);
            else
            {
                qDebug() << "WRONG ToF STATUS";
                led_tof5->setPixmap(redscaled);
            }

            break;

        case 6:

            if(tof_status == 0)
                led_tof6->setPixmap(redscaled);
            else if(tof_status == 1)
                led_tof6->setPixmap(greenscaled);
            else
            {
                qDebug() << "WRONG ToF STATUS";
                led_tof6->setPixmap(redscaled);
            }

            break;

        case 7:

            if(tof_status == 0)
                led_tof7->setPixmap(redscaled);
            else if(tof_status == 1)
                led_tof7->setPixmap(greenscaled);
            else
            {
                qDebug() << "WRONG ToF STATUS";
                led_tof7->setPixmap(redscaled);
            }

            break;

        case 8:

            if(tof_status == 0)
                led_tof8->setPixmap(redscaled);
            else if(tof_status == 1)
                led_tof8->setPixmap(greenscaled);
            else
            {
                qDebug() << "WRONG ToF STATUS";
                led_tof8->setPixmap(redscaled);
            }

            break;
        }
    }
} //


/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_receiveInitMsgInfo()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce
 *
 * \n<b>Parameters</b>:\n
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 05/10/2021
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::MW_receiveData16MsgInfo(VL53LX_DATA_16_MSG *rcv_msg_data16)
{
    uint8 msg_id = static_cast<uint8>(rcv_msg_data16->header.msg_id);
    uint8 tof_id = static_cast<uint8>(rcv_msg_data16->id_tof);
    uint16 dist;
    uint8 status;
    uint8 j = 0;
    QColor color_dist;

    if(msg_id == ID_VL53LX_DATA_MSG)
    {
        stop_button->setEnabled(true);

        switch(tof_id)
        {

        case 1:
                j = 0;
                for (int i = 0; i < 16; i++ )
                {
                    dist = static_cast<uint16>(rcv_msg_data16->object_data[i].range_val);
                    status = static_cast<uint8>(rcv_msg_data16->object_data[i].range_status);
                    color_dist = GetColorFromDist(dist,status);
                    areas_1[(2*i) + j]->setBrush(color_dist);
                    areas_1[((2*i)+1) + j]->setBrush(color_dist);
                    areas_1[((2*i)+8) + j]->setBrush(color_dist);
                    areas_1[((2*i)+9) + j]->setBrush(color_dist);

                    if(i == 3 || i == 7 || i == 11)
                        j += 8;
                }

            break;

        case 2:
                j = 0;
                for (int i = 0; i < 16; i++ )
                {
                    dist = static_cast<uint16>(rcv_msg_data16->object_data[i].range_val);
                    status = static_cast<uint8>(rcv_msg_data16->object_data[i].range_status);
                    color_dist = GetColorFromDist(dist,status);
                    areas_2[(2*i) + j]->setBrush(color_dist);
                    areas_2[((2*i)+1) + j]->setBrush(color_dist);
                    areas_2[((2*i)+8) + j]->setBrush(color_dist);
                    areas_2[((2*i)+9) + j]->setBrush(color_dist);

                    if(i == 3 || i == 7 || i == 11)
                        j += 8;
                } 
            break;

        case 3:
                j = 0;
                for (int i = 0; i < 16; i++ )
                {
                    dist = static_cast<uint16>(rcv_msg_data16->object_data[i].range_val);
                    status = static_cast<uint8>(rcv_msg_data16->object_data[i].range_status);
                    color_dist = GetColorFromDist(dist,status);
                    areas_3[(2*i) + j]->setBrush(color_dist);
                    areas_3[((2*i)+1) + j]->setBrush(color_dist);
                    areas_3[((2*i)+8) + j]->setBrush(color_dist);
                    areas_3[((2*i)+9) + j]->setBrush(color_dist);

                    if(i == 3 || i == 7 || i == 11)
                        j += 8;
                }

            break;

        case 4:
                j = 0;
                for (int i = 0; i < 16; i++ )
                {
                    dist = static_cast<uint16>(rcv_msg_data16->object_data[i].range_val);
                    status = static_cast<uint8>(rcv_msg_data16->object_data[i].range_status);
                    color_dist = GetColorFromDist(dist,status);
                    areas_4[(2*i) + j]->setBrush(color_dist);
                    areas_4[((2*i)+1) + j]->setBrush(color_dist);
                    areas_4[((2*i)+8) + j]->setBrush(color_dist);
                    areas_4[((2*i)+9) + j]->setBrush(color_dist);

                    if(i == 3 || i == 7 || i == 11)
                        j += 8;
                }

            break;
        }
    }
}

/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_receiveInitMsgInfo()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce
 *
 * \n<b>Parameters</b>:\n
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 05/10/2021
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::MW_receiveData64MsgInfo(VL53LX_DATA_64_MSG *rcv_msg_data64)
{
    uint8 msg_id = static_cast<uint8>(rcv_msg_data64->header.msg_id);
    uint8 tof_id = static_cast<uint8>(rcv_msg_data64->id_tof);
    uint16 dist;
    uint8 status;
    QColor color_dist;

    if(msg_id == ID_VL53LX_DATA_MSG)
    {
        stop_button->setEnabled(true);

        switch(tof_id)
        {

        case 1:

                for (int i = 0; i < 64; i++ )
                {
                    dist = static_cast<uint16>(rcv_msg_data64->object_data[i].range_val);
                    status = static_cast<uint8>(rcv_msg_data64->object_data[i].range_status);
                    //qDebug() << dist << "\n";
                    color_dist = GetColorFromDist(dist,status);
                    areas_1[i]->setBrush(color_dist);
                }

            break;

        case 2:

                for (int i = 0; i < 64; i++ )
                {
                    dist = static_cast<uint16>(rcv_msg_data64->object_data[i].range_val);
                    status = static_cast<uint8>(rcv_msg_data64->object_data[i].range_status);
                    //qDebug() << dist << "\n";
                    color_dist = GetColorFromDist(dist,status);
                    areas_2[i]->setBrush(color_dist);
                }

            break;

        case 3:

                for (int i = 0; i < 64; i++ )
                {
                    dist = static_cast<uint16>(rcv_msg_data64->object_data[i].range_val);
                    status = static_cast<uint8>(rcv_msg_data64->object_data[i].range_status);
                    //qDebug() << dist << "\n";
                    color_dist = GetColorFromDist(dist,status);
                    areas_3[i]->setBrush(color_dist);
                }

            break;

        case 4:

                for (int i = 0; i < 64; i++ )
                {
                    dist = static_cast<uint16>(rcv_msg_data64->object_data[i].range_val);
                    status = static_cast<uint8>(rcv_msg_data64->object_data[i].range_status);
                    //qDebug() << dist << "\n";
                    color_dist = GetColorFromDist(dist,status);
                    areas_4[i]->setBrush(color_dist);
                }

            break;
        }

    }
}



/*!
 * *******************************************************************************
 *
 * \fn MainWindow::mw_boxInitMsgConfig()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione configura la groupBox di ricezione messaggi init
 *
 * \n<b>Parameters</b>:\n
 *
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 05/10/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

QColor MainWindow::GetColorFromDist(uint16 dist, uint8 status)
{
    QColor color_dist;
    uint16 value;

    if(status == 255)
    {
        color_dist.setRgb(0,0,0,255);
        return color_dist;
    }

    if(dist > 2500)
    {
        color_dist.setRgb(0,0,0,255);
    }
    if(dist > 2000 && dist <= 2500)
    {
        value = 255 - (((dist-2000)*255)/500);
        color_dist.setRgb(0,0,value,255);
    }
    else if(dist > 1500 && dist <= 2000)
    {
        value = 255 - (((dist-1500)*255)/500);
        color_dist.setRgb(0,value,255,255);
    }
    else if(dist > 1000 && dist <= 1500)
    {
        value = (((dist-1000)*255)/500);
        color_dist.setRgb(0,255,value,255);
    }
    else if(dist > 500 && dist <= 1000)
    {
        value = 255 - (((dist-500)*255)/500);
        color_dist.setRgb(value,255,0,255);
    }
    else if(dist > 0 && dist <= 500)
    {
        value = ((dist*255)/500);
        color_dist.setRgb(255,value,0,255);
    }

    return color_dist;
}


/*!
 * *******************************************************************************
 *
 * \fn MainWindow::mw_boxInitMsgConfig()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione configura la groupBox di ricezione messaggi init
 *
 * \n<b>Parameters</b>:\n
 *
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 05/10/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */
void MainWindow::mw_boxChartConfig()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            seriesA = new QLineSeries();
            *seriesA << QPointF(j, i) << QPointF(j+1, i);
            seriesA_1[(8*i)+j] = seriesA;
            seriesA_2[(8*i)+j] = seriesA;
            seriesA_3[(8*i)+j] = seriesA;
            seriesA_4[(8*i)+j] = seriesA;
        }
    }

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            seriesB = new QLineSeries();
            *seriesB << QPointF(j, i+1) << QPointF(j+1, i+1);
            seriesB_1[(8*i)+j] = seriesB;
            seriesB_2[(8*i)+j] = seriesB;
            seriesB_3[(8*i)+j] = seriesB;
            seriesB_4[(8*i)+j] = seriesB;
        }
    }
    QPen pen(0x059605);
    pen.setWidth(1);
    QColor color(Qt::darkBlue);

    for(int i = 0; i < 64; i++)
    {
        area = new QAreaSeries(seriesA_1[i],seriesB_1[i]);
        area->setBrush(color);
        area->setPen(pen);

        areas_1[i] = area;
    }

    for(int i = 0; i < 64; i++)
    {
        area = new QAreaSeries(seriesA_2[i],seriesB_2[i]);
        area->setBrush(color);
        area->setPen(pen);

        areas_2[i] = area;
    }

    for(int i = 0; i < 64; i++)
    {
        area = new QAreaSeries(seriesA_3[i],seriesB_3[i]);
        area->setBrush(color);
        area->setPen(pen);

        areas_3[i] = area;
    }

    for(int i = 0; i < 64; i++)
    {
        area = new QAreaSeries(seriesA_4[i],seriesB_4[i]);
        area->setBrush(color);
        area->setPen(pen);

        areas_4[i] = area;
    }


    chart_1 = new QChart();
    for(int i = 0; i < 64; i++)
    {
        chart_1->addSeries(areas_1[i]);
    }

    chart_2 = new QChart();
    for(int i = 0; i < 64; i++)
    {
        chart_2->addSeries(areas_2[i]);
    }

    chart_3 = new QChart();
    for(int i = 0; i < 64; i++)
    {
        chart_3->addSeries(areas_3[i]);
    }

    chart_4 = new QChart();
    for(int i = 0; i < 64; i++)
    {
        chart_4->addSeries(areas_4[i]);
    }

    chart_1->setTitle("TOF 1");
    chart_1->legend()->hide();
    chart_1->createDefaultAxes();
    chart_1->axes(Qt::Horizontal).first()->setRange(0, 8);
    chart_1->axes(Qt::Vertical).first()->setRange(0, 8);


    chart_2->setTitle("TOF 2");
    chart_2->legend()->hide();
    chart_2->createDefaultAxes();
    chart_2->axes(Qt::Horizontal).first()->setRange(0, 8);
    chart_2->axes(Qt::Vertical).first()->setRange(0, 8);

    chart_3->setTitle("TOF 3");
    chart_3->legend()->hide();
    chart_3->createDefaultAxes();
    chart_3->axes(Qt::Horizontal).first()->setRange(0, 8);
    chart_3->axes(Qt::Vertical).first()->setRange(0, 8);

    chart_4->setTitle("TOF 4");
    chart_4->legend()->hide();
    chart_4->createDefaultAxes();
    chart_4->axes(Qt::Horizontal).first()->setRange(0, 8);
    chart_4->axes(Qt::Vertical).first()->setRange(0, 8);

    chartView_1 = new QChartView(chart_1);
    chartView_1->setRenderHint(QPainter::Antialiasing);
    chartView_1->setMinimumSize(CHART_DIM,CHART_DIM);
    chartView_1->setMaximumSize(CHART_DIM,CHART_DIM);

    chartView_2 = new QChartView(chart_2);
    chartView_2->setRenderHint(QPainter::Antialiasing);
    chartView_2->setMinimumSize(CHART_DIM,CHART_DIM);
    chartView_2->setMaximumSize(CHART_DIM,CHART_DIM);

    chartView_3 = new QChartView(chart_3);
    chartView_3->setRenderHint(QPainter::Antialiasing);
    chartView_3->setMinimumSize(CHART_DIM,CHART_DIM);
    chartView_3->setMaximumSize(CHART_DIM,CHART_DIM);

    chartView_4 = new QChartView(chart_4);
    chartView_4->setRenderHint(QPainter::Antialiasing);
    chartView_4->setMinimumSize(CHART_DIM,CHART_DIM);
    chartView_4->setMaximumSize(CHART_DIM,CHART_DIM);

    chart_layout = new QHBoxLayout;
    chart_layout->addWidget(chartView_4);
    chart_layout->addWidget(chartView_3);
    chart_layout->addWidget(chartView_2);
    chart_layout->addWidget(chartView_1);

    check_configuration_default = new QCheckBox("Default configuration");
    check_configuration_default->setChecked(true);

    box_resolution = new QGroupBox("Resolution");
    box_resolution->setMaximumHeight(115);
    box_resolution->setMinimumHeight(115);
    box_frequency = new QGroupBox("Frequency (Hz)");
    box_frequency->setMaximumHeight(115);
    box_frequency->setMinimumHeight(115);
    box_sharpener = new QGroupBox("Sharpener (%)");
    box_sharpener->setMaximumHeight(115);
    box_sharpener->setMinimumHeight(115);

    radio_resolution_8x8 = new QRadioButton("8x8 configuration");
    radio_resolution_4x4 = new QRadioButton("4x4 configuration");

    QStringList sequence_len_freq = QStringList()    << tr("0") << tr("1") << tr("2") << tr("3") << tr("4") << tr("5") << tr("6") << tr("7") << tr("8") << tr("9")
                                            << tr("10") << tr("11")<< tr("12") << tr("13") << tr("14") << tr("15") << tr("16") << tr("17")<< tr("18") << tr("19")
                                            << tr("20") << tr("21")<< tr("22") << tr("23") << tr("24") << tr("25") << tr("26") << tr("27")<< tr("28") << tr("29")
                                            << tr("30") << tr("31")<< tr("33") << tr("33") << tr("34") << tr("35") << tr("36") << tr("37")<< tr("38") << tr("39")
                                            << tr("40") << tr("41")<< tr("44") << tr("43") << tr("44") << tr("45") << tr("46") << tr("47")<< tr("48") << tr("49")
                                            << tr("50") << tr("51")<< tr("55") << tr("53") << tr("54") << tr("55") << tr("56") << tr("57")<< tr("58") << tr("59")
                                            << tr("60");

    QStringList sequence_len_sharp = QStringList()    << tr("0") << tr("1") << tr("2") << tr("3") << tr("4") << tr("5") << tr("6") << tr("7") << tr("8") << tr("9")
                                            << tr("10") << tr("11")<< tr("12") << tr("13") << tr("14") << tr("15") << tr("16") << tr("17")<< tr("18") << tr("19")
                                            << tr("20") << tr("21")<< tr("22") << tr("23") << tr("24") << tr("25") << tr("26") << tr("27")<< tr("28") << tr("29")
                                            << tr("30") << tr("31")<< tr("32") << tr("33") << tr("34") << tr("35") << tr("36") << tr("37")<< tr("38") << tr("39")
                                            << tr("40") << tr("41")<< tr("42") << tr("43") << tr("44") << tr("45") << tr("46") << tr("47")<< tr("48") << tr("49")
                                            << tr("50") << tr("51")<< tr("52") << tr("53") << tr("54") << tr("55") << tr("56") << tr("57")<< tr("58") << tr("59")
                                            << tr("60") << tr("61")<< tr("62") << tr("63") << tr("64") << tr("65") << tr("66") << tr("67")<< tr("68") << tr("69")
                                            << tr("70") << tr("71")<< tr("72") << tr("73") << tr("74") << tr("75") << tr("76") << tr("77")<< tr("78") << tr("79")
                                            << tr("80") << tr("81")<< tr("82") << tr("83") << tr("84") << tr("85") << tr("86") << tr("87")<< tr("88") << tr("89")
                                            << tr("90") << tr("91")<< tr("92") << tr("93") << tr("94") << tr("95") << tr("96") << tr("97")<< tr("98") << tr("99")
                                            << tr("100");

    combo_frequency = new QComboBox;
    combo_frequency->insertItems(0, sequence_len_freq);

    lbl_warning_frequency = new QLabel("Warning: with 8x8 resolution\nmax frequency 15 Hz");
    combo_sharpener = new QComboBox;
    combo_sharpener->insertItems(0, sequence_len_sharp);


    start_button = new QPushButton("START");
    start_button->setEnabled(false);
    start_button->setMaximumWidth(120);
    start_button->setMinimumWidth(120);
    start_button->setMaximumHeight(50);
    start_button->setMinimumHeight(50);
    stop_button = new QPushButton("STOP");
    stop_button->setEnabled(false);
    stop_button->setMaximumWidth(120);
    stop_button->setMinimumWidth(120);
    stop_button->setMaximumHeight(50);
    stop_button->setMinimumHeight(50);

    mw_initDefaultConfig();

    layout_resolution = new QFormLayout();
    layout_resolution->addWidget(radio_resolution_8x8);
    layout_resolution->addWidget(radio_resolution_4x4);
    box_resolution->setLayout(layout_resolution);

    layout_frequency = new QFormLayout();
    layout_frequency->addWidget(combo_frequency);
    layout_frequency->addWidget(lbl_warning_frequency);
    box_frequency->setLayout(layout_frequency);

    layout_sharpener = new QFormLayout();
    layout_sharpener->addWidget(combo_sharpener);
    box_sharpener->setLayout(layout_sharpener);

    layout_configuration = new QHBoxLayout();
    layout_configuration->addWidget(box_resolution);
    layout_configuration->addWidget(box_frequency);
    layout_configuration->addWidget(box_sharpener);

    layout_btn_configuration = new QVBoxLayout();
    layout_btn_configuration->addWidget(start_button);
    layout_btn_configuration->addWidget(stop_button);

    grid_layout_config = new QGridLayout();
    grid_layout_config->addWidget(check_configuration_default,0,0);
    grid_layout_config->addLayout(layout_configuration,1,0);
    grid_layout_config->addLayout(layout_btn_configuration,1,1);

    box_configuration->setLayout(grid_layout_config);
    box_configuration->setMaximumHeight(200);
    box_configuration->setMinimumHeight(200);
    //box_configuration->setEnabled(false);
}

/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_CheckConfigHandle()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce il cambiamento di stato della check box per
 *      l'inizializzazione dei tof
 *
 * \n<b>Parameters</b>:\n
 * 		- int check_state: state of the check box (unchecked, partially checked or checked);\n
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 *
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 28/01/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */



void MainWindow::MW_CheckConfigHandle(int check_state)
{

    // se la checkBox è disabilitata -> abilito il pulsante per l'invio del messaggio manuale
    if (check_state == Qt::Unchecked)
    {
        box_resolution->setEnabled(true);
        box_frequency->setEnabled(true);
        box_sharpener->setEnabled(true);
    }

    else if(check_state == Qt::Checked)
    {
        mw_initDefaultConfig();
    }
}


/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_CheckConfigHandle()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce il cambiamento di stato della check box per
 *      l'inizializzazione dei tof
 *
 * \n<b>Parameters</b>:\n
 * 		- int check_state: state of the check box (unchecked, partially checked or checked);\n
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 *
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 28/01/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::mw_initDefaultConfig()
{
    box_resolution->setEnabled(false);
    box_frequency->setEnabled(false);
    box_sharpener->setEnabled(false);
    radio_resolution_8x8->setChecked(true);
    radio_resolution_4x4->setChecked(false);
    combo_frequency->setCurrentIndex(15);
     combo_sharpener->setCurrentIndex(20);
}


/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_4x4RadioBtnHandle()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce il click del radio button per
 *      la configurazione a 16 zone (4x4)
 *
 * \n<b>Parameters</b>:\n
 * 		Nessuno
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 *
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 28/01/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::MW_4x4RadioBtnHandle()
{
    lbl_warning_frequency->setHidden(true);
}   // MW_4x4RadioBtnHandle()


/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_8x8RadioBtnHandle()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce il click del radio button per
 *      la configurazione a 64 zone (8x8)
 *
 * \n<b>Parameters</b>:\n
 * 		Nessuno
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 *
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 28/01/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::MW_8x8RadioBtnHandle()
{
    lbl_warning_frequency->setHidden(false);

    if(combo_frequency->currentIndex() > 15)
        combo_frequency->setCurrentIndex(15);
}   // MW_8x8RadioBtnHandle()

/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_frequencyIndexChanged(int index)
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce il click del radio button per
 *      la configurazione a 64 zone (8x8)
 *
 * \n<b>Parameters</b>:\n
 * 		Nessuno
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 *
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 28/01/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::MW_frequencyIndexChanged(int index)
{
    if(radio_resolution_8x8->isChecked())
    {
        if(index > 15)
            combo_frequency->setCurrentIndex(15);
    }
}   // MW_frequencyIndexChanged(int index)

/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_sharpenerIndexChanged(int index)
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce il click del radio button per
 *      la configurazione a 64 zone (8x8)
 *
 * \n<b>Parameters</b>:\n
 * 		Nessuno
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 *
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 28/01/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::MW_sharpenerIndexChanged(int index)
{

}   // MW_sharpenerIndexChanged(int index)



/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_startBtnHandle()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce l'evento click del bottone
 *     	START sulla GUI per l'avvio dei tof
 *
 * \n<b>Parameters</b>:\n
 * 		Nessuno
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 22/09/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::MW_startBtnHandle()
{

    start_button->setEnabled(false);
    stop_button->setEnabled(false);

    if(radio_resolution_4x4->isChecked())
        resolution_val = VL53L5CX_RESOLUTION_16;
    else if(radio_resolution_8x8->isChecked())
        resolution_val = VL53L5CX_RESOLUTION_64;

    frequency_val = combo_frequency->currentIndex();
    sharpener_val = combo_sharpener->currentIndex();

    my_serial->SC_MsgStartHandle(resolution_val, frequency_val, sharpener_val);
    my_serial->SC_SendStart();

}  //MW_startBtnHandle()

/*!
 * *******************************************************************************
 *
 * \fn MainWindow::MW_stopBtnHandle()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce l'evento click del bottone
 *     	STOP sulla GUI per l'avvio dei tof
 *
 * \n<b>Parameters</b>:\n
 * 		Nessuno
 *
 * \n<b>Returns</b>:\n
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 22/09/2021
 * \n- Author: Frnacesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void MainWindow::MW_stopBtnHandle()
{
    start_button->setEnabled(false);
    stop_button->setEnabled(false);

    my_serial->SC_SendStop();

}  //MW_stopBtnHandle()
