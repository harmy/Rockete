TEMPLATE = app
TARGET = rockete
DESTDIR = ./debug
QT += gui core opengl
CONFIG += debug console qtestlib
DEFINES += QT_LARGEFILE_SUPPORT QT_OPENGL_LIB
SOURCES += \
 ./src/Action.cpp \
 ./src/ActionManager.cpp \
 ./src/AttributeTreeModel.cpp \
 ./src/CSSHighlighter.cpp \
 ./src/GraphicSystem.cpp \
 ./src/main.cpp \
 ./src/OpenedDocument.cpp \
 ./src/OpenedFile.cpp \
 ./src/OpenedStyleSheet.cpp \
 ./src/PropertyTreeModel.cpp \
 ./src/RenderingView.cpp \
 ./src/Rockete.cpp \
 ./src/RocketHelper.cpp \
 ./src/RocketRenderInterface.cpp \
 ./src/RocketSystem.cpp \
 ./src/StyleSheet.cpp \
 ./src/XMLHighlighter.cpp
HEADERS += \
 ./src/Action.h \
 ./src/AttributeTreeModel.h \
 ./src/CSSHighlighter.h \
 ./src/GraphicSystem.h \
 ./src/OpenedDocument.h \
 ./src/OpenedFile.h \
 ./src/OpenedStyleSheet.h \
 ./src/OpenGL.h \
 ./src/PropertyTreeModel.h \
 ./src/RenderingView.h \
 ./src/Rockete.h \
 ./src/RocketHelper.h \
 ./src/RocketRenderInterface.h \
 ./src/RocketSystem.h \
 ./src/StyleSheet.h \
 ./src/XMLHighlighter.h
FORMS += ./rockete.ui
INCLUDEPATH = ./src
win32 {
    INCLUDEPATH += $(LIBROCKET)/Include
    LIBS += \
        -L$(LIBROCKET)/bin \
        -lkernel32 \
        -luser32 \
        -lshell32 \
        -luuid \
        -lole32 \
        -ladvapi32 \
        -lws2_32 \
        -lgdi32 \
        -lcomdlg32 \
        -loleaut32 \
        -limm32 \
        -lwinmm \
        -lwinspool \
        -lopengl32 \
        -lglu32 \
        -lRocketCore_d
}
unix {
    LIBS += -lRocketCore
}

include(./src/modeltest/modeltest.pri)