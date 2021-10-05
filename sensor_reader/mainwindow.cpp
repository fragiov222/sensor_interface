#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "msg_struct.h"


#define         WINDOW_WIDTH                        1200
#define         WINDOW_HEIGTH                       700

/*
 *******************************************************************************
 *> Global variables
 *******************************************************************************
 */
static bool flag_conn = 0;          // true if the connection is established
QString serial_port_name = "";

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

    layout_v_sensors = new QVBoxLayout;
    data_layout = new QHBoxLayout;

    // Inserisco gli oggetti all interno del layout
    receiver_layout = new QGridLayout();
    receiver_layout->addLayout(layout_h_conn,0,0);
    receiver_layout->addLayout(layout_v_sensors,1,0);
    receiver_layout->addLayout(data_layout,2,0);

    // Inserisco il layout che ho formattato all interno del widget principale
    receiver_widget->setLayout(receiver_layout);

    // Dichiarazione SerialConn
    my_serial = new SerialConn();

    // Gestione dei click sul pulsante "connect" per la connessione alla porta seriale
    connect(this->connect_button, SIGNAL(clicked(bool)),this,SLOT(MW_connectBtnHandle()));
    connect(combo_serial_port, SIGNAL(currentIndexChanged(int)),this,SLOT(MW_ComboConnIndexChanged(int)));
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

