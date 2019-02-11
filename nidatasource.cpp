#include "nidatasource.h"

#include <QDebug>
#include <chrono>
#include <iostream>

NIDataSource::NIDataSource(QQuickView *appViewer, QObject *parent)
    : QObject(parent),
      m_appViewer(appViewer)
{
    updateAvailableDevices();
}

void NIDataSource::updateAvailableDevices()
{
    qDebug() << "Update Available Devices Called";

    m_availableDevices = QStringList();

    for (auto card: NICard::getAvailableDevices())
        m_availableDevices.push_back(QString::fromStdString(card));

    emit availableDevicesChanged();
}

const QStringList NIDataSource::availableDevices() const
{
    return m_availableDevices;
}

const QStringList NIDataSource::digitalOutLines() const
{
    qDebug() << "Get Digital Out Lines Called";
    QStringList out;

    for (auto line: m_device.getDigitalOutLines())
        out.push_back(QString::fromStdString(line));

    return out;
}

const QStringList NIDataSource::counters() const
{
    qDebug() << "Get Counters Called";
    QStringList out;

    for (auto counter: m_device.getCounters())
        out.push_back(QString::fromStdString(counter));

    return out;
}

const QStringList NIDataSource::counterLines() const
{
    qDebug() << "Get Counter Lines Called";
    QStringList out;

    for (auto line: m_device.getCounterLines())
        out.push_back(QString::fromStdString(line));

    return out;
}

const QStringList NIDataSource::timebases() const
{
    qDebug() << "Get Timebases Called";
    QStringList out;

    for (auto line: m_device.getTimebases())
        out.push_back(QString::fromStdString(line));

    return out;
}

void NIDataSource::setCurrentDevice(const QString& devName)
{
    qDebug() << "Set Current Device Called";
    m_device = NICard(devName.toStdString());
    emit digitalOutLinesChanged();
    emit counterLinesChanged();
    emit timebasesChanged();
    emit countersChanged();
    emit sendValues();
}

void NIDataSource::setDonorLaserPin(const QString& pin)  { m_device.setDonorLaserPin(pin.toStdString()); }
void NIDataSource::setAcceptorLaserPin(const QString& pin) { m_device.setAcceptorLaserPin(pin.toStdString()); }
void NIDataSource::setDonorDetectorCounter(const QString& pin) { m_device.setDonorDetectorCounter(pin.toStdString()); }
void NIDataSource::setAcceptorDetectorCounter(const QString& pin) { m_device.setAcceptorDetectorCounter(pin.toStdString()); }
void NIDataSource::setDonorDetectorPin(const QString& pin) { m_device.setDonorDetectorPin(pin.toStdString()); }
void NIDataSource::setAcceptorDetectorPin(const QString& pin) { m_device.setAcceptorDetectorPin(pin.toStdString()); }
void NIDataSource::setDonorDetectorGate(const QString& pin) { m_device.setDonorDetectorGate(pin.toStdString()); }
void NIDataSource::setAcceptorDetectorGate(const QString& pin) { m_device.setAcceptorDetectorGate(pin.toStdString()); }
void NIDataSource::setTimebase(const QString& pin) { m_device.setTimebase(pin.toStdString()); }
void NIDataSource::setLaserControlResolution(quint32 res) {
    qDebug() << "Set Laser Control Resolution called with:" << res;
    m_device.setLaserControlResolution(std::chrono::microseconds(res));
}

bool NIDataSource::startAcquisition() {
    auto res = m_device.prime();

    if (res.has_value())
    {
        qCritical() << QString::fromStdString(res.value());
        return false;
    }

    res = m_device.start();
    if (res.has_value())
    {
        qCritical() << QString::fromStdString(res.value());
        return false;
    }

    return true;
}

bool NIDataSource::stopAcquisition()
{
    auto res = m_device.stop();
    if (res.has_value())
    {
        qWarning() << QString::fromStdString(res.value());
        return false;
    }

    return true;
}
