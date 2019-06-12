#ifndef PHOTONARRIVALGRAPH_H
#define PHOTONARRIVALGRAPH_H

#include <QtQuick/QQuickPaintedItem>
#include <QObject>
#include <QRectF>
#include <QGradient>

#include <random>

#define REGISTER_PHOTON_ARRIVAL_PLOT_TYPES(uri, versionMajor, versionMinor) \
    qmlRegisterType<PhotonArrivalGraph::PhotonArrivalGraph>(uri, versionMajor, versionMinor, "PhotonArrivalGraph"); \
    qmlRegisterUncreatableType<PhotonArrivalGraph::PAGraphMargins>(uri, versionMajor, versionMinor, "PAGraphMargins", "Exposes properties only"); \
    qmlRegisterUncreatableType<PhotonArrivalGraph::PAGraphLaserPeriods>(uri, versionMajor, versionMinor, "PAGraphLaserPeriods", "Exposes properties only"); \
    qmlRegisterUncreatableType<PhotonArrivalGraph::PAGraphHisto>(uri, versionMajor, versionMinor, "PAGraphHisto", "Exposes properties only"); \
    qmlRegisterUncreatableType<PhotonArrivalGraph::PAGraphLegend>(uri, versionMajor, versionMinor, "PAGraphLegend", "Exposes properties only"); \
    qmlRegisterUncreatableType<PhotonArrivalGraph::PAGraphLayout>(uri, versionMajor, versionMinor, "PAGraphLayout", "Exposes properties only"); \

namespace PhotonArrivalGraph {

    struct PAGraphMargins : QObject {
        Q_OBJECT
        Q_PROPERTY(qreal top MEMBER top)
        Q_PROPERTY(qreal right MEMBER right)
        Q_PROPERTY(qreal left MEMBER left)
        Q_PROPERTY(qreal bottom MEMBER right)
    public:
        qreal top = 10;
        qreal bottom = 10;
        qreal left = 10;
        qreal right = 10;

        [[nodiscard]] QRectF area(qreal width, qreal height) {
            return QRectF(left, top, width - left - right, height - top - bottom);
        }
    };

    struct PAGraphLaserPeriods : QObject {
        Q_OBJECT
        Q_PROPERTY(qreal bottomGap MEMBER bottomGap)
    public:
        qreal bottomGap = 5;
    };

    struct PAGraphHisto : QObject {
        Q_OBJECT
        Q_PROPERTY(qreal topGap MEMBER topGap)
        Q_PROPERTY(qreal nGridLines MEMBER nGridLines)
    public:
        qreal topGap = 5;
        qreal nGridLines = 3;
    };

    struct PAGraphLegend : QObject {
        Q_OBJECT
        Q_PROPERTY(qreal topGap MEMBER topGap)
        Q_PROPERTY(qreal textGap MEMBER textGap)
        Q_PROPERTY(qreal textHeight MEMBER textHeight)
        Q_PROPERTY(qreal boxSize MEMBER boxSize)
    public:
        qreal topGap = 5;
        qreal textHeight = 11;
        qreal textGap = 5;
        qreal boxSize = 10;
    };

    struct PAGraphLayout : QObject {
        Q_OBJECT
        Q_PROPERTY(PAGraphMargins* margins READ getMargins)
        Q_PROPERTY(PAGraphLaserPeriods* laserPeriods READ getLaserPeriods)
        Q_PROPERTY(PAGraphHisto* histogram READ getHistogram)
        Q_PROPERTY(PAGraphLegend* legend READ getLegend)
    public:
        PAGraphMargins margins;
        PAGraphLaserPeriods laserPeriods;
        PAGraphHisto histogram;
        PAGraphLegend legend;

        PAGraphMargins* getMargins() { return &margins; }
        PAGraphLaserPeriods* getLaserPeriods() { return &laserPeriods; }
        PAGraphHisto* getHistogram() { return &histogram; }
        PAGraphLegend* getLegend() { return &legend; }

        [[nodiscard]] QRectF laserPeriodsArea(const QRectF& totalArea) {
            return QRectF {
                totalArea.x(),
                totalArea.y(),
                totalArea.width(),
                totalArea.height() / 3 - laserPeriods.bottomGap
            };
        }

        [[nodiscard]] QRectF histogramArea(const QRectF& totalArea) {
            return QRectF {
                totalArea.x(),
                totalArea.y() + totalArea.height() / 3 + histogram.topGap,
                totalArea.width(),
                2 * totalArea.height() / 3 - histogram.topGap - legend.topGap - legend.textHeight
            };
        }

