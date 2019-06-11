#include "zstage.h"

#include <QSerialPortInfo>
#include <QSerialPort>

ZStage::ZStage(QObject *parent) : QObject(parent)
{
}

QStringList ZStage::getAvailableCOMPorts()
{
    QStringList out;
    for (auto& info : QSerialPortInfo::availablePorts())
    {
        out.push_back(info.portName());
    }

    return out;
}

void ZStage::refreshCOMPorts()
{
    emit onAvailableCOMPortsChanged();
}
