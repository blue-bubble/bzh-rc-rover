#ifndef REMOTE_CONTROL_SVC_H
#define REMOTE_CONTROL_SVC_H

#include "remote_control_svc_common_include.h"
#include <QThread>

#include "remote_control_svc_joystick_svc.h"
#include "remote_control_svc_serialport_svc.h"



class remote_control_svc : public QThread
{
    Q_OBJECT

signals:
    void signal_serialport_svc_write_panding();
    void signal_serialport_svc_reset_arduino();

public slots:
    void slot_serialport_svc_read_finished();
private:

    uint8_t control_data[7];


    bool is_loop = false;

public:
    remote_control_svc_joystick_svc * joystick_svc_p;
    remote_control_svc_serialport_svc * serialport_svc_p;

    void run();
    void quit();

    remote_control_svc();
    ~remote_control_svc();
};

#endif // REMOTE_CONTROL_SVC_H
