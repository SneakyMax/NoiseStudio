TEMPLATE = lib
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += staticlib

SOURCES += \
    graph.cpp \
    graph_node.cpp \
    connection_data_type.cpp \
    connection.cpp \
    input_socket.cpp \
    output_socket.cpp \
    socket.cpp \
    accepted_types.cpp \
    nodes/perlin_noise.cpp \
    data_buffer.cpp \
    composite_data_buffer.cpp

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
    composite_data_buffer.h

header_files.path = $$OUT_PWD/lib
header_files.files = $$HEADERS
INSTALLS += header_files

INCLUDEPATH += ../boost

QMAKE_CXXFLAGS += -std=c++11