        [[nodiscard]] qreal histogramBottom(const QRectF& totalArea) {
            return (totalArea.y() + totalArea.height() / 3 + histogram.topGap +
                    (2 * totalArea.height() / 3 - histogram.topGap - legend.topGap - legend.textHeight));
        }

        [[nodiscard]] QRectF legendArea(const QRectF& totalArea) {
            return QRectF {
                totalArea.x(),
                totalArea.bottom() - legend.textHeight,
                totalArea.width(),
                legend.textHeight
            };
        }
    };

    class PhotonArrivalGraph : public QQuickPaintedItem
    {
        Q_OBJECT
        Q_PROPERTY(PAGraphLayout* plotLayout READ layout)
        Q_PROPERTY(QColor ddColor READ DDColor WRITE setDDColor)
        Q_PROPERTY(QColor aaColor READ AAColor WRITE setAAColor)
        Q_PROPERTY(QColor daColor READ DAColor WRITE setDAColor)
        Q_PROPERTY(QColor noiseColor READ NoiseColor WRITE setNoiseColor)
        Q_PROPERTY(QColor donorLaserColor READ donorLaserColor WRITE setDonorLaserColor)
        Q_PROPERTY(QColor acceptorLaserColor READ acceptorLaserColor WRITE setAcceptorLaserColor)
        Q_PROPERTY(qreal donorLaserStart READ donorLaserStart WRITE setDonorLaserStart)
        Q_PROPERTY(qreal donorLaserEnd READ donorLaserEnd WRITE setDonorLaserEnd)
        Q_PROPERTY(qreal acceptorLaserStart READ acceptorLaserStart WRITE setAcceptorLaserStart)
        Q_PROPERTY(qreal acceptorLaserEnd READ acceptorLaserEnd WRITE setAcceptorLaserEnd)
    public:
        PhotonArrivalGraph(QQuickItem *parent = nullptr);

        void paint(QPainter *painter);
        void updateData(std::vector<uint32_t> data, uint32_t nbins);

        PAGraphLayout* layout() {  return &m_layout; }

        QColor DDColor() const { return m_DDColor; }
        void setDDColor(const QColor &color) { m_DDColor = color; }
        QColor AAColor() const { return m_AAColor; }
        void setAAColor(const QColor &color) { m_AAColor = color; }
        QColor DAColor() const { return m_DAColor; }
        void setDAColor(const QColor &color) { m_DAColor = color; }
        QColor NoiseColor() const { return m_NoiseColor; }
        void setNoiseColor(const QColor &color) { m_NoiseColor = color; }

        QColor donorLaserColor() const { return m_donorLaserColor; }
        void setDonorLaserColor(const QColor &color) { m_donorLaserColor = color; }
        QColor acceptorLaserColor() const { return m_acceptorLaserColor; }
        void setAcceptorLaserColor(const QColor &color) { m_acceptorLaserColor = color; }

        qreal donorLaserStart() const { return m_donorLaserStart; }
        void setDonorLaserStart(qreal start) { m_donorLaserStart = start; update(); }
        qreal donorLaserEnd() const { return m_donorLaserEnd; }
        void setDonorLaserEnd(qreal end) { m_donorLaserEnd = end; update(); }
        qreal acceptorLaserStart() const { return m_acceptorLaserStart; }
        void setAcceptorLaserStart(qreal start) { m_acceptorLaserStart = start; update(); }
        qreal acceptorLaserEnd() const { return m_acceptorLaserEnd; }
        void setAcceptorLaserEnd(qreal end) { m_acceptorLaserEnd = end; update(); }

    public slots:
        void addTestData();
    private:
        void drawHiso(QPainter *painter, const QRectF& totalArea);
        void drawLaserIndicators(QPainter *painter, const QRectF& totalArea);
        void drawLegend(QPainter *painter, const QRectF& totalArea);
        QLinearGradient getBarGradient(const QColor& color);

        PAGraphLayout m_layout;

        QColor m_DDColor = { 0, 255, 0};
        QColor m_AAColor = { 255, 255, 0};
        QColor m_DAColor = { 255, 0, 0 };
        QColor m_NoiseColor = QColor("grey");

        QColor m_donorLaserColor = { 0, 255, 0 };
        QColor m_acceptorLaserColor = { 255, 0, 0 };

        qreal m_donorLaserStart = 0.10;
        qreal m_donorLaserEnd = 0.30;
        qreal m_acceptorLaserStart = 0.50;
        qreal m_acceptorLaserEnd = 0.90;

        std::vector<uint32_t> m_data;
        uint32_t m_dataMax = 0;
        uint32_t m_nbins = 0;

        std::default_random_engine rng;
    };

}

#endif // PHOTONARRIVALGRAPH_H
