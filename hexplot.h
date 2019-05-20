#ifndef HEXPLOT_H
#define HEXPLOT_H

#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QLineF>

#include <vector>
#include <list>
#include <random>
#include <memory>

#include "hexlayout.h"

#define REGISTER_HEX_PLOT_TYPES(uri, versionMajor, versionMinor) \
    qmlRegisterType<HexPlot::HexPlot>(uri, versionMajor, versionMinor, "HexPlot"); \
    qmlRegisterUncreatableType<HexPlot::HexLayout>(uri, versionMajor, versionMinor, "HexLayout", "Exposes properties only"); \
    qmlRegisterUncreatableType<HexPlot::HexMargins>(uri, versionMajor, versionMinor, "HexMargins", "Exposes properties only"); \
    qmlRegisterUncreatableType<HexPlot::Hex1DHistLayout>(uri, versionMajor, versionMinor, "Hex1DHistLayout", "Exposes properties only"); \
    qmlRegisterUncreatableType<HexPlot::HexScalebarLayout>(uri, versionMajor, versionMinor, "HexScalebarLayout", "Exposes properties only"); \
    qmlRegisterUncreatableType<HexPlot::HexAxesLayout>(uri, versionMajor, versionMinor, "HexAxesLayout", "Exposes properties only");


namespace HexPlot {

    class HexPlot : public QQuickPaintedItem
    {
        Q_OBJECT
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(QColor color READ color WRITE setColor)
        Q_PROPERTY(quint32 nBinsX READ nBinsX WRITE setNBinsX)
        Q_PROPERTY(quint32 nBinsY READ nBinsY WRITE setNBinsY)
        Q_PROPERTY(HexLayout* plotLayout READ layout /*WRITE setLayout*/)
        Q_PROPERTY(quint32 initialScaleMax READ initialScaleMax WRITE setInitialScaleMax)

    public:
        HexPlot(QQuickItem *parent = nullptr);

        QString name() const;
        void setName(const QString &name);

        QColor color() const;
        void setColor(const QColor &color);

        quint32 nBinsX() const;
        void setNBinsX(quint32 bins);

        quint32 nBinsY() const;
        void setNBinsY(quint32 bins);

        quint32 initialScaleMax() const;
        void setInitialScaleMax(quint32 max);

        HexLayout* layout();

        void paint(QPainter *painter);

        void addData(std::list<QPointF> &data);

    public slots:
        void addPoints();
        void clearData();

    private:
        void drawHex(QPointF pos, qreal R, QPainter *painter, quint32 value, quint32 max = 255);
        void draw1DHisto(QPainter *painter, const QRectF& actualPlotArea, bool horizontal);
        void updateBins(std::list<QPointF>::iterator begin, std::list<QPointF>::iterator end);
        void drawScalebar(QPainter *painter, const QRectF& plotArea);
        void drawAxes(QPainter *painter, const QRectF& plotArea);
        void drawTextRotated(QPainter *painter, QRectF textRect, const QString& str, qreal rot);
        QColor getColor(quint32 value, quint32 max);

        HexLayout m_layout;

        QString m_name;
        QColor m_color;

        QColor m_borderColor = QColor(120, 120, 120, 255);
        //QColor m_tickColor = QColor(120, 120, 120);
        QColor m_textColor = QColor(230, 230, 230);

        quint32 m_nBinsX;
        quint32 m_nBinsY;

        std::list<QPointF> m_data;
        std::vector<quint32> m_bins;

        quint32 m_initialScaleMax = 20;
        quint32 m_scaleMax;

        // DELETE ME
        std::default_random_engine rnd;
    };


}

#endif // HEXPLOT_H
