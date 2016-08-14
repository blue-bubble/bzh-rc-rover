#ifndef REMOTE_CONTROL_SVC_JOYSTICK_HELPER_H
#define REMOTE_CONTROL_SVC_JOYSTICK_HELPER_H

#include "remote_control_svc_common_include.h"
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <limits>
#include <linux/joystick.h>

//define data package types

typedef struct
{
    uint32_t time;     /* event timestamp in milliseconds */
    int16_t value;    /* value */
    uint8_t type;      /* event type */
    uint8_t number;    /* axis/button number */

}_js_event;

typedef struct
{
    uint8_t num_of_axes;
    uint8_t num_of_buttons;
    uint32_t drv_version;
    char id_string[256];

}_js_info;

class remote_control_svc_joystick_helper
{
private:
    char device_file_path[PATH_MAX];
    int fd_handle;
    bool is_device_opened;

public:
    _js_event js_event;
    _js_info js_info;

    void remote_control_svc_joystick_helper_dev_open();
    void remote_control_svc_joystick_helper_dev_close();
    void remote_control_svc_joystick_helper_dev_info();
    void remote_control_svc_joystick_helper_dev_read();

    remote_control_svc_joystick_helper(char* device_path);
    ~remote_control_svc_joystick_helper();
};

#endif // REMOTE_CONTROL_SVC_JOYSTICK_HELPER_H
