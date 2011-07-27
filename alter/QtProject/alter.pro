# -------------------------------------------------
# Project created by QtCreator 2009-10-02T18:59:07
# -------------------------------------------------
QT += opengl
TARGET = alter
TEMPLATE = lib
DEFINES += ALTER_LIBRARY
LIBS += -L../../bin \
    -lEksCore \
    -lEksGui
INCLUDEPATH += ../include \
    ../../EksCore/ \
    ../../EksGui
DESTDIR = ../../bin/
QMAKE_CXXFLAGS += -msse
include(../SDK/SDK.pri)
HEADERS += ../include/APrivateWindow.h \
    ../include/APrivateDocument.h \
    ../include/APrivateArrayCommand.h \
    ../include/APrivateAttributeCommand.h \
    ../include/APrivateCommand.h \
    ../include/APrivateCommandStack.h \
    ../include/APrivateCustomDock.h \
    ../include/APrivateDeviceCommand.h \
    ../include/APrivateLogDock.h \
    ../include/APrivatePluginDialog.h \
    ../include/APrivatePropertyCommand.h \
    ../include/APrivateEntityCreators.h \
    ../include/APrivateEntitySaver.h \
    ../include/APrivateSystemEntity.h \
    ../include/APrivateWorkspaceDialog.h \
    ../include/APrivatePropertyWidgetFactory.h \
    ../include/APrivateGuiProperties.h \
    ../include/APrivateSettingsEntity.h \
    ../include/APrivateCommandTools.h \
    ../include/APrivateSplashScreen.h
SOURCES += ../src/APlugin.cpp \
    ../src/APrivateWindow.cpp \
    ../src/APrivateDocument.cpp \
    ../src/APrivateArrayCommand.cpp \
    ../src/APrivateAttributeCommand.cpp \
    ../src/AAbstractDock.cpp \
    ../src/AAbstractToolBar.cpp \
    ../src/APrivateCommandStack.cpp \
    ../src/APrivateCustomDock.cpp \
    ../src/APrivateDeviceCommand.cpp \
    ../src/AEntityLocation.cpp \
    ../src/AAbstractFileType.cpp \
    ../src/APrivateLogDock.cpp \
    ../src/APrivatePluginDialog.cpp \
    ../src/APrivatePropertyCommand.cpp \
    ../src/AProperty.cpp \
    ../src/APropertyDevice.cpp \
    ../src/AEntity.cpp \
    ../src/AEntityArray.cpp \
    ../src/APrivateEntitySaver.cpp \
    ../src/AEntitySearch.cpp \
    ../src/AEntityType.cpp \
    ../src/APrivateSystemEntity.cpp \
    ../src/APrivateWorkspaceDialog.cpp \
    ../src/APrivatePropertyWidgetFactory.cpp \
    ../src/APropertyManager.cpp \
    ../src/ASettings.cpp \
    ../src/APrivateSettingsEntity.cpp \
    ../src/AFileDialog.cpp \
    ../src/AEvent.cpp \
    ../src/AApplication.cpp \
    ../src/APrivateSplashScreen.cpp
RESOURCES += ../resources/branding.qrc \
    ../resources/icons.qrc
FORMS += ../forms/APrivateWindow.ui \
    ../forms/APrivateLogDock.ui \
    ../forms/APrivatePluginDialog.ui \
    ../forms/APrivateWorkspaceDialog.ui
macx:LIBS += -framework \
    CoreFoundation
