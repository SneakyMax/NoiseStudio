TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    graph_view_model.cpp \
    global_context.cpp \
    package_view_model.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES +=

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../../NoiseStudioLib/boost


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../NoiseStudioLib/build/release/ -lNoiseStudioLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../NoiseStudioLib/build/debug/ -lNoiseStudioLib

INCLUDEPATH += $$PWD/../../NoiseStudioLib/NoiseStudioLib
DEPENDPATH += $$PWD/../../NoiseStudioLib/NoiseStudioLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/build/release/libNoiseStudioLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/build/debug/libNoiseStudioLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/build/release/NoiseStudioLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/build/debug/NoiseStudioLib.lib

HEADERS += \
    graph_view_model.h \
    global_context.h \
    package_view_model.h

