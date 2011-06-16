TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = ../EksCore/QtProject/EksCore.pro \
          ../EksGui/QtProject/EksGui.pro \
          ../Eks3D/QtProject/Eks3D.pro \
          ../alter2/alter2.pro \
          ../shift/shift.pro \
          ../alter2/plugins/ShiftAlter/ShiftAlter.pro \
          ../alter2/plugins/UI/UI.pro \
          ../alter2/plugins/Script/Script.pro \
          ../shift/GraphicsCore/GraphicsCore.pro \
          ../Tang/Tang.pro

TARGET = Synapse
DESTDIR = ../bin
