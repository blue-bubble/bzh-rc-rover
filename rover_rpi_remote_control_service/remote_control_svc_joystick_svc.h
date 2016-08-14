#ifndef REMOTE_CONTROL_SVC_JOYSTICK_SVC_H
#define REMOTE_CONTROL_SVC_JOYSTICK_SVC_H

#include "remote_control_svc_common_include.h"
#include <QThread>

#include"remote_control_svc_joystick_helper.h"

class remote_control_svc_joystick_svc : public QThread
{
    Q_OBJECT

private:
    remote_control_svc_joystick_helper * joystic_helper_p;

    bool isloop = true;

    int16_t axes_l_raw[2]; // x y
    uint8_t threshold_l_raw[4]; //x+ x- y+ y-

    int16_t axes_r_raw[2]; // x y
    uint8_t button_rb_raw;

    void direction_threshold_refresh();
    void servo_degree_refresh();


public:

    struct processed_data
    {
        uint8_t servo_1 = 90;
        uint8_t servo_2 = 180;
        uint8_t servo_3 = 90;
        uint8_t direction_l;
        uint8_t threshole_l;
    }processed_data;


    void run();
    void quit();

    remote_control_svc_joystick_svc(char* device_path);
    ~remote_control_svc_joystick_svc();
};

#endif // REMOTE_CONTROL_SVC_JOYSTICK_SVC_H
