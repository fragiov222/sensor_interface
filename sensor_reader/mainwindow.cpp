#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "msg_struct.h"


#define         WINDOW_WIDTH                        1200
#define         WINDOW_HEIGTH                       600

/*
 *******************************************************************************
 *> Global variables
 *******************************************************************************
 */
static bool flag_conn = 0;          // true if the connection is established
QString serial_port_name = "";
static QPixmap *green;
static QPixmap *red;

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

    // Dichiarazione label di connessione
    lbl_serial_port = new QLabel("Serial port:");

    connect_button = new QPushButton("CONNECT");
    connect_button->setEnabled(false);
    connect_button->setMaximumWidth(120);
    connect_button->setMinimumWidth(120);
    connect_button->setMaximumHeight(30);
    connect_button->setMinimumHeight(30);

    combo_serial_port = new QComboBox;
    combo_serial_port->setMaximumWidth(120);
    combo_serial_port->setMinimumWidth(120);
    combo_serial_port->setMaximumHeight(30);
    combo_serial_port->setMinimumHeight(30);
    combo_serial_port->addItem(0,"");
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
            combo_serial_port->addItem("/dev/"+port.portName());
    }

    layout_h_conn = new QHBoxLayout;
    layout_h_conn->addWidget(lbl_serial_port);
    layout_h_conn->addWidget(combo_serial_port);
    layout_h_conn->addWidget(connect_button);
    layout_h_conn->setAlignment(connect_button,Qt::AlignVCenter);
    layout_h_conn->addStretch();

    data_layout = new QHBoxLayout;

    // Dichiarazione Received Message GroupBox e configurazione
    box_init_msg = new QGroupBox("Sensors available");
    mw_boxInitMsgConfig();

    // Inserisco gli oggetti all interno del layout
    receiver_layout = new QGridLayout();
    receiver_layout->addLayout(layout_h_conn,0,0);
    receiver_layout->addWidget(box_init_msg,1,0);
    receiver_layout->addLayout(data_layout,2,0);

    // Inserisco il layout che ho formattato all interno del widget principale
    receiver_widget->setLayout(receiver_layout);

    // Dichiarazione SerialConn
    my_serial = new SerialConn();

    // Gestione dei click sul pulsante "connect" per la connessione alla porta seriale
    connect(this->connect_button, SIGNAL(clicked(bool)),this,SLOT(MW_connectBtnHandle()));
    connect(combo_serial_port, SIGNAL(currentIndexChanged(int)),this,SLOT(MW_ComboConnIndexChanged(int)));
    connect(my_serial, SIGNAL(SC_sendInitMsgInfo(VL53LX_INIT_MSG *)), this,
                                    SLOT(MW_receiveInitMsgInfo(VL53LX_INIT_MSG *)));
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
        qDebug() << "\n----------- CONNECTION ESTABLISHED ------------\n\n";
        flag_conn = 1;
    }

    else if(flag_conn == 1)
    {
        //invio a SerialConn il comando di stabilire la connesione
        my_serial->SC_dismissConn();
        connect_button->setText("CONNECT");
        qDebug() << "\n----------- CONNECTION DISMISSED ------------\n\n";
        flag_conn = 0;

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
    greenscaled = green->scaled(QSize(25,25));
    redscaled   = red->scaled(QSize(25,25));

    led_tof1 = new QLabel;
    led_tof2 = new QLabel;
    led_tof3 = new QLabel;
    led_tof4 = new QLabel;
    led_tof5 = new QLabel;
    led_tof6 = new QLabel;
    led_tof7 = new QLabel;
    led_tof8 = new QLabel;

    led_tof1->setPixmap(redscaled);
    led_tof2->setPixmap(redscaled);
    led_tof3->setPixmap(redscaled);
    led_tof4->setPixmap(redscaled);
    led_tof5->setPixmap(redscaled);
    led_tof6->setPixmap(redscaled);
    led_tof7->setPixmap(redscaled);
    led_tof8->setPixmap(redscaled);

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
                qDebug() << "ToF 1 STATUS 0";
            }
            else if(tof_status == 1)
            {
                led_tof1->setPixmap(greenscaled);
                qDebug() << "ToF 1 STATUS 1";
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
}
