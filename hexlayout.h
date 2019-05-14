#ifndef HEXLAYOUT_H
#define HEXLAYOUT_H

#include <QtMath>
#include <QMetaType>
#include <QRectF>
#include <QLineF>
#include <QVector>
#include <QObject>

namespace HexPlot {

    enum class Orientation {
        Horizontal,
        Vertical
    };

    struct HexMargins : QObject {
        Q_OBJECT
        Q_PROPERTY(qreal left MEMBER left)
        Q_PROPERTY(qreal right MEMBER right)
        Q_PROPERTY(qreal top MEMBER top)
        Q_PROPERTY(qreal bottom MEMBER bottom)
    public:
        qreal left = 10;
        qreal right = 10;
        qreal top = 10;
        qreal bottom = 10;

        bool operator==(HexMargins const &other) const {
        return qFuzzyCompare(this->left, other.left) &&
            qFuzzyCompare(this->right, other.right) &&
            qFuzzyCompare(this->top, other.top) &&
            qFuzzyCompare(this->bottom, other.bottom);
        }

        bool operator!=(HexMargins const &other) const {
        return !(*this == other);
        }
    };

    struct Hex1DHistLayout : QObject {
        Q_OBJECT
        Q_PROPERTY(qreal gap MEMBER gap)
        Q_PROPERTY(qreal height MEMBER height)
    public:
        qreal gap = 10;
        qreal height = 50;

        template<Orientation orientation>
        QRectF getBar(const QRectF& area, quint32 nBins, size_t binNum, qreal coefficient) {
        auto binHeight = height * coefficient;

        if constexpr (orientation == Orientation::Horizontal)
        {
            auto binSize = area.width() / nBins;
            return QRectF(area.x() + binSize * binNum, area.y() + height - binHeight, binSize, binHeight);
        }
        else
        {
            auto binSize = area.height() / nBins;
            return QRectF(area.x() + height - binHeight, area.y() + binSize * binNum, binHeight, binSize);
        }
        }

        bool operator==(Hex1DHistLayout const &other) const {
        return qFuzzyCompare(this->gap, other.gap) && qFuzzyCompare(this->height, other.height);
        }

        bool operator!=(Hex1DHistLayout const &other) const {
        return !(*this == other);
        }
    };

    struct HexScalebarLayout : QObject {
        Q_OBJECT
        Q_PROPERTY(qreal gap MEMBER gap)
        Q_PROPERTY(qreal width MEMBER width)
        Q_PROPERTY(qint32 textHeight MEMBER textHeight)
        Q_PROPERTY(qreal textWidth MEMBER textWidth)
        Q_PROPERTY(qreal largeTickSize MEMBER largeTickSize)
        Q_PROPERTY(qreal smallTickSize MEMBER smallTickSize)
        Q_PROPERTY(quint8 ticksEvery MEMBER ticksEvery)
        Q_PROPERTY(quint8 largeTickEvery MEMBER largeTickEvery)
    public:
        qreal gap = 20;
        qreal width = 20;

        qint32 textHeight = 11;
        qreal textWidth = 25;

        qreal largeTickSize = 10;
        qreal smallTickSize = 5;

        quint8 ticksEvery = 2;
        quint8 largeTickEvery = 10;

        QLineF getTick(const QRectF& scalebar, quint8 percent) {
        auto size = percent % largeTickEvery ? smallTickSize : largeTickSize;
        auto inc = scalebar.height() / (ticksEvery * 100.0);
        auto y = scalebar.bottom() - percent * inc;

        return QLineF(scalebar.right(), y,
                  scalebar.right() + size, y);
        }

        QVector<QLineF> getTicks(const QRectF& scalebar) {
        auto out = QVector<QLineF>();
        for (quint8 i = 0; i <= 100; i += ticksEvery) {
            out.push_back(getTick(scalebar, i * ticksEvery));
        }
        return out;
        }

