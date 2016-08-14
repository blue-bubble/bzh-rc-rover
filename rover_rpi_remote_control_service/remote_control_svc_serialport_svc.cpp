#include "remote_control_svc_serialport_svc.h"

remote_control_svc_serialport_svc::remote_control_svc_serialport_svc()
{
    //value init
    loop = false;
    baudrate = QSerialPort::Baud115200;
    stopbits = QSerialPort::TwoStop;

    //serial port init
    serial_port_p = new QSerialPort;
    serial_port_p->setBaudRate(baudrate);
    serial_port_p->setStopBits(stopbits);

    //search arduino serial port
    while(this->serial_port_p->portName().isEmpty())
    {
        serialports = QSerialPortInfo::availablePorts();
        for(int i = 0;i < this->serialports.size();i++)
        {
            if(this->serialports.at(i).portName().startsWith("ttyACM"))
            {
                this->serial_port_p->setPort(this->serialports.at(i));
                break;
            }
        }
    }
    print_msg_info("Serial Port:" + this->serial_port_p->portName());

    //open serial port
    if(!serial_port_p->isOpen())
    {
        serial_port_p->open(QIODevice::ReadWrite);
    }
    //check open and print
    if(this->serial_port_p->isOpen())
    {
        print_msg_info("Serial Port Connected");
    }
    else
    {
        print_msg_info("Serial Port NOT Connected");
    }
}

remote_control_svc_serialport_svc::~remote_control_svc_serialport_svc()
{
    //close serial port
    if(serial_port_p->isOpen())
    {
        serial_port_p->close();
    }
    //check close and print
    this->msleep(100);
    if(!serial_port_p->isOpen())
    {
        print_msg_info("Serial Port Disconnected");
    }

    this->quit();
    delete serial_port_p;
}

void remote_control_svc_serialport_svc::slot_serialport_svc_write_panding()
{
    if(serial_port_p->isOpen())
    {
        serial_port_p->write((char*)this->send_data,7);
    }
}

void remote_control_svc_serialport_svc::slot_serialport_svc_reset_arduino()
{
    if(serial_port_p->isOpen())
    {
        serial_port_p->setDataTerminalReady(true);
        this->msleep(100);
        serial_port_p->setDataTerminalReady(false);
    }
}

void remote_control_svc_serialport_svc::run()
{
    qDebug() << "SerialPortService Start" << this->currentThreadId();

    //main loop
    loop = true;
    while(loop)
    {
        if(serial_port_p->isOpen())
        {
            if(serial_port_p->bytesAvailable() > 0)
            {
                serial_port_p->read((char*)this->recv_data,1);

                this->msleep(30);
                emit signal_serialport_svc_read_finished();
            }
            else
            {
                //1000 for debug 10 for normal
                this->msleep(100);
            }
        }
        else
        {
            this->msleep(300);
        }
    }

    qDebug() << "SerialPortService Stop" << this->currentThreadId();
}

void remote_control_svc_serialport_svc::quit()
{
    if(serial_port_p->isOpen())
    {
        serial_port_p->close();
    }
    loop = false;
}
