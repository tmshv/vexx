TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = ../EksCore/QtProject/EksCore.pro \
          ../EksGui/QtProject/EksGui.pro \
          ../alter2/alter2.pro \
          ../shift/shift.pro \
          ../alter2/plugins/ShiftAlter/ShiftAlter.pro \
          ../alter2/plugins/UI/UI.pro \
          ../Synapse/SynapseCore/SynapseCore.pro \
          ../Synapse/Synapse.pro

TARGET = Synapse
DESTDIR = ../bin
