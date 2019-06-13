#ifndef NIDATASOURCE_H
#define NIDATASOURCE_H

#include <QObject>
#include <QAbstractSeries>
#include <QStringList>
#include <QString>
#include <QBarSet>
#include <QValueAxis>
#include <QtCharts/QLegend>

#include <mutex>
#include <optional>
#include <chrono>

#include "photon.h"
#include "photonhdf5exporter.h"
#include "nicard.h"
#include "hexplot.h"
#include "photonarrivalgraph.h"

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
    Q_PROPERTY(QStringList analogueInLines READ analogueInLines NOTIFY analogueInLinesChanged)

public:
    explicit NIDataSource(QQuickView *appViewer, QObject *parent = nullptr);

    const QStringList availableDevices() const;
    const QStringList digitalOutLines() const;
    const QStringList counters() const;
    const QStringList counterLines() const;
    const QStringList timebases() const;
    const QStringList analogueInLines() const;

signals:
    void availableDevicesChanged();
    void digitalOutLinesChanged();
    void countersChanged();
    void counterLinesChanged();
    void timebasesChanged();
    void analogueInLinesChanged();
    void sendValues();
    void error(const QString& msg);

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
    void setLaserPowerPin(const QString& pin);
    void setTimebase(const QString& pin);
    void setLaserControlResolution(quint32 res);
    void setTimestampAdjustment(quint64 val);

    void setAlexPeriod(quint32 micros);
    void setDonorLaserOffPercentage(quint8 percentage);
    void setDonorLaserOnPercentage(quint8 percentage);
    void setAcceptorLaserOffPercentage(quint8 percentage);
    void setAcceptorLaserOnPercentage(quint8 percentage);
    void setExperimentLength(quint32 minutes);
    void setSaveLaserPowers(bool save);

    void setFilename(const QString& filename);
    void setDescription(const QString& description);
    void setDonorExWavelength(float donorExWavelength);
    void setAcceptorExWavelength(float acceptorExWavelength);
    void setDonorDetWavelength(float donorDetWavelength);
    void setAcceptorDetWavelength(float acceptorDetWavelength);
    void setDonorInputPower(float donorInputPower);
    void setAcceptorInputPower(float acceptorInputPower);
    void setDonorDye(const QString& donorDye);
    void setAcceptorDye(const QString& acceptorDye);
    void setBufferDesc(const QString& bufferDesc);
    void setSampleName(const QString& sampleName);
    void setUserName(const QString& userName);
    void setUserAffiliation(const QString& userAffiliation);


    QString getFilename() const;

    bool isRunning();
    bool startAcquisition(bool live = false);
    bool stopAcquisition();
    qint64 timeSinceAcqStart();

    quint64 getTotalDonorPhotons();
    quint64 getTotalAcceptorPhotons();

    void removeSeriesFromLegend(QLegend* chart, QAbstractSeries *series);
    void updateLiveTrace(QAbstractSeries *DDSeries, QAbstractSeries *AASeries, QAbstractSeries *DASeries, QAbstractSeries *currentPosition, quint64 min_t, quint64 max_t, quint64 total, qreal y_min, qreal y_max);
    void updateAlignmentTrace(QAbstractSeries* donor, QAbstractSeries* acceptor, QAbstractSeries* currentPosition, quint32 binning, quint64 min_t, quint64 max_t, quint64 total, qreal y_min, qreal y_max);
    void updateESHistogram(HexPlot::HexPlot *hexPlot, quint64 threshold_AA, quint64 threshold_DD_DA, quint64 min_t, quint64 max_t);
    void updatePhotonArrivalTimes(PhotonArrivalGraph::PhotonArrivalGraph* pag);

    void saveNewPhotons(bool endOfAcquisition);
    bool fileExists();

private:
    QStringList m_availableDevices;
    QQuickView *m_appViewer;

    std::unique_ptr<NICard> m_device;
    std::future<PhotonHDF5Exporter::SaveResult> m_saveFuture;
    std::optional<PhotonStore::ConstPhotonIterator> m_lastSavedPhoton;
    PhotonHDF5Exporter m_exporter;
};

#endif // NIDATASOURCE_H
