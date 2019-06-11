#include "zstage.h"

#include <QSerialPortInfo>
#include <QTextStream>

ZStage::ZStage(QObject *parent)
    : QObject(parent),
      m_connected(false)
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

bool ZStage::connect(const QString& port) {
    m_connected = false;
    if (m_port.isOpen())
        m_port.close();

    m_port.setBaudRate(115200);
    m_port.setParity(QSerialPort::Parity::NoParity);
    m_port.setStopBits(QSerialPort::StopBits::OneStop);
    m_port.setPortName(port);

    if (!m_port.open(QIODevice::ReadWrite)) {
        emit error("Failed to open " + port);
        return false;
    }

    if (!write(QString("INFO_\n")))
        return false;

    QString response;
    if (!readResponse(response))
        return false;

    if (!response.toUpper().contains("PIEZOCONCEPT PRODUCT:"))
    {
        emit error("Unrecognised response from " + port);
        return false;
    }

    m_connected = true;
    return true;
}

bool ZStage::write(const QString& msg, int timeout) {
    m_port.write(msg.toUtf8());
    if (!m_port.waitForBytesWritten(timeout)) {
        emit error("Failed to send data to port " + m_port.portName() + ". Timeout exceeded");
        return false;
    }

    return true;
}

bool ZStage::readResponse(QString& out, int timeout) {
    if (!m_port.waitForReadyRead(timeout))
    {
        emit error("No response from " + m_port.portName());
        return false;
    }

    auto resposeData = m_port.readAll();
    while(m_port.waitForReadyRead(10))
        resposeData += m_port.readAll();

    out = QString::fromUtf8(resposeData);
    return true;
}

quint32 ZStage::getZPos()
{
    if (!m_connected)
        return 0;

    if (!write("GET_Z\n"))
        return 0;

    QString response;
    if (!readResponse(response))
        return 0;

    QTextStream stream(&response);
    int pos = -1; QString unit;
    stream >> pos >> unit;

    if (pos == -1) {
        emit error("Invalid response from " + m_port.portName());
        return 0;
    }

    if (unit.trimmed() == "um") return static_cast<quint32>(pos);
    if (unit.trimmed() == "nm") return static_cast<quint32>(pos / 1000);

    emit error("Warning: Invalid position unit in response from " + m_port.portName());
    return static_cast<quint32>(pos);
}

void ZStage::setZPos(quint32 pos)
{
    if (!m_connected) {
        emit onZPosChanged();
        emit error("No z stage connected");
        return;
    }

    if (!write(QString("MOVEZ %1um").arg(pos)))
        return;

    QString response;
    if (!readResponse(response))
        return;

    emit onZPosChanged();
}
