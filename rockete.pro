TEMPLATE = app
TARGET = rockete
DESTDIR = ./debug
QT += gui core opengl xml
CONFIG += debug_and_release console qtestlib
DEFINES += QT_LARGEFILE_SUPPORT QT_OPENGL_LIB
RESOURCES = rockete.qrc
SOURCES += \
 ./src/Action.cpp \
 ./src/ActionGroup.cpp \
 ./src/ActionManager.cpp \
 ./src/ActionInsertElement.cpp \
 ./src/ActionSetAttribute.cpp \
 ./src/ActionSetInlineProperty.cpp \
 ./src/ActionSetProperty.cpp \
 ./src/AttributeTreeModel.cpp \
 ./src/CodeEditor.cpp \
 ./src/CSSHighlighter.cpp \
 ./src/DocumentHierarchyEventFilter.cpp \
 ./src/EditionHelper.cpp \
 ./src/EditionHelperColor.cpp \
 ./src/GraphicSystem.cpp \
 ./src/LocalizationManagerInterface.cpp \
 ./src/LuaHighlighter.cpp \
 ./src/main.cpp \
 ./src/OpenedDocument.cpp \
 ./src/OpenedFile.cpp \
 ./src/OpenedLuaScript.cpp \
 ./src/OpenedStyleSheet.cpp \
 ./src/ProjectManager.cpp \
 ./src/PropertyTreeModel.cpp \
 ./src/RenderingView.cpp \
 ./src/Rockete.cpp \
 ./src/RocketFileInterface.cpp \
 ./src/RocketHelper.cpp \
 ./src/RocketRenderInterface.cpp \
 ./src/RocketSystem.cpp \
 ./src/Settings.cpp \
 ./src/SnippetsManager.cpp \
 ./src/StyleSheet.cpp \
 ./src/Tool.cpp \
 ./src/ToolDiv.cpp \
 ./src/ToolImage.cpp \
 ./src/ToolManager.cpp \
 ./src/ToolSelecter.cpp \
 ./src/ToolTest.cpp \
 ./src/XMLHighlighter.cpp \
 ./src/WizardButton.cpp
HEADERS += \
 ./src/Action.h \
 ./src/ActionGroup.h \
 ./src/ActionInsertElement.h \
 ./src/ActionManager.h \
 ./src/ActionSetAttribute.h \
 ./src/ActionSetInlineProperty.h \
 ./src/ActionSetProperty.h \
 ./src/AttributeTreeModel.h \
 ./src/CodeEditor.h \
 ./src/CSSHighlighter.h \
 ./src/DocumentHierarchyEventFilter.h \
 ./src/EditionHelper.h \
 ./src/EditionHelperColor.h \
 ./src/GraphicSystem.h \
 ./src/LocalizationManagerInterface.h \
 ./src/LuaHighlighter.h \
 ./src/OpenedDocument.h \
 ./src/OpenedFile.h \
 ./src/OpenedLuaScript.h \
 ./src/OpenedStyleSheet.h \
 ./src/OpenGL.h \
 ./src/ProjectManager.h \
 ./src/PropertyTreeModel.h \
 ./src/RenderingView.h \
 ./src/Rockete.h \
 ./src/RocketFileInterface.h \
 ./src/RocketHelper.h \
 ./src/RocketRenderInterface.h \
 ./src/RocketSystem.h \
 ./src/Settings.h \
 ./src/SnippetsManager.h \
 ./src/StyleSheet.h \
 ./src/Tool.h \
 ./src/ToolManager.h \
 ./src/ToolDiv.h \
 ./src/ToolImage.h \
 ./src/ToolSelecter.h \
 ./src/ToolTest.h \
 ./src/XMLHighlighter.h \
 ./src/WizardButton.h
FORMS += ./ui/rockete.ui \

INCLUDEPATH = ./src $(LIBROCKET)/Include

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
        -lglu32

    CONFIG(debug, debug|release) {
        LIBS += -lRocketCore_d -lRocketControls_d
        
        exists( $(LIBROCKET)/Build/RocketFreeType_d.lib ) {
            LIBS += -lRocketFreeType_d
            DEFINES += ROCKET_FREETYPE
        }
    }
    
    CONFIG(release, debug|release) {
        LIBS += -lRocketCore -lRocketControls
        
        exists( $(LIBROCKET)/Build/RocketFreeType.lib ) {
            LIBS += -lRocketFreeType -lfreetype
            DEFINES += ROCKET_FREETYPE
        }
    }
}
unix {
    INCLUDEPATH += $(LIBROCKET)/include
    LIBS += -L$(LIBROCKET)/lib -L$(LIBROCKET)/Build -lRocketCore -lRocketControls -lGLU 
    
    exists( $(LIBROCKET)/Build/libRocketFreeType.a ) {
        LIBS += -lRocketFreeType -lfreetype
        DEFINES += ROCKET_FREETYPE
    }
    
}

include(./src/modeltest/modeltest.pri)
