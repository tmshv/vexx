TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = ../Eks/EksAll.pro \
          ../alter2/Alter2All.pro \
          ../shift/ShiftAll.pro \
          ../Tang/Tang.pro

TARGET = Synapse
DESTDIR = ../bin
