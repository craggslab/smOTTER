#include "byteimage.h"

#include <QSGSimpleTextureNode>
#include <QQuickWindow>
#include <QSGSimpleRectNode>

ByteImage::ByteImage()
    : m_imgWidth(1280),
      m_imgHeight(1024),
      m_texture(nullptr, &deleteTexture)
{
    m_imageData = std::vector<quint8>();
    for (size_t y = 0; y < m_imgHeight; y++)
    {
        for (size_t x = 0; x < m_imgWidth; x++)
        {
            bool top = y < 5;
            bool bottom = y > m_imgHeight - 5;
            bool left = x < 5;
            bool right = x > m_imgWidth - 5;

            m_imageData.push_back(top || bottom || left || right ? 0 : 100);
        }
    }

    m_image = QImage(m_imageData.data(), static_cast<qint32>(m_imgWidth), static_cast<qint32>(m_imgHeight), QImage::Format_Grayscale8);

    setFlag(QQuickItem::ItemHasContents);
}

void ByteImage::setImage(const std::vector<uint8_t>& img, quint32 width, quint32 height)
{
    if (m_imgWidth != width || m_imgHeight != height || m_image.format() != QImage::Format_RGB32)
		m_image = QImage(static_cast<qint32>(width), static_cast<qint32>(height), QImage::Format_RGB32);

    auto qimg = m_image.bits();
    auto thorImage = img.data();

    for (int i = 0; i < static_cast<int>(width*height); i++)
    {
		auto idx = i << 2;
        qimg[idx    ] = thorImage[static_cast<size_t>(i)];
        qimg[idx + 1] = thorImage[static_cast<size_t>(i)];
        qimg[idx + 2] = thorImage[static_cast<size_t>(i)];
    }

    if (m_imgWidth != width || m_imgHeight != height)
        emit onAspectRatioChanged();
    m_imgWidth = width;
    m_imgHeight = height;

    update();
}

QSGNode *ByteImage::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData*)
{
    QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode*>(oldNode);
    if (!node) {
        node = new QSGSimpleTextureNode();
    }
    m_texture.reset(window()->createTextureFromImage(m_image));
    node->setTexture(m_texture.get());
    node->setRect(boundingRect());
    return node;
}

void ByteImage::deleteTexture(QSGTexture *tex) {
    if (tex != nullptr)
        tex->deleteLater();
}
