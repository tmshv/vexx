#-------------------------------------------------
#
# Project created by QtCreator 2012-02-16T11:35:32
#
#-------------------------------------------------

QT       -= gui

TARGET = EksScript
TEMPLATE = lib

include("../EksCore/GeneralOptions.pri")

SOURCES += XScriptGlobal.cpp \
    XInterface.cpp \
    XEngine.cpp \
    XContext.cpp \
    XScriptException.cpp \
    XScriptValue.cpp \
    XScriptFunction.cpp \
    XScriptSource.cpp \
    XScriptObject.cpp \
    XQObjectWrapper.cpp \
    XFunctions.cpp

HEADERS += XScriptGlobal.h \
    XInterface.h \
    XEngine.h \
    XContext.h \
    XSignature.h \
    XSignatureHelpers.h \
    XSignatureSpecialisations.h \
    XTemplateMetaHelpers.h \
    XScriptTypeInfo.h \
    XConvertToScript.h \
    XConvertFromScript.h \
    XConvertScriptSTL.h \
    XConvert.h \
    XScriptException.h \
    XScriptValue.h \
    XScriptValueV8Internals.h \
    XScriptFunction.h \
    XInterfaceUtilities.h \
    XProperties.h \
    XScriptConstructors.h \
    XConvertToScriptMap.h \
    XFunctions.h \
    XFunctionSpecialisations.h \
    XScriptSource.h \
    XScriptObject.h \
    XQObjectWrapper.h

LIBS += -lQtV8 -lEksCore
INCLUDEPATH += $$ROOT/EksCore $$ROOT/EksScript/qtjsbackend/src/3rdparty/v8/include

OTHER_FILES += \
    License.txt
