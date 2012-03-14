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
    XInterfaceObject.cpp \
    XEngine.cpp \
    XContext.cpp \
    XScript.cpp \
    XScriptException.cpp \
    XScriptObject.cpp \
    XScriptFunction.cpp

HEADERS += XScriptGlobal.h \
    XInterface.h \
    XInterfaceObject.h \
    XEngine.h \
    XContext.h \
    XScript.h \
    v8/v8-testing.h \
    v8/v8stdint.h \
    v8/v8-profiler.h \
    v8/v8-preparser.h \
    v8/v8-debug.h \
    v8/v8.h \
    v8/cvv8/XTo.hpp \
    v8/cvv8/V8Shell.hpp \
    v8/cvv8/v8-convert.hpp \
    v8/cvv8/signature.hpp \
    v8/cvv8/properties.hpp \
    v8/cvv8/NativeToJSMap.hpp \
    v8/cvv8/invocable.hpp \
    v8/cvv8/ClassCreator.hpp \
    v8/cvv8/arguments.hpp \
    v8/cvv8/detail/invocable_generated.hpp \
    v8/cvv8/detail/invocable_core.hpp \
    v8/cvv8/detail/convert_core.hpp \
    v8/cvv8/generator/Signature.hpp \
    XSignature.h \
    XSignatureHelpers.h \
    XSignatureSpecialisations.h \
    XTemplateMetaHelpers.h \
    XScriptTypeInfo.h \
    XConvertToScript.h \
    XConvertFromScript.h \
    XConvertScriptSTL.h \
    XConvert.h \
    XFunctions.h \
    XScriptException.h \
    XScriptObject.h \
    XScriptObjectV8Internals.h \
    XScriptFunction.h

debug:LIBS += -L$$ROOT/EksScript/v8/debuglib/
release:LIBS += -L$$ROOT/EksScript/v8/releaselib/

LIBS += -lv8 -lEksCore
INCLUDEPATH += $$ROOT/EksCore $$ROOT/EksScript/v8

OTHER_FILES += \
    License.txt
