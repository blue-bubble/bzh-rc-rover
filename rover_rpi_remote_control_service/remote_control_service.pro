QT += core
QT -= gui

CONFIG += c++11

TARGET = remote_control_service
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QT += serialport

SOURCES += main.cpp \
    remote_control_svc_serialport_svc.cpp \
    remote_control_svc_joystick_svc.cpp \
    remote_control_svc_joystick_helper.cpp \
    remote_control_svc.cpp \
    remote_control_svc_common_include.cpp

HEADERS += \
    remote_control_svc_common_include.h \
    remote_control_svc_serialport_svc.h \
    remote_control_svc_joystick_svc.h \
    remote_control_svc_joystick_helper.h \
    remote_control_svc.h
