#include "remote_control_svc_joystick_svc.h"

remote_control_svc_joystick_svc::remote_control_svc_joystick_svc(char* device_path)
{
    joystic_helper_p = new remote_control_svc_joystick_helper(device_path);
}
remote_control_svc_joystick_svc::~remote_control_svc_joystick_svc()
{
    joystic_helper_p->~remote_control_svc_joystick_helper();
    delete joystic_helper_p;
}

void remote_control_svc_joystick_svc::direction_threshold_refresh()
{
    this->processed_data.direction_l = 0;
    this->processed_data.threshole_l = 0;

    //ly
    if(axes_l_raw[1] >= 0)
    {
        this->processed_data.direction_l &= ~((1<<0)|(1<<1));//REVERSE
        this->threshold_l_raw[2] = (axes_l_raw[1]*255)/32767;
    }
    else
    {
        this->processed_data.direction_l |= ((1<<0)|(1<<1));//FORWARD
        this->threshold_l_raw[3] = (axes_l_raw[1]*255)/-32767;
    }

    //lx
    if(axes_l_raw[0] >= 0)
    {
        if(axes_l_raw[0] >= 20000)
        {
            this->processed_data.direction_l |= (1<<1);//L
            this->processed_data.direction_l &= ~(1<<0);//R
        }
        this->threshold_l_raw[0] = (axes_l_raw[0]*255)/32767;
    }
    else
    {
        if(axes_l_raw[0] <= -20000)
        {
            this->processed_data.direction_l &= ~(1<<1);//L
            this->processed_data.direction_l |= (1<<0);//R
        }
        this->threshold_l_raw[1] = (axes_l_raw[0]*255)/-32767;
    }

    for(int i = 0; i < 4; i++)
    {
        if(this->threshold_l_raw[i] > this->processed_data.threshole_l)
        {
            this->processed_data.threshole_l = this->threshold_l_raw[i];
        }
    }
}
void remote_control_svc_joystick_svc::servo_degree_refresh()
{
    //servo 1 x
    this->processed_data.servo_1 = (this->axes_r_raw[0]+32767)*180/65535;

    //servo 2 y
    this->processed_data.servo_2 = (this->axes_r_raw[1]+32767)*180/65535;
    if(this->processed_data.servo_2 < 70)
    {
        this->processed_data.servo_2 = 70;
    }

    //servo 3 (make sure the values are suit the hand!!)
    if(this->button_rb_raw == 1)
    {
        this->processed_data.servo_3 = 180;
    }
    if(this->button_rb_raw == 0)
    {
        this->processed_data.servo_3 = 0;
    }
}

void remote_control_svc_joystick_svc::run()
{
    qDebug() << "JoystickService Start" << this->currentThreadId();

    this->joystic_helper_p->remote_control_svc_joystick_helper_dev_open();
    this->joystic_helper_p->remote_control_svc_joystick_helper_dev_info();

    while(this->isloop)
    {
        this->joystic_helper_p->remote_control_svc_joystick_helper_dev_read();

        print_msg_debug("number=" + QString::number(this->joystic_helper_p->js_event.number) + " " +
                        "value=" + QString::number(this->joystic_helper_p->js_event.value) + " " +
                        "time=" + QString::number(this->joystic_helper_p->js_event.time));
        switch(this->joystic_helper_p->js_event.type)
        {
        case JS_EVENT_BUTTON:
            print_msg_debug("Joystick button event");
            switch(this->joystic_helper_p->js_event.number)
            {
            case 5:
                this->button_rb_raw = this->joystic_helper_p->js_event.value;
                servo_degree_refresh();
                break;

            default:
                break;
            }

            break;
        case JS_EVENT_AXIS:
            print_msg_debug("Joystick axis event");

            switch(this->joystic_helper_p->js_event.number)
            {
            //L
            case 0:
                //X
                this->axes_l_raw[0] = this->joystic_helper_p->js_event.value;
                direction_threshold_refresh();

                break;
            case 1:
                //Y
                this->axes_l_raw[1] = this->joystic_helper_p->js_event.value;
                direction_threshold_refresh();
                break;
            case 2:
                //Z
                break;

                //R
            case 3:
                //X
                this->axes_r_raw[0] = this->joystic_helper_p->js_event.value;
                servo_degree_refresh();
                break;
            case 4:
                //Y
                this->axes_r_raw[1] = this->joystic_helper_p->js_event.value;
                servo_degree_refresh();
                break;
            case 5:
                //Z
                break;

                //PAD
            case 6:
                //X
                break;
            case 7:
                //Y
                break;

            default:
                break;
            }
            break;
        case JS_EVENT_INIT:
            print_msg_debug("Joystick init event");
            break;
        default:
            //rint_msg_debug("Joystick invalid event");
            break;
        }

        print_msg_debug("servo_1=" + QString::number(this->processed_data.servo_1));
        print_msg_debug("servo_2=" + QString::number(this->processed_data.servo_2));
        print_msg_debug("servo_3=" + QString::number(this->processed_data.servo_3));
        print_msg_debug("threshold=" + QString::number(this->processed_data.threshole_l));
        print_msg_debug("direction=" + QString::number(this->processed_data.direction_l));
    }

    this->joystic_helper_p->remote_control_svc_joystick_helper_dev_close();

    qDebug() << "JoystickService Stop" << this->currentThreadId();
}

void remote_control_svc_joystick_svc::quit()
{
    this->isloop = false;
}
