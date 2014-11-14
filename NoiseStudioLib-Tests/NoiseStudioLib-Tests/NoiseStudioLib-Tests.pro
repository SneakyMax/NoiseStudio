TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    connection_data_type_tests.cpp \
    data_buffer_tests.cpp \
    composite_data_buffer_tests.cpp \
    property_collection_tests.cpp \
    simple_graph_tests.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    catch.h

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += ../../NoiseStudioLib/boost

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../NoiseStudioLib/build/release/ -lNoiseStudioLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../NoiseStudioLib/build/debug/ -lNoiseStudioLib

INCLUDEPATH += $$PWD/../../NoiseStudioLib/build/lib
DEPENDPATH += $$PWD/../../NoiseStudioLib/build/lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/build/release/libNoiseStudioLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/build/debug/libNoiseStudioLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/build/release/NoiseStudioLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/build/debug/NoiseStudioLib.lib
