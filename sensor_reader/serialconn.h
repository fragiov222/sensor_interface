
/*!
 * *******************************************************************************
 *\n \file serialconn.h
 *\n \brief
 *\n <b>Software Module</b>:
 *\n 		Header file contenente la definizione della classe SerialConn
 *\n
 *\n <b>Description</b>:
 *\n        La classe in questione gestisce la connessione tra l'interfaccia
 *\n        dell'amplificatore e quella del P&T.
 *\n
 *\n <b>Note & Remarks</b>:
 *\n   		'QSerialPort' was designed for RS-232. Having RS232 or 422
 *\n        shouldn't cause any difference to software as we're using asynchronous
 *\n        communication (i.e. without any flow control and handshaking
 *\n        synchronisation lines).
 *\n        The same program should work fine for both RS232 and RS422
 *\n
 *\n <b>History</b>:
 *\n
 *\n 	0.1 - 21/01/2021 - Francesco Giovinazzo
 *\n 	First Issue: fase preliminare
 *\n
 ********************************************************************************
 *\n
 */

#ifndef SERIALCONN_H
#define SERIALCONN_H

/*
 *******************************************************************************
 *> Include Files
 *******************************************************************************
 */

#include <QMainWindow>
#include <QWidget>
#include <QSerialPort>
#include <QByteArray>
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>
#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QStandardPaths>
#include <QtEndian>
#include <QThread>
#include "msg_struct.h"

/*
 *******************************************************************************
 *> Typedefs and Struct
 *******************************************************************************
 */

class SerialConn:public QWidget
{
    Q_OBJECT

public:

    SerialConn();
    ~SerialConn();

signals:
    void SC_sendInitMsgInfo(VL53LX_INIT_MSG *rcv_msg_init);

public slots:

    void SC_readDataHandle();

    void SC_establishConn(QString);
    void SC_dismissConn();

private:

    QSerialPort *serial;

    VL53LX_INIT_MSG *rcv_msg_init;
    VL53LX_DATA_MSG *rcv_msg_data;

    void sc_configSerialConn();
    void sc_ManageRcvMsg(uint8 len);
};


#endif // SERIALCONN_H

