TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    connection_data_type_tests.cpp \
    data_buffer_tests.cpp \
    composite_data_buffer_tests.cpp \
    property_collection_tests.cpp \
    simple_graph_tests.cpp \
    graph_validator_tests.cpp \
    math_node_tests.cpp \
    blank_grid_mapping_tests.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    catch.h

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

win32: LIBS += -L$$PWD/../../NoiseStudioLib/boost/build/boost/bin.v2/libs/regex/build/gcc-mingw-4.8.2/debug/link-static/ -lboost_regex-mgw48-d-1_57

INCLUDEPATH += $$PWD/../../NoiseStudioLib/boost/build/boost/bin.v2/libs/regex/build/gcc-mingw-4.8.2/debug/link-static
DEPENDPATH += $$PWD/../../NoiseStudioLib/boost/build/boost/bin.v2/libs/regex/build/gcc-mingw-4.8.2/debug/link-static

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/boost/build/boost/bin.v2/libs/regex/build/gcc-mingw-4.8.2/debug/link-static/boost_regex-mgw48-d-1_57.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../NoiseStudioLib/boost/build/boost/bin.v2/libs/regex/build/gcc-mingw-4.8.2/debug/link-static/libboost_regex-mgw48-d-1_57.a
