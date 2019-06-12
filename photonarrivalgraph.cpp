#include "photonarrivalgraph.h"

#include <QPainter>
#include <QLineF>
#include <QDebug>

namespace PhotonArrivalGraph {


    PhotonArrivalGraph::PhotonArrivalGraph(QQuickItem *parent)
        : QQuickPaintedItem (parent) { }

    void PhotonArrivalGraph::drawHiso(QPainter *painter, const QRectF &totalArea) {
        painter->save();

        auto graphArea = m_layout.histogramArea(totalArea);

        QLinearGradient backgroundGradient(0.5, 0.0, 0.5, 1.0);
        backgroundGradient.setColorAt(0.0, QRgb(0x2e303a));
        backgroundGradient.setColorAt(1.0, QRgb(0x121218));
        backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);

        painter->setPen(QRgb(0x86878c));
        painter->setBrush(QBrush(backgroundGradient));
        painter->drawRect(graphArea);

        for (int i = 1; i <= m_layout.histogram.nGridLines; i++)
        {
            auto line = QLineF(graphArea.left(),
                               graphArea.top() + (i * graphArea.height()) / (m_layout.histogram.nGridLines + 1),
                               graphArea.right(),
                               graphArea.top() + (i * graphArea.height()) / (m_layout.histogram.nGridLines + 1));
            painter->drawLine(line);
        }

        if (m_nbins == 0) {
            painter->restore();
            return;
        }

        painter->setBrush(QBrush(QColor(0, 0, 0)));
        painter->setPen(QColor(240, 240, 240));
        auto binWidth = graphArea.width() / m_nbins;
        auto max = static_cast<double>(m_dataMax);

        for (size_t i = 0; i < m_nbins; i++)
        {
            auto NN_top = graphArea.bottom() - (m_data[i] / max) * graphArea.height();
            auto DD_top = NN_top - (m_data[i + m_nbins] / max) * graphArea.height() ;
            auto AA_top = DD_top - (m_data[i + 2 * m_nbins] / max) * graphArea.height();
            auto DA_top = AA_top - (m_data[i + 3 * m_nbins] / max) * graphArea.height();

            auto NN_Rect = QRectF(graphArea.left() + i * binWidth,
                    NN_top,
                    binWidth,
                    graphArea.bottom() - NN_top);

            auto DD_Rect = QRectF(graphArea.left() + i * binWidth,
                    DD_top,
                    binWidth,
                    NN_top - DD_top);

            auto AA_Rect = QRectF(graphArea.left() + i * binWidth,
                                  AA_top,
                                  binWidth,
                                  DD_top - AA_top);

            auto DA_Rect = QRectF(graphArea.left() + i * binWidth,
                                  DA_top,
                                  binWidth,
                                  AA_top - DA_top);

            painter->setBrush(getBarGradient(QColor("grey")));
            painter->drawRect(NN_Rect);
            painter->setBrush(getBarGradient(m_DDColor));
            painter->drawRect(DD_Rect);
            painter->setBrush(getBarGradient(m_AAColor));
            painter->drawRect(AA_Rect);
            painter->setBrush(getBarGradient(m_DAColor));
            painter->drawRect(DA_Rect);
        }

        painter->setBrush(QColor(0, 0, 0, 0));
        painter->setPen(QRgb(0x86878c));
        painter->drawRect(graphArea);

