TEMPLATE = app

QT += qml quick multimedia
CONFIG += c++11
CONFIG += thread

SOURCES += main.cpp \
    ADLMIDI/adldata.cpp \
    ADLMIDI/adlmidi.cpp \
    ADLMIDI/dbopl.cpp \
    player/midi_player.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $$PWD/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

HEADERS += \
    ADLMIDI/adldata.hh \
    ADLMIDI/adlmidi.h \
    ADLMIDI/dbopl.h \
    ADLMIDI/fraction.h \
    player/midi_player.h
