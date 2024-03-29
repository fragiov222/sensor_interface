/*!
 * *******************************************************************************
 *\n \file serialconn.cpp
 *\n \brief
 *\n <b>Software Module</b>:
 *\n 		modulo sw serialconn
 *\n
 *\n <b>Description</b>:
 *\n 		Il file contiene variabili e metodi per la visualizzazione dei layouyt
 *\n 		nella finestra principale.
 *\n
 *\n <b>Note & Remarks</b>:
 *\n   	Nessuno
 *\n
 *\n <b>History</b>:
 *\n 	1.0 - 22/09/2021 - Francesco Giovinazzo
 *\n    First Issue: prima implementazione
 *\n
 ********************************************************************************
 *\n
 */

/*
 *******************************************************************************
 *> Include Files
 *******************************************************************************
 */

#include "serialconn.h"

/*
 *******************************************************************************
 *> Global variables
 *******************************************************************************
 */

// Buffer ricezione e invio messaggi
static char rcv_buf [MAX_LEN_MSG];      // buffer dei dati da ricevere

// variabili utilizzate per la lettura dei messaggi ricevuti
                                        // FALSE se i dati ricevuti appartengono a un messaggio già in lettura
static uint16 rcv_bytes_read;            // numero di byte letti per il mesaggio in lettura
static uint16 rcv_bytes_to_read;         // numero di byte ancora da leggere per il messaggio in lettura (<= rcv_msg_rcv_len)
static uint16 rcv_msg_rcv_len;           // numero di byte del messaggio
static uint8 resolution;
static uint8 frequency;
static uint8 sharpener;
/*!
 * *******************************************************************************
 *
 * \fn SerialConn::SerialConn()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
* 		Costruttore della classe SerialConn
 * \n   - inizializzazione dei parametri per la connessione seriale
 * \n   - inizializzazione variabili globali alla classe
 * \n   - gestione eventi
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
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

SerialConn::SerialConn()
{

    //dichiaro una variabile serial che sfrutto per effettuare la comunicazione
    serial = new QSerialPort();

    // Configurazione della porta seriale
    sc_configSerialConn();

    rcv_msg_init = new VL53LX_INIT_MSG;
    memset(rcv_msg_init, 0, sizeof(VL53LX_INIT_MSG));

    rcv_msg_data_64 = new VL53LX_DATA_64_MSG;
    memset(rcv_msg_data_64, 0, sizeof(VL53LX_DATA_64_MSG));

    rcv_msg_data_16 = new VL53LX_DATA_16_MSG;
    memset(rcv_msg_data_16, 0, sizeof(VL53LX_DATA_16_MSG));

    // readyRead() signal is emitted once every time new data is available for reading from the device
    connect(serial, SIGNAL(readyRead()), this, SLOT(SC_readDataHandle()));



}




/*!
 * *******************************************************************************
 *
 * \fn SerialConn::sc_configSerialConn()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		Configura i parametri per la connessione seriale.
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
 * \n- Date: 04/10/2021
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */


void SerialConn::sc_configSerialConn()
{

    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl); //Handshaking

} // sc_configSerialConn()


/*!
 * *******************************************************************************
 *
 * \fn SerialConn::SC_establishConn(QString serial_port_name)
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce la connessione
 *      seriale del ricevitore
 *
 *
 * \n<b>Parameters</b>:\n
 * 		Nessuno
 *
 * \n<b>Returns</b>:\n
 *
 * 		Nessuno
 *
 * \n<b>Note & Remarks</b>:\n
 * 		Nessuno
 *
 * \n<b>History</b>:
 *
 * \n- Version:	1.0
 * \n- Date: 22/09/2021
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void SerialConn::SC_establishConn(QString serial_port_name)
{
    serial->setPortName(serial_port_name);
    serial->open(QIODevice::ReadWrite);
    if(serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "\nConnessione seriale instaurata correttamente\n\n";
    }
    else
         qDebug() << serial->errorString() << endl;

} // SC_establishConn()


/*!
 * *******************************************************************************
 *
 * \fn SerialConn::SC_dismissConn()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce la sconnessione
 *      deln ricevitore
 *
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
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void SerialConn::SC_dismissConn()
{

    serial->close();

} // SC_dismissConn()


/*!
 * *******************************************************************************
 *
 * \fn SerialConn::~SerialConn()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		Distruttore della classe SerialConn

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
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */


SerialConn::~SerialConn()
{

    serial->close();

} // ~SerialConn()



