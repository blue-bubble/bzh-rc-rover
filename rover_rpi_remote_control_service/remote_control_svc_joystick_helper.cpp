#include "remote_control_svc_joystick_helper.h"

remote_control_svc_joystick_helper::remote_control_svc_joystick_helper(char* device_path)
{
    strcpy(this->device_file_path,device_path);
}
remote_control_svc_joystick_helper::~remote_control_svc_joystick_helper()
{
    this->remote_control_svc_joystick_helper_dev_close();
}

void remote_control_svc_joystick_helper::remote_control_svc_joystick_helper_dev_open()
{
    this->fd_handle = open(device_file_path,O_RDONLY);
    //this->fd_handle = open(device_file_path,O_RDONLY|O_NONBLOCK);
    if(this->fd_handle != 0)
    {
        this->is_device_opened = true;
    }
    else
    {
        this->is_device_opened = false;
    }
}
void remote_control_svc_joystick_helper::remote_control_svc_joystick_helper_dev_close()
{
    if(this->is_device_opened)
    {
        close(fd_handle);
    }
    this->is_device_opened = false;
}

void remote_control_svc_joystick_helper::remote_control_svc_joystick_helper_dev_info()
{
    if(this->is_device_opened)
    {
        ioctl(this->fd_handle,JSIOCGAXES,&this->js_info.num_of_axes);
        ioctl(this->fd_handle,JSIOCGBUTTONS,&this->js_info.num_of_buttons);
        ioctl(this->fd_handle,JSIOCGNAME(sizeof(this->js_info.id_string)),&this->js_info.id_string);
        ioctl(this->fd_handle,JSIOCGVERSION,&this->js_info.drv_version);
    }
}

void remote_control_svc_joystick_helper::remote_control_svc_joystick_helper_dev_read()
{
    if(this->is_device_opened)
    {
        read(this->fd_handle,&this->js_event,sizeof(this->js_event));
    }
}
