#ifndef BYTEIMAGE_H
#define BYTEIMAGE_H

#include <QQuickItem>
#include <QImage>
#include <QSGTexture>
#include <memory>
#include <vector>

class ByteImage : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal aspectRatio READ getAspectRatio NOTIFY onAspectRatioChanged)
public:
    ByteImage();

    void setImage(const std::vector<uint8_t>& img, quint32 width, quint32 height);

    QSGNode *updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData*) override;

    qreal getAspectRatio() { return static_cast<qreal>(m_imgWidth) / static_cast<qreal>(m_imgHeight); }

signals:
    void onAspectRatioChanged();

private:
    static void deleteTexture(QSGTexture *tex);

    quint32 m_imgWidth;
    quint32 m_imgHeight;
    std::vector<quint8> m_imageData;
    QVector<QRgb> m_lut;
    QImage m_image;
    std::unique_ptr<QSGTexture, decltype (&deleteTexture)> m_texture;
};

#endif // BYTEIMAGE_H
