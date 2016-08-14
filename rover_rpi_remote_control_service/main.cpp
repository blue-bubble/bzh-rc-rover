#include <QCoreApplication>
#include "remote_control_svc.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    remote_control_svc remote_control_svc_p;
    remote_control_svc_p.start();

    return a.exec();
}
