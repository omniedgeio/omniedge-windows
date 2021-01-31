QT       += core gui network networkauth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:"level='requireAdministrator'"

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES -= UNICODE

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    menucontroller.cpp \
    n2nworker.cpp \
    oauth.cpp \
    oauthreplyhandler.cpp \
    proxy.cpp \
    syslog.cpp \
    tapmanager.cpp \
    traymenu.cpp \
    n2n/edge_utils.c \
    n2n/minilzo.c \
    n2n/transform_null.c \
    n2n/transform_tf.c \
    n2n/twofish.c \
    n2n/wire.c \
    n2n/win32/getopt.c \
    n2n/win32/getopt1.c \
    n2n/win32/version-msvc.c \
    n2n/win32/wintap.c \
    n2n/n2n.c

HEADERS += \
    menucontroller.h \
    n2nworker.h \
    oauth.h \
    oauthreplyhandler.h \
    proxy.h \
    syslog.h \
    tapmanager.h \
    traymenu.h \
    n2n/lzoconf.h \
    n2n/lzodefs.h \
    n2n/minilzo.h \
    n2n/n2n.h \
    n2n/n2n_transforms.h \
    n2n/n2n_wire.h \
    n2n/twofish.h \
    n2n/uthash.h \
    n2n/win32/getopt.h \
    n2n/win32/n2n_win32.h \
    n2n/win32/winconfig.h \
    n2n/win32/wintap.h \
    n2n/n2n_bridging_header.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
RC_ICONS = "./images/logo.ico"
