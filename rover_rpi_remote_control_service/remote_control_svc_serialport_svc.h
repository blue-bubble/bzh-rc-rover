#ifndef REMOTE_CONTROL_SVC_SERIALPORT_SVC_H
#define REMOTE_CONTROL_SVC_SERIALPORT_SVC_H

#include "remote_control_svc_common_include.h"
#include <QThread>

#include <QtSerialPort>
#include <QList>

class remote_control_svc_serialport_svc : public QThread
{
    Q_OBJECT

signals:
    void signal_serialport_svc_read_finished();

public slots:
    void slot_serialport_svc_write_panding();
    void slot_serialport_svc_reset_arduino();

private:

    bool loop;
    qint32 baudrate;
    QSerialPort::StopBits stopbits;
    QList<QSerialPortInfo> serialports;

    QSerialPort * serial_port_p;

public:

    uint8_t recv_data[1];
    uint8_t send_data[7];

    void run();
    void quit();

    remote_control_svc_serialport_svc();
    ~remote_control_svc_serialport_svc();
};

#endif // REMOTE_CONTROL_SVC_SERIALPORT_SVC_H
