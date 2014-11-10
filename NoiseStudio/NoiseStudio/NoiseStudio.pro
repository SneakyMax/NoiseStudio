TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../NoiseStudioLib/NoiseStudioLib/build-NoiseStudioLib-Desktop_Qt_5_3_MinGW_32bit-Debug/release/ -lNoiseStudioLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../NoiseStudioLib/NoiseStudioLib/build-NoiseStudioLib-Desktop_Qt_5_3_MinGW_32bit-Debug/debug/ -lNoiseStudioLib

INCLUDEPATH += $$PWD/../../NoiseStudioLib/NoiseStudioLib/build-NoiseStudioLib-Desktop_Qt_5_3_MinGW_32bit-Debug/lib
DEPENDPATH += $$PWD/../../NoiseStudioLib/NoiseStudioLib/build-NoiseStudioLib-Desktop_Qt_5_3_MinGW_32bit-Debug/lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/NoiseStudioLib/build-NoiseStudioLib-Desktop_Qt_5_3_MinGW_32bit-Debug/release/libNoiseStudioLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/NoiseStudioLib/build-NoiseStudioLib-Desktop_Qt_5_3_MinGW_32bit-Debug/debug/libNoiseStudioLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/NoiseStudioLib/build-NoiseStudioLib-Desktop_Qt_5_3_MinGW_32bit-Debug/release/NoiseStudioLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/NoiseStudioLib/build-NoiseStudioLib-Desktop_Qt_5_3_MinGW_32bit-Debug/debug/NoiseStudioLib.lib