        QRectF getTextPos(const QRectF& scalebar, quint8 percent) {
        auto yPos = scalebar.bottom() -
                percent * scalebar.height() / 100.0 -
                textHeight/2.0;

        return QRectF(scalebar.right() + largeTickSize + 1,
                  yPos, textWidth, textHeight);
        }

        bool operator==(HexScalebarLayout const &other) const {
        return qFuzzyCompare(this->gap, other.gap) &&
            qFuzzyCompare(this->width, other.width) &&
            this->textHeight == other.textHeight &&
            qFuzzyCompare(this->textWidth, other.textWidth) &&
            qFuzzyCompare(this->largeTickSize, other.largeTickSize) &&
            qFuzzyCompare(this->smallTickSize, other.smallTickSize) &&
            this->ticksEvery == other.ticksEvery &&
            this->largeTickEvery == other.largeTickEvery;
        }

        bool operator!=(HexScalebarLayout const &other) const {
        return !(*this == other);
        }

    };

    struct HexAxesLayout : QObject {
        Q_OBJECT
        Q_PROPERTY(quint8 ticksEvery MEMBER ticksEvery)
        Q_PROPERTY(quint8 largeTickEvery MEMBER largeTickEvery)
        Q_PROPERTY(qreal largeTickSize MEMBER largeTickSize)
        Q_PROPERTY(qreal smallTickSize MEMBER smallTickSize)
        Q_PROPERTY(qint32 textHeight MEMBER textHeight)
        Q_PROPERTY(QString x_label MEMBER x_label)
        Q_PROPERTY(QString y_label MEMBER y_label)
        Q_PROPERTY(qreal labelGap MEMBER labelGap)
    public:

        quint8 ticksEvery = 2;
        quint8 largeTickEvery = 20;

        qreal largeTickSize = 10;
        qreal smallTickSize = 5;

        qint32 textHeight = 11;

        QString x_label = "X";
        QString y_label = "Y";
        qreal labelGap = 4;

        template<Orientation orientation>
        QLineF getTick(const QRectF& plotArea, quint8 percent) {
        auto size = percent % largeTickEvery ? smallTickSize : largeTickSize;

        QPointF start, step;
        if constexpr (orientation == Orientation::Vertical)
        {
            start = plotArea.bottomRight() - percent * QPointF{ 0.0, plotArea.height() / (ticksEvery * 100.0) };
            step = { size, 0.0 };
        }
        else
        {
            start = plotArea.bottomLeft() + percent * QPointF{ plotArea.width() / (ticksEvery * 100.0), 0.0 };
            step = { 0.0, size };
        }


        return QLineF(start, start + step);
        }

        template<Orientation orientation>
        QVector<QLineF> getTicks(const QRectF& plotArea) {
        auto out = QVector<QLineF>();
        for (quint8 i = 0; i <= 100; i += ticksEvery) {
            out.push_back(getTick<orientation>(plotArea, i * ticksEvery));
        }
        return out;
        }

        template<Orientation orientation>
        QRectF getTickTextPos(const QRectF& plotArea, quint8 percent) {

        QPointF topLeft;
        if constexpr (orientation == Orientation::Vertical) {
            topLeft = plotArea.bottomRight()
                    - percent * QPointF{ 0.0, plotArea.height() / 100.0 }
                    - QPointF{ -1 - largeTickSize + 1.5 * textHeight, textHeight / 2.0 };
        } else {
            topLeft = plotArea.bottomLeft()
                    + percent * QPointF{ plotArea.width() / 100.0, 0.0 }
                    - QPointF{ 2.0 * textHeight, -1 - largeTickSize };
        }

        return QRectF(topLeft, QSizeF(4 * textHeight, textHeight));
        }