/*!
 * *******************************************************************************
 *
 * \fn SerialConn::SC_readDataHandle()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce l'evento di ricezione messaggi
 *      In particolare, la funzione:
 * \n       - scarica i messaggi ricevuti da P&T
 * \n       - calcola il CRC dei messaggi ricevuti
 * \n       - controlla gli ID dei messaggi per scegliere le azioni da effettuare
 *
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
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */
void SerialConn::SC_readDataHandle()
{

    uint16 tmp_data_size = 0;
    uint16 bytes_in_serial = 0;

    // contatore messaggi ricevuti/inviati
    static int counter;

    //check how many bytes are available in the serial buffer
    bytes_in_serial = serial->bytesAvailable();

    static bool rcv_new_msg = TRUE;                // TRUE se i dati letti si riferiscono a un nuovo messaggio;

    try
    {
        while(bytes_in_serial > 0)      // while the serial buffer is full
        {

            // rcv_new_msg == TRUE --> i dati che si stanno leggendo sono riferiti a un nuovo messaggio
            //                          significa anche che ho finito di leggere il vecchio messaggio

            if (rcv_new_msg == TRUE)
            {
                memset(rcv_buf, 0, sizeof(rcv_buf));

                // lettura header del messaggio -> se i dati in ingresso sono supeiori alla dimensione dell'header
                if ( bytes_in_serial >= static_cast<uint16>(sizeof(MSG_HEADER)) )
                {

                    MSG_HEADER *message_header = new MSG_HEADER();

                    // rcv_bytes read -> numero di bytes del header message
                    rcv_bytes_read = serial->read(rcv_buf, sizeof(MSG_HEADER));

                    // copia dei dati letti nella struttura msg_header
                    memcpy(message_header,rcv_buf, sizeof(MSG_HEADER)); //MESSAGE_HEADER_TYPE_LEN);

                    // dal campo header.msg_len si ricava il numero di byte del messaggio
                    rcv_msg_rcv_len = message_header->msg_len;

                    // byte ancora da leggere
                    rcv_bytes_to_read = static_cast<uint16>(rcv_msg_rcv_len) - rcv_bytes_read;

                    // se nel canale ci sono più di msg_len bytes -> aggiorna il numero di byte nel canale
                    // sottrae il numero di byte dell'header
                    bytes_in_serial -= rcv_bytes_read;

                    // FALSE = i dati che seguono si riferiscono a un messaggio già letto
                    rcv_new_msg = FALSE;

                    delete message_header;
                }

                else // il dato in ingresso è incompleto - esco dalla funzione.
                {
                    return;
                }

            }

            // se bytes_in_serial < bytes ancora da leggere per uno specifico messaggio
            // (calcolati andando a leggere il campo "msg_len" dell'header_msg)

            if (bytes_in_serial < rcv_bytes_to_read)
            {
                tmp_data_size = 0;

                while(tmp_data_size < bytes_in_serial)
                {
                    tmp_data_size += serial->read(rcv_buf + rcv_bytes_read, bytes_in_serial - tmp_data_size);

                    rcv_bytes_to_read -= tmp_data_size;
                    rcv_bytes_read += tmp_data_size;

                }
            }

            else // se bytes_in_serial >= byte ancora da leggere
            {
                tmp_data_size = 0;

                while (tmp_data_size < rcv_bytes_to_read)
                {
                    tmp_data_size += serial->read(rcv_buf + rcv_bytes_read, rcv_bytes_to_read - tmp_data_size);
                    rcv_bytes_read += tmp_data_size;
                }
            }

             bytes_in_serial -= tmp_data_size;

            // se i bytes letti sono uguali al msglen => ho ricevuto il messaggio completamente
            if (rcv_bytes_read == static_cast<uint16>(rcv_msg_rcv_len))
            {
                // TRUE => il prossimo messaggio è un nuovo messaggio
                rcv_new_msg = TRUE;

                // funzione per gestione msg ricevuti
                sc_ManageRcvMsg(rcv_msg_rcv_len);
            }

        }
    }


    catch(...)
    {
        printf("rcv_RxReadDataHandle - Fail to read msg for an exception\n");

        rcv_new_msg = TRUE;

        bytes_in_serial = serial->bytesAvailable();
    }


    counter ++;

} // SC_readDataHandle()



/*!
 * *******************************************************************************
 *
 * \fn void client::sc_ManageRcvMsg(u8 len)
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		Richiama la funzione corretta in base al messaggio ricevuto
 *
 * \n<b>Parameters</b>:\n
 * 		u8 len: lunghezza in byte del messaggio ricevuto
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
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: prima implementazione.
 *
 ********************************************************************************
 */

