#include "nidatasource.h"

#include <QDebug>
#include <QtCharts/QXYSeries>

#include <chrono>
#include <iostream>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries*)
Q_DECLARE_METATYPE(QAbstractAxis*)

using namespace std::chrono_literals;

NIDataSource::NIDataSource(QQuickView *appViewer, QObject *parent)
    : QObject(parent),
      m_appViewer(appViewer),
      m_device(std::make_unique<NICard>()),
      m_lastPhoton(std::nullopt)
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

    for (auto line: m_device->getDigitalOutLines())
        out.push_back(QString::fromStdString(line));

    return out;
}

const QStringList NIDataSource::counters() const
{
    qDebug() << "Get Counters Called";
    QStringList out;

    for (auto counter: m_device->getCounters())
        out.push_back(QString::fromStdString(counter));

    return out;
}

const QStringList NIDataSource::counterLines() const
{
    qDebug() << "Get Counter Lines Called";
    QStringList out;

    for (auto line: m_device->getCounterLines())
        out.push_back(QString::fromStdString(line));

    return out;
}

const QStringList NIDataSource::timebases() const
{
    qDebug() << "Get Timebases Called";
    QStringList out;

    for (auto line: m_device->getTimebases())
        out.push_back(QString::fromStdString(line));

    return out;
}

void NIDataSource::setCurrentDevice(const QString& devName)
{
    qDebug() << "Set Current Device Called";
    m_device = std::make_unique<NICard>(devName.toStdString());
    emit digitalOutLinesChanged();
    emit counterLinesChanged();
    emit timebasesChanged();
    emit countersChanged();
    emit sendValues();
}

void NIDataSource::setDonorLaserPin(const QString& pin)  { m_device->setDonorLaserPin(pin.toStdString()); }
void NIDataSource::setAcceptorLaserPin(const QString& pin) { m_device->setAcceptorLaserPin(pin.toStdString()); }
void NIDataSource::setDonorDetectorCounter(const QString& pin) { m_device->setDonorDetectorCounter(pin.toStdString()); }
void NIDataSource::setAcceptorDetectorCounter(const QString& pin) { m_device->setAcceptorDetectorCounter(pin.toStdString()); }
void NIDataSource::setDonorDetectorPin(const QString& pin) { m_device->setDonorDetectorPin(pin.toStdString()); }
void NIDataSource::setAcceptorDetectorPin(const QString& pin) { m_device->setAcceptorDetectorPin(pin.toStdString()); }
void NIDataSource::setDonorDetectorGate(const QString& pin) { m_device->setDonorDetectorGate(pin.toStdString()); }
void NIDataSource::setAcceptorDetectorGate(const QString& pin) { m_device->setAcceptorDetectorGate(pin.toStdString()); }
void NIDataSource::setTimebase(const QString& pin) { m_device->setTimebase(pin.toStdString()); }
void NIDataSource::setLaserControlResolution(quint32 res) { m_device->setLaserControlResolution(std::chrono::nanoseconds(res)); }
void NIDataSource::setTimestampAdjustment(quint64 val) { m_device->setTimestampAdjustment(val); }

void NIDataSource::setAlexPeriod(quint32 micros) { m_device->setAlexPeriod(std::chrono::microseconds(micros)); }
void NIDataSource::setDonorLaserOffPercentage(quint8 percentage) { m_device->setDonorLaserOffPercentage(percentage); }
void NIDataSource::setDonorLaserOnPercentage(quint8 percentage) { m_device->setDonorLaserOnPercentage(percentage); }
void NIDataSource::setAcceptorLaserOffPercentage(quint8 percentage) { m_device->setAcceptorLaserOffPercentage(percentage); }
void NIDataSource::setAcceptorLaserOnPercentage(quint8 percentage) { m_device->setAcceptorLaserOnPercentage(percentage); }
void NIDataSource::setExperimentLength(quint32 minutes) { m_device->setExperimentLength(std::chrono::minutes(minutes)); }

bool NIDataSource::isRunning() { return m_device->isRunning(); }

bool NIDataSource::startAcquisition()
{
    m_lastPhoton = std::nullopt;

    auto res = m_device->prime();

    if (res.has_value())
    {
        qCritical() << QString::fromStdString(res.value());
        return false;
    }

    res = m_device->start();
    if (res.has_value())
    {
        qCritical() << QString::fromStdString(res.value());
        return false;
    }


    return true;
}

bool NIDataSource::stopAcquisition()
{
    auto res = m_device->stop();
    if (res.has_value())
    {
        qWarning() << QString::fromStdString(res.value());
        return false;
    }


    return true;
}

quint64 NIDataSource::getTotalDonorPhotons() { return m_device->getTotalDonorPhotons(); }
quint64 NIDataSource::getTotalAcceptorPhotons() { return m_device->getTotalAcceptorPhotons(); }

void NIDataSource::updateLiveTrace(QAbstractSeries *DDSeries, QAbstractSeries *AASeries, QAbstractSeries *DASeries, quint64 min_t, quint64 max_t)
{
    using namespace std::chrono;

    if (!m_device->isRunning())
        return;

    auto DDTrace = static_cast<QXYSeries*>(DDSeries);
    auto AATrace = static_cast<QXYSeries*>(AASeries);
    auto DATrace = static_cast<QXYSeries*>(DASeries);

    DDTrace->clear();
    AATrace->clear();
    DATrace->clear();

    QVector<QPointF> DDPoints(static_cast<int>(max_t - min_t));
    QVector<QPointF> AAPoints(static_cast<int>(max_t - min_t));
    QVector<QPointF> DAPoints(static_cast<int>(max_t - min_t));

    auto lock = m_device->getPhotonLockObject();
    lock.lock();

    auto currentPhotons = m_device->getCurrentPhotons(lock); //kinda bypassing the lock mechanism....
    for (auto t = min_t; t <= max_t; t++)
    {
        if (auto itr = currentPhotons.find(t); itr != currentPhotons.end())
        {
            DDPoints.push_back(QPointF(t, static_cast<double>(itr->second.nDD)));
            AAPoints.push_back(QPointF(t, -static_cast<double>(itr->second.nAA)));
            DAPoints.push_back(QPointF(t, -static_cast<double>(itr->second.nDA)));
        }
        else
        {
            DDPoints.push_back(QPointF(t, 0));
            AAPoints.push_back(QPointF(t, 0));
            DAPoints.push_back(QPointF(t, 0));
        }
    }

    lock.unlock();

    DDTrace->replace(DDPoints);
    AATrace->replace(AAPoints);
    DATrace->replace(DAPoints);
}
