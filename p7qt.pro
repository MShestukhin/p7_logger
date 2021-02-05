QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

#win32:CONFIG(release, debug|release): LIBS += -L/home/melkonst/Projects/libP7Client_v5.6/_build_/Sources/ -lp7
#else:win32:CONFIG(debug, debug|release): LIBS += -L/home/melkonst/Projects/libP7Client_v5.6/_build_/Sources/ -lp7


#INCLUDEPATH += /home/melkonst/Projects/libP7Client_v5.6/_build_/Sources
#DEPENDPATH += /home/melkonst/Projects/libP7Client_v5.6/_build_/Sources

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += /home/melkonst/Projects/libP7Client_v5.6/_build_/Sources/libp7.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += /home/melkonst/Projects/libP7Client_v5.6/_build_/Sources/libp7.a

INCLUDEPATH += /home/melkonst/Projects/libP7Client_v5.6/Headers

LIBS += -lp7
LIBS += -lrt
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
