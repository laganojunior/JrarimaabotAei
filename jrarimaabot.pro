SOURCES += main.cpp
CONFIG += debug_and_release

CONFIG(debug, debug|release) {
 TARGET = jrarimaabot_debug
 QMAKE_CFAGS  += -pg
 QMAKE_CXXFLAGS += -pg
 QMAKE_LFLAGS += -pg
} else {
 TARGET = jrarimaabot
}
