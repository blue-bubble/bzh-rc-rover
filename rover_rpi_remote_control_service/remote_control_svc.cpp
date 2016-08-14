#include "remote_control_svc.h"

#define DEVICE_PATH "/dev/input/js0"

#include <QObject>

remote_control_svc::remote_control_svc()
{
    this->joystick_svc_p = new remote_control_svc_joystick_svc(DEVICE_PATH);
    this->serialport_svc_p = new remote_control_svc_serialport_svc();

    QObject::connect(this,SIGNAL(signal_serialport_svc_write_panding()),
                     this->serialport_svc_p,SLOT(slot_serialport_svc_write_panding()));

    QObject::connect(this->serialport_svc_p,SIGNAL(signal_serialport_svc_read_finished()),
                     this,SLOT(slot_serialport_svc_read_finished()));

    QObject::connect(this,SIGNAL(signal_serialport_svc_reset_arduino()),
                     this->serialport_svc_p,SLOT(slot_serialport_svc_reset_arduino()));
}

remote_control_svc::~remote_control_svc()
{
    delete this->joystick_svc_p;
    delete this->serialport_svc_p;
}

void remote_control_svc::run()
{
    qDebug() << "RemoteControlService Start" << this->currentThreadId();
    this->joystick_svc_p->start();
    this->serialport_svc_p->start();

    //main loop
    this->is_loop = true;
    while(this->is_loop)
    {
        this->msleep(1000);
    }

    this->serialport_svc_p->quit();
    this->joystick_svc_p->quit();
    qDebug() << "RemoteControlService Stop" << this->currentThreadId();
}
void remote_control_svc::quit()
{
    this->is_loop = false;
}

void remote_control_svc::slot_serialport_svc_read_finished()
{
    uint8_t local_recv_data = this->serialport_svc_p->recv_data[0];
    switch((char)local_recv_data)
    {
    case 'i':
        print_msg_info("Controller initiated");
        break;
    case 'r':
        print_msg_info("Controller required control data");

        this->serialport_svc_p->send_data[0] = 0xcd;

        this->serialport_svc_p->send_data[1] = joystick_svc_p->processed_data.servo_1;
        this->serialport_svc_p->send_data[2] = joystick_svc_p->processed_data.servo_2;
        this->serialport_svc_p->send_data[3] = joystick_svc_p->processed_data.servo_3;

        this->serialport_svc_p->send_data[4] = joystick_svc_p->processed_data.direction_l;

        this->serialport_svc_p->send_data[5] = joystick_svc_p->processed_data.threshole_l;
        this->serialport_svc_p->send_data[6] = joystick_svc_p->processed_data.threshole_l;

        emit signal_serialport_svc_write_panding();

        break;
    case 'f':
        print_msg_info("Controller flush buffer");
        break;
    default:
        print_msg_debug("Data:" + QString::number(local_recv_data));
        break;
    }
}
