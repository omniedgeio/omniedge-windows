QT += quick networkauth network
CONFIG += c++11

#QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:"level='requireAdministrator'"

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES  -= UNICODE

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    googleoauth.cpp \
        main.cpp \
    n2nworker.cpp \
    n2nworkerwrapper.cpp \
    syslog.cpp \
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
    googleoauth.h \
    n2nworker.h \
    n2nworkerwrapper.h \
    syslog.h \
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

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

RC_ICONS = "./images/logo.ico"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


