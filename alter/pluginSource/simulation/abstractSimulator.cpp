#include "abstractSimulator.h"

void abstractSimulator::added()
    {
    }

void abstractSimulator::log( QString in )
    {
    emit logSignal( in );
    }

void abstractSimulator::setupChanged()
    {
    emit setupChangedSignal();
    }

QString abstractSimulator::currentTimeLogText( ) const
    {
    return "";
    }
