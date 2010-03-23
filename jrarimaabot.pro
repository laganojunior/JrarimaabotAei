SOURCES += main.cpp  \
           int64.cpp \
           piece.cpp \
           board.cpp \
           square.cpp \
           step.cpp 
HEADERS += board.h \
           int64.h \
           piece.h \
           square.h \
           step.h \
           stringsplit.h \

CONFIG += debug_and_release

CONFIG(debug, debug|release) {
 TARGET = jrarimaabot_debug
 QMAKE_CFAGS  += -pg
 QMAKE_CXXFLAGS += -pg
 QMAKE_LFLAGS += -pg
} else {
 TARGET = jrarimaabot
}