        painter->restore();
    }

    void PhotonArrivalGraph::drawLaserIndicators(QPainter *painter, const QRectF &totalArea)
    {
        auto graphArea = m_layout.laserPeriodsArea(totalArea);

        QLinearGradient backgroundGradient(0.5, 0.0, 0.5, 1.0);
        backgroundGradient.setColorAt(0.0, QRgb(0x2e303a));
        backgroundGradient.setColorAt(1.0, QRgb(0x121218));
        backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);

        painter->setPen(QRgb(0x86878c));
        painter->setBrush(QBrush(backgroundGradient));
        painter->drawRect(graphArea);

        auto donorFill = QColor(m_donorLaserColor.red(),
                                m_donorLaserColor.green(),
                                m_donorLaserColor.blue(),
                                64);

        painter->setPen(m_donorLaserColor);
        painter->setBrush(donorFill);
        auto donorLaserRect = QRectF {
                graphArea.left() + m_donorLaserStart * graphArea.width(),
                graphArea.bottom() - 0.9 * graphArea.height(),
                graphArea.width() * (m_donorLaserEnd - m_donorLaserStart),
                graphArea.height() * 0.9
        };


        auto acceptorFill = QColor(m_acceptorLaserColor.red(),
                                   m_acceptorLaserColor.green(),
                                   m_acceptorLaserColor.blue(),
                                   64);

        painter->drawRect(donorLaserRect);

        painter->setPen(m_acceptorLaserColor);
        painter->setBrush(acceptorFill);

        auto acceptorLaserRect = QRectF {
                graphArea.left() + m_acceptorLaserStart * graphArea.width(),
                graphArea.bottom() - 0.9 * graphArea.height(),
                graphArea.width() * (m_acceptorLaserEnd - m_acceptorLaserStart),
                graphArea.height() * 0.9
        };

        painter->drawRect(acceptorLaserRect);

        painter->setPen(QRgb(0x86878c));
        painter->setBrush(QColor(0, 0, 0, 0));
        painter->drawRect(graphArea);


        auto donorCalloutPen = QPen(m_donorLaserColor);
        donorCalloutPen.setStyle(Qt::PenStyle::DashLine);

        auto acceptorCalloutPen = QPen(m_acceptorLaserColor);
        acceptorCalloutPen.setStyle(Qt::PenStyle::DashLine);

        painter->setPen(donorCalloutPen);
        painter->drawLine(QLineF(graphArea.left() + m_donorLaserStart * graphArea.width(),
                                graphArea.bottom(),
                                graphArea.left() + m_donorLaserStart * graphArea.width(),
                                m_layout.histogramBottom(totalArea)));

        painter->drawLine(QLineF(graphArea.left() + m_donorLaserEnd * graphArea.width(),
                                graphArea.bottom(),
                                graphArea.left() + m_donorLaserEnd * graphArea.width(),
                                m_layout.histogramBottom(totalArea)));


        painter->setPen(acceptorCalloutPen);
        painter->drawLine(QLineF(graphArea.left() + m_acceptorLaserStart * graphArea.width(),
                                graphArea.bottom(),
                                graphArea.left() + m_acceptorLaserStart * graphArea.width(),
                                m_layout.histogramBottom(totalArea)));

        painter->drawLine(QLineF(graphArea.left() + m_acceptorLaserEnd * graphArea.width(),
                                graphArea.bottom(),
                                graphArea.left() + m_acceptorLaserEnd * graphArea.width(),
                                m_layout.histogramBottom(totalArea)));
    }

    void PhotonArrivalGraph::paint(QPainter *painter) {
        auto totalArea = m_layout.margins.area(width(), height());

        drawHiso(painter, totalArea);

        drawLaserIndicators(painter, totalArea);

        drawLegend(painter, totalArea);
    }

    void PhotonArrivalGraph::updateData(std::vector<uint32_t> data, uint32_t nBins) {
        m_data = std::move(data);
        m_nbins = nBins;

        auto NN_start = m_data.begin();
        auto DD_start = std::next(NN_start, nBins);
        auto AA_start = std::next(DD_start, nBins);
        auto DA_start = std::next(AA_start, nBins);

        m_dataMax = 0;
        for (int i = 0; i < static_cast<int>(nBins); i++) {
            auto total = *std::next(NN_start, i) +  *std::next(DD_start, i) + *std::next(AA_start, i) + *std::next(DA_start, i);
            m_dataMax = std::max(m_dataMax, total);
        }

        update();
    }

    void PhotonArrivalGraph::drawLegend(QPainter *painter, const QRectF& totalArea) {
        painter->save();

        auto legendArea = m_layout.legendArea(totalArea);

        QFont legendFont;
        legendFont.setPixelSize(static_cast<int>(m_layout.legend.textHeight));
        painter->setFont(legendFont);

        QFontMetricsF fm(legendFont);
        auto DDWidth = fm.width("DD");
        auto AAWidth = fm.width("AA");
        auto DAWidth = fm.width("DA");
        auto NNWidth = fm.width("Noise");
        auto textWidth = DDWidth + AAWidth + DAWidth + NNWidth;

        auto totalWidth = m_layout.legend.boxSize * 4 + textWidth + m_layout.legend.textGap * 7;

        auto x = (legendArea.width() - totalWidth) / 2.0;

        painter->setBrush(m_DDColor);
        painter->setPen(QColor(255, 255, 255));

        auto square = QRectF(0, 0, m_layout.legend.boxSize, m_layout.legend.boxSize);
        square.moveCenter(QPointF(x + m_layout.legend.boxSize / 2.0, legendArea.center().y()));

        painter->drawRect(square);
        x += m_layout.legend.boxSize + m_layout.legend.textGap;

        painter->drawText(QPointF(x, legendArea.bottom() - 1), "DD");
        x += DDWidth + m_layout.legend.textGap;

        painter->setBrush(m_AAColor);
        square.moveCenter(QPointF(x + m_layout.legend.boxSize /2.0, legendArea.center().y()));
        painter->drawRect(square);
        x += m_layout.legend.boxSize + m_layout.legend.textGap;

        painter->drawText(QPointF(x, legendArea.bottom() - 1), "AA");
        x += AAWidth + m_layout.legend.textGap;

        painter->setBrush(m_DAColor);
        square.moveCenter(QPointF(x + m_layout.legend.boxSize /2.0, legendArea.center().y()));
        painter->drawRect(square);
        x += m_layout.legend.boxSize + m_layout.legend.textGap;

        painter->drawText(QPointF(x, legendArea.bottom() - 1), "DA");
        x += DAWidth + m_layout.legend.textGap;

        painter->setBrush(m_NoiseColor);
        square.moveCenter(QPointF(x + m_layout.legend.boxSize /2.0, legendArea.center().y()));
        painter->drawRect(square);
        x += m_layout.legend.boxSize + m_layout.legend.textGap;

        painter->drawText(QPointF(x, legendArea.bottom() - 1), "Noise");
        x += NNWidth + m_layout.legend.textGap;

        painter->restore();
    }

    void PhotonArrivalGraph::addTestData() {
        auto dist = std::uniform_int_distribution(0, 10);

        if (m_data.size() == 0)
            m_data = std::vector<uint32_t>(300);

        for (size_t i = 0; i < 100; i++)
        {
            if (i >= 100 * m_donorLaserStart && i < 100 * m_donorLaserEnd) {
                m_data[i] += static_cast<uint32_t>(dist(rng));
                m_data[i + 200] += static_cast<uint32_t>(dist(rng));
            }
            if (i >= 100 * m_acceptorLaserStart && i < 100 * m_acceptorLaserEnd) {
                m_data[i + 100] += 2*static_cast<uint32_t>(dist(rng));
            }
        }

        updateData(m_data, 100);
    }

    QLinearGradient PhotonArrivalGraph::getBarGradient(const QColor& color) {
        QLinearGradient g(0.5, 0, 0.5, 1.0);
        qreal h = color.hsvHueF();
        qreal s = color.hsvSaturationF();
        qreal v = color.valueF();

        QColor start = color, end = color;

        start.setHsvF(h, 0.0, 1.0);
        end.setHsvF(h, s, v * 0.7);

        g.setColorAt(0.0, start);
        g.setColorAt(0.5, color);
        g.setColorAt(1.0, end);
        g.setCoordinateMode(QGradient::CoordinateMode::StretchToDeviceMode);

        return g;
    }
}
