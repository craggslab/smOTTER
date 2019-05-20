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
      m_lastSavedPhoton(std::nullopt)
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

const QStringList NIDataSource::analogueInLines() const
{
    qDebug() << "Get Analogue In Called";
    QStringList out;

    for (auto line: m_device->getAnalogueInLines())
        out.push_back(QString::fromStdString(line));

    return out;
}

void NIDataSource::setCurrentDevice(const QString& devName)
{
    qDebug() << "Set Current Device Called";
    m_device = std::make_unique<NICard>(devName.toStdString());
    qDebug() << "Set Current Device end";
    emit digitalOutLinesChanged();
    emit counterLinesChanged();
    emit timebasesChanged();
    emit countersChanged();
    emit analogueInLinesChanged();
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
void NIDataSource::setLaserPowerPin(const QString& pin) { m_device->setLaserPowerPin(pin.toStdString()); }
void NIDataSource::setTimebase(const QString& pin) { m_device->setTimebase(pin.toStdString()); }
void NIDataSource::setTimestampAdjustment(quint64 val) { m_device->setTimestampAdjustment(val); }
void NIDataSource::setLaserControlResolution(quint32 res) {
    m_device->setLaserControlResolution(std::chrono::nanoseconds(res));
    m_exporter.setLaserPowerSampleInterval(std::chrono::nanoseconds(res) * 10);
}

void NIDataSource::setAlexPeriod(quint32 micros) {
    m_device->setAlexPeriod(std::chrono::microseconds(micros));
    m_exporter.setAlexPeriod(std::chrono::microseconds(micros));
}
void NIDataSource::setDonorLaserOffPercentage(quint8 percentage) {
    m_device->setDonorLaserOffPercentage(percentage);
    m_exporter.setLaserOffPercentage<FluorophoreType::Donor>(percentage);
}
void NIDataSource::setDonorLaserOnPercentage(quint8 percentage) {
    m_device->setDonorLaserOnPercentage(percentage);
    m_exporter.setLaserOnPercentage<FluorophoreType::Donor>(percentage);
}
void NIDataSource::setAcceptorLaserOffPercentage(quint8 percentage) {
    m_device->setAcceptorLaserOffPercentage(percentage);
    m_exporter.setLaserOffPercentage<FluorophoreType::Acceptor>(percentage);
}
void NIDataSource::setAcceptorLaserOnPercentage(quint8 percentage) {
    m_device->setAcceptorLaserOnPercentage(percentage);
    m_exporter.setLaserOnPercentage<FluorophoreType::Acceptor>(percentage);
}
void NIDataSource::setExperimentLength(quint32 minutes) {
    m_device->setExperimentLength(std::chrono::minutes(minutes));
    m_exporter.setAcquisitionDuration(std::chrono::minutes(minutes));
}
void NIDataSource::setSaveLaserPowers(bool save) { m_device->setSaveLaserPowers(save); }

void NIDataSource::setFilename(const QString& filename) { m_exporter.setFilename(filename.toStdString()); }
void NIDataSource::setDescription(const QString& description) { m_exporter.setDescription(description.toStdString()); }
void NIDataSource::setDonorExWavelength(float donorExWavelength) { m_exporter.setDonorExWavelength(donorExWavelength*1e-9f); }
void NIDataSource::setAcceptorExWavelength(float acceptorExWavelength) { m_exporter.setAcceptorExWavelength(acceptorExWavelength*1e-9f); }
void NIDataSource::setDonorDetWavelength(float donorDetWavelength) { m_exporter.setDonorDetWavelength(donorDetWavelength*1e-9f); }
void NIDataSource::setAcceptorDetWavelength(float acceptorDetWavelength) { m_exporter.setAcceptorDetWavelength(acceptorDetWavelength*1e-9f); }
void NIDataSource::setDonorInputPower(float donorInputPower) { m_exporter.setDonorInputPower(donorInputPower*1e-3f); }
void NIDataSource::setAcceptorInputPower(float acceptorInputPower) { m_exporter.setAcceptorInputPower(acceptorInputPower*1e-3f); }
void NIDataSource::setDonorDye(const QString& donorDye) { m_exporter.setDonorDye(donorDye.toStdString()); }
void NIDataSource::setAcceptorDye(const QString& acceptorDye) { m_exporter.setAcceptorDye(acceptorDye.toStdString()); }
void NIDataSource::setBufferDesc(const QString& bufferDesc) { m_exporter.setBufferDesc(bufferDesc.toStdString()); }
void NIDataSource::setSampleName(const QString& sampleName) { m_exporter.setSampleName(sampleName.toStdString()); }
void NIDataSource::setUserName(const QString& userName) { m_exporter.setUserName(userName.toStdString()); }
void NIDataSource::setUserAffiliation(const QString& userAffiliation) { m_exporter.setUserAffiliation(userAffiliation.toStdString()); }

QString NIDataSource::getFilename() const {
    return QString::fromStdString(m_exporter.getFilename());
}

bool NIDataSource::isRunning() { return m_device->isRunning(); }

bool NIDataSource::startAcquisition(bool live)
{
    m_lastSavedPhoton = std::nullopt;

    if (!live)
    {
        auto res = m_exporter.createFile();
        if (res.has_value())
        {
            emit error(QString::fromStdString(res.value()));
            return false;
        }
    }

    auto res = m_device->prime(live);
    if (res.has_value())
    {
        emit error(QString::fromStdString(res.value()));
        return false;
    }

    res = m_device->start(live);
    if (res.has_value())
    {
        emit error(QString::fromStdString(res.value()));
        return false;
    }


    return true;
}

bool NIDataSource::stopAcquisition()
{
    auto res = m_device->stop();
    if (res.has_value())
    {
        emit error(QString::fromStdString(res.value()));
        return false;
    }

    return true;
}

qint64 NIDataSource::timeSinceAcqStart()
{
    return m_device->timeSinceAcqStart().count();
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

    QVector<QPointF> DDPoints(static_cast<int>(max_t - min_t));
    QVector<QPointF> AAPoints(static_cast<int>(max_t - min_t));
    QVector<QPointF> DAPoints(static_cast<int>(max_t - min_t));

    auto& store = m_device->getPhotonStore();

    auto lock = store.getReadLockObject();
    if (!lock.try_lock_for(10ms))
        return;

    DDTrace->clear();
    AATrace->clear();
    DATrace->clear();

    for (auto t = min_t; t <= max_t; t++)
    {
        if (auto itr = store.findBin(t, lock); itr != store.binnedPhotons(lock).end())
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

void NIDataSource::updateESHistogram(HexPlot::HexPlot *hexPlot, quint64 threshold_AA, quint64 threshold_DD_DA, quint64 min_t, quint64 max_t) {
    using namespace std::chrono;


    if (!m_device->isRunning())
        return;

    std::list<QPointF> newData;

    auto& store = m_device->getPhotonStore();
    auto lock = store.getReadLockObject();
    if (!lock.try_lock_for(10ms))
        return;


    for (auto t = min_t; t <= max_t; t++)
    {
        if (auto itr = store.findBin(t, lock); itr != store.binnedPhotons(lock).end())
        {
            if (itr->second.nAA < threshold_AA) continue;

            auto DD_DA = itr->second.nDD + itr->second.nDA;
            if (DD_DA < threshold_DD_DA) continue;

            auto DA = static_cast<qreal>(itr->second.nDA);
            auto DD_DA_AA = static_cast<qreal>(DD_DA + itr->second.nAA);

            newData.emplace_back(QPointF{DA / DD_DA, DD_DA / DD_DA_AA});
        }
    }

    lock.unlock();

    if (newData.size() != 0)
        hexPlot->addData(newData);
}

void NIDataSource::saveNewPhotons(bool endOfAcquisition)
{
    auto& store = m_device->getPhotonStore();

    if (m_saveFuture.valid())
    {
        auto [lastSaved, err] = m_saveFuture.get();
        if (err.has_value())
        {
            emit error(QString::fromStdString(err.value()));
        }

        m_lastSavedPhoton = lastSaved;
    }

    m_saveFuture = std::async(std::launch::async, &PhotonHDF5Exporter::savePhotons,
                        &m_exporter, m_lastSavedPhoton, std::ref(store));

    if (endOfAcquisition)
    {
        if (auto [_, res] = m_saveFuture.get(); res.has_value())
            emit error(QString::fromStdString(res.value()));
    }
}

bool NIDataSource::fileExists() {
    return QFile::exists(QString::fromStdString(m_exporter.getFilename()));
}
