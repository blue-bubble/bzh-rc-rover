#include "remote_control_svc_common_include.h"

void print_msg_info(QString msg_info)
{
    cout << "[ INFO] " << msg_info.toStdString() << endl;
}
void print_msg_debug(QString msg_debug)
{
    cout << "[DEBUG] " << msg_debug.toStdString() << endl;
}
