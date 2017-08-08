QT += qml quick serialbus

CONFIG += c++11

SOURCES += main.cpp \
    caninterface.cpp \
    canpids.cpp \
    cannodes.cpp \
    translation.cpp \
    settings.cpp

RESOURCES += qml/qml.qrc \
             images/images.qrc \
             sounds/sounds.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    caninterface.h \
    ../../parkbrake_ECU/can_protocol.h \
    canpids.h \
    cannodes.h \
    ../../parkbrake_ECU/control.h \
    translation.h \
    settings.h \
    ../../parkbrake_ECU/park_settings.h

INCLUDEPATH += ../../parkbrake_ECU/

TRANSLATIONS.path = ts/
TRANSLATIONS += ts_sv.ts


copydata.commands = $(COPY_DIR) $$PWD/ts/ $$OUT_PWD/ts/
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata


INSTALLS += TRANSLATIONS