void SerialConn::sc_ManageRcvMsg(uint16 len)
{

    // Buffer di dimensione arbitraria per l'invio del messaggio
    //char output_buffer[MAX_LEN_MSG];
    //qint64 elapsed_time = 0;
    //QString msg_origin;
    //QString msg_name;
    int i = 0;

    // Segno l'istante in cui ho ricevuto tutto il messaggio
    //elapsed_time = stay_on_timer->elapsed();

    // Calcolo il CRC del messaggio ricevuto
    //computed_crc = crcFast(reinterpret_cast<unsigned char*>(rcv_buf), (u16)(len - FOOTER_LEN));

    switch(rcv_buf[0])
    {
    case ID_VL53LX_INIT_MSG:

        qDebug() << "\nVL53LX_INIT_MSG\n\n";
        rcv_msg_init = reinterpret_cast<VL53LX_INIT_MSG*>(rcv_buf);

        // Mando le info al MainWindow in modo che riempa tabella e scriva nelle label
        emit SC_sendInitMsgInfo(rcv_msg_init);

        break;

    case ID_VL53LX_DATA_MSG:

        //qDebug() << "\nVL53LX_DATA_MSG\n\n";
        if(len == LEN_VL53LX_DATA_64_MSG)
        {
            rcv_msg_data_64 = reinterpret_cast<VL53LX_DATA_64_MSG*>(rcv_buf);
            emit SC_sendData64MsgInfo(rcv_msg_data_64);
        }

        //qDebug() << "\nVL53LX_DATA_MSG\n\n";
        else if(len == LEN_VL53LX_DATA_16_MSG)
        {
            rcv_msg_data_16 = reinterpret_cast<VL53LX_DATA_16_MSG*>(rcv_buf);
            emit SC_sendData16MsgInfo(rcv_msg_data_16);
        }

        break;
    }

}


/*!
 * *******************************************************************************
 *
 * \fn SerialConn::SC_MsgStartHandle(uint8 resolution_val, uint8 frequency_val, uint8 sharpener_val)
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione gestisce la modifica dei parametri del messaggio
 *
 *
 * \n<b>Parameters</b>:\n
 * 		- \n
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
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */


void SerialConn::SC_MsgStartHandle(uint8 resolution_val, uint8 frequency_val, uint8 sharpener_val)
{

    resolution = resolution_val;
    frequency = frequency_val;
    sharpener = sharpener_val;

}   // SC_MsgStartHandle()


/*!
 * *******************************************************************************
 *
 * \fn SerialConn::SC_SendStart()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce la il button "START"
 *
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
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void SerialConn::SC_SendStart()
{

    // Buffer di dimensione arbitraria per l'invio del messaggio
    char output_buffer[MAX_LEN_MSG];

    QString msg_origin;
    QString msg_name;


    // casting del buffer al tipo del messaggio
    ((VL53LX_CONFIG_MSG*)output_buffer)->header.msg_id= ID_VL53LX_CONFIG_MSG;
    ((VL53LX_CONFIG_MSG*)output_buffer)->header.msg_len = LEN_VL53LX_CONFIG_MSG;
    ((VL53LX_CONFIG_MSG*)output_buffer)->resolution = resolution;
    ((VL53LX_CONFIG_MSG*)output_buffer)->frequency = frequency;
    ((VL53LX_CONFIG_MSG*)output_buffer)->sharpener = sharpener;

    serial->write(output_buffer, sizeof(VL53LX_CONFIG_MSG));

}   //SC_SendStart()


/*!
 * *******************************************************************************
 *
 * \fn SerialConn::SC_SendStop()
 *
 * \brief
 *
 * \n<b>Description</b>:\n
 * 		La funzione è l'handle (SLOT) che gestisce la il button "STOP"
 *
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
 * \n- Author: Francesco Giovinazzo
 * \n- Description:
 * 		First Issue: fase preliminare
 *
 ********************************************************************************
 */

void SerialConn::SC_SendStop()
{

    // Buffer di dimensione arbitraria per l'invio del messaggio
    char output_buffer[MAX_LEN_MSG];

    QString msg_origin;
    QString msg_name;
    uint8 stop = 1;

    // casting del buffer al tipo del messaggio
    ((VL53LX_STOP_MSG*)output_buffer)->header.msg_id= ID_VL53LX_STOP_MSG;
    ((VL53LX_STOP_MSG*)output_buffer)->header.msg_len = LEN_VL53LX_STOP_MSG;
    ((VL53LX_STOP_MSG*)output_buffer)->stop = stop;

    serial->write(output_buffer, sizeof(VL53LX_STOP_MSG));

}   //SC_SendStop()

