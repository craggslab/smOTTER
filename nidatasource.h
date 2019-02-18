#ifndef NIDATASOURCE_H
#define NIDATASOURCE_H

#include <QObject>
#include <QAbstractSeries>
#include <QStringList>
#include <QString>
#include <mutex>
#include <optional>
#include <chrono>

#include "photon.h"
#include "nicard.h"

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class NIDataSource : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList availableDevices READ availableDevices NOTIFY availableDevicesChanged)
    Q_PROPERTY(QStringList digitalOutLines READ digitalOutLines NOTIFY digitalOutLinesChanged)
    Q_PROPERTY(QStringList counters READ counters NOTIFY countersChanged)
    Q_PROPERTY(QStringList counterLines READ counterLines NOTIFY counterLinesChanged)
    Q_PROPERTY(QStringList timebases READ timebases NOTIFY timebasesChanged)
public:
    explicit NIDataSource(QQuickView *appViewer, QObject *parent = nullptr);


    const QStringList availableDevices() const;
    const QStringList digitalOutLines() const;
    const QStringList counters() const;
    const QStringList counterLines() const;
    const QStringList timebases() const;



signals:
    void availableDevicesChanged();
    void digitalOutLinesChanged();
    void countersChanged();
    void counterLinesChanged();
    void timebasesChanged();
    void sendValues();

public slots:
    void updateAvailableDevices();
    void setCurrentDevice(const QString& devName);

    void setDonorLaserPin(const QString& pin);
    void setAcceptorLaserPin(const QString& pin);
    void setDonorDetectorCounter(const QString& pin);
    void setAcceptorDetectorCounter(const QString& pin);
    void setDonorDetectorPin(const QString& pin);
    void setAcceptorDetectorPin(const QString& pin);
    void setDonorDetectorGate(const QString& pin);
    void setAcceptorDetectorGate(const QString& pin);
    void setTimebase(const QString& pin);
    void setLaserControlResolution(quint32 res);
    void setTimestampAdjustment(quint64 val);

    void setAlexPeriod(quint32 micros);
    void setDonorLaserOffPercentage(quint8 percentage);
    void setDonorLaserOnPercentage(quint8 percentage);
    void setAcceptorLaserOffPercentage(quint8 percentage);
    void setAcceptorLaserOnPercentage(quint8 percentage);
    void setExperimentLength(quint32 minutes);

    bool isRunning();
    bool startAcquisition();
    bool stopAcquisition();

    quint64 getTotalDonorPhotons();
    quint64 getTotalAcceptorPhotons();

    void updateLiveTrace(QAbstractSeries *DDSeries, QAbstractSeries *AASeries, QAbstractSeries *DASeries, quint64 min_t, quint64 max_t);
private:
    QStringList m_availableDevices;
    QQuickView *m_appViewer;

    std::unique_ptr<NICard> m_device;
    std::optional<std::list<Photon>::const_iterator> m_lastPhoton;
};

#endif // NIDATASOURCE_H
