TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = jLib/QtProject/jLib.pro \
            EksCore/QtProject/EksCore.pro \
            EksGui/QtProject/EksGui.pro \
            alter/QtProject/alter.pro \
            alter/pluginSource/baseFileTypes/baseFileTypes.pro \
            alter/pluginSource/debugBrowser/debugBrowser.pro \
            calc/calc.pro \
            calc/synapseBasicNodes/synapseBasicNodes.pro \
            alter/QtProject/app/app.pro
