TEMPLATE = lib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += staticlib

SOURCES += \
    graph.cpp \
    graph_node.cpp \
    connection.cpp \
    input_socket.cpp \
    output_socket.cpp \
    socket.cpp \
    accepted_types.cpp \
    nodes/perlin_noise.cpp \
    data_buffer.cpp \
    composite_data_buffer.cpp \
    property.cpp \
    property_collection.cpp \
    graph_executor.cpp \
    nodes/constant_value.cpp \
    nodes/uniform_buffer.cpp \
    nodes/attribute_buffer.cpp \
    graph_outputs.cpp \
    connection_data_type.cpp \
    graph_validator.cpp \
    validation_results.cpp \
    nodes/math.cpp \
    nodes/blank_grid.cpp \
    nodes/mappings/pixel_mapping.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    graph.h \
    graph_node.h \
    connection_data_type.h \
    connection.h \
    input_socket.h \
    output_socket.h \
    socket.h \
    accepted_types.h \
    nodes/perlin_noise.h \
    socket_collection.h \
    data_buffer.h \
    socket_type.h \
    composite_data_buffer.h \
    property.h \
    ptr_array.h \
    property_collection.h \
    utils.h \
    graph_executor.h \
    nodes/constant_value.h \
    ptr_array_common.hpart \
    nodes/uniform_buffer.h \
    nodes/attribute_buffer.h \
    graph_outputs.h \
    graph_validator.h \
    validation_results.h \
    nodes/math.h \
    nodes/blank_grid.h \
    attribute_info.h \
    nodes/mappings/pixel_mapping.h

header_files.path = $$OUT_PWD/lib
header_files.files = $$HEADERS
INSTALLS += header_files

INCLUDEPATH += ../boost

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -fstrict-aliasing

win32: LIBS += -L$$PWD/../boost/build/boost/bin.v2/libs/regex/build/gcc-mingw-4.8.2/debug/link-static/ -lboost_regex-mgw48-d-1_57

INCLUDEPATH += $$PWD/../boost/build/boost/bin.v2/libs/regex/build/gcc-mingw-4.8.2/debug/link-static
DEPENDPATH += $$PWD/../boost/build/boost/bin.v2/libs/regex/build/gcc-mingw-4.8.2/debug/link-static

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../boost/build/boost/bin.v2/libs/regex/build/gcc-mingw-4.8.2/debug/link-static/boost_regex-mgw48-d-1_57.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../boost/build/boost/bin.v2/libs/regex/build/gcc-mingw-4.8.2/debug/link-static/libboost_regex-mgw48-d-1_57.a