        template<Orientation orientation>
        QRectF getLabelPos(const QRectF& plotArea) {
        if constexpr (orientation == Orientation::Horizontal) {
            return QRectF(plotArea.left(),
                  plotArea.bottom() + largeTickSize + 1 + textHeight + labelGap,
                  plotArea.width(),
                  static_cast<qreal>(textHeight));
        } else {
            return QRectF(plotArea.right() + largeTickSize + 1 + 1.5 * textHeight + labelGap - plotArea.height()/2.0,
                  plotArea.center().y(),
                  plotArea.height(),
                  textHeight);
        }
        }

        bool operator==(HexAxesLayout const &other) const {
        return this->ticksEvery  == other.ticksEvery &&
            this->largeTickEvery == other.largeTickEvery &&
            qFuzzyCompare(this->largeTickSize, other.largeTickSize) &&
            qFuzzyCompare(this->smallTickSize, other.smallTickSize) &&
            this->textHeight == other.textHeight &&
            this->x_label == other.x_label &&
            this->y_label == other.y_label &&
            qFuzzyCompare(this->labelGap, other.labelGap);

        }

        bool operator!=(HexAxesLayout const &other) const {
        return !(*this == other);
        }

    };

    struct HexLayout : QObject {
        Q_OBJECT
        Q_PROPERTY(HexMargins* margins READ getMargins)
        Q_PROPERTY(Hex1DHistLayout* hist READ getHist)
        Q_PROPERTY(HexScalebarLayout* scalebar READ getScalebar)
        Q_PROPERTY(HexAxesLayout* axes READ getAxes)

    public:
        HexMargins margins;
        Hex1DHistLayout hist;
        HexScalebarLayout scalebar;
        HexAxesLayout axes;

        HexMargins* getMargins() { return &margins; }
        Hex1DHistLayout* getHist() { return &hist; }
        HexScalebarLayout* getScalebar() { return &scalebar; }
        HexAxesLayout* getAxes() { return &axes; }

        QRectF getPlotArea(qreal width, qreal height, quint32 nBinsX, quint32 nBinsY, qreal& R, qreal& r) {
        auto maxPlotArea = QRectF(margins.left + hist.height + hist.gap,
                      margins.top + hist.height + hist.gap,
                      width - margins.right - margins.left - hist.height - hist.gap - scalebar.gap - scalebar.width - scalebar.textWidth - 2 - axes.labelGap - axes.largeTickSize - axes.textHeight * 2,
                      height - margins.top - margins.bottom - hist.height - hist.gap - axes.largeTickSize - axes.textHeight * 2 - axes.labelGap);


        auto ratio = qSqrt(3.0)/2.0;
        R = maxPlotArea.height() / ( ( nBinsY - 1 ) * 1.5 + 2.0 );
        r = maxPlotArea.width() / ( nBinsX * 2.0 + 1.0 );

        if (r > R * ratio) r = R * ratio;
        else R = r / ratio;

        auto totalHexWidth = r * (2 * nBinsX + 1);
        auto totalHexHeight = ((nBinsY - 1) * 1.5 + 2.0) * R;
        return QRectF((maxPlotArea.width() - totalHexWidth)/2.0 + maxPlotArea.x(),
                  (maxPlotArea.height() - totalHexHeight)/2.0 + maxPlotArea.y(),
                  totalHexWidth,
                  totalHexHeight);
        }

        template<Orientation orientation>
        QRectF getHistoArea(const QRectF& plotArea) {
        if constexpr (orientation == Orientation::Horizontal)
            return QRectF(plotArea.left(),
                  plotArea.top() - hist.gap - hist.height,
                  plotArea.width(),
                  hist.height);
        else
            return QRectF(plotArea.left() - hist.height - hist.gap,
                  plotArea.top(),
                  hist.height,
                  plotArea.height());
        }

        QRectF getScalebarArea(const QRectF& plotArea) {
        return QRectF(plotArea.right() + axes.largeTickSize + 2 * axes.textHeight + scalebar.gap,
                  plotArea.top(),
                  scalebar.width,
                  plotArea.height());
        }

    };
}

#endif // HEXLAYOUT_H
