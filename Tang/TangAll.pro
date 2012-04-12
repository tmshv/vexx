TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = ../Eks/EksAll.pro \
          ../shift/ShiftAll.pro \
          ../alter2/Alter2All.pro \
          ../Tang/Tang.pro

TARGET = Synapse
DESTDIR = ../bin
