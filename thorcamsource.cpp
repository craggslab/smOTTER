#include "thorcamsource.h"

namespace ThorCam {

    ThorCamSource::ThorCamSource(QObject*) : m_exposureRangeMax(10.0) {}

    QStringList ThorCamSource::getAvailableCameras() {
        auto ids = ThorCam::getAvailableIDs();

        QStringList out;
        for (auto id : ids)
        {
        out.push_back(QString::fromStdString(id));
        }

        return out;
    }

    qreal ThorCamSource::getExposureMin() const {
        if (!m_cam.isConnected()) return 0.0;
        else return m_cam.getExposureMin();
    }

    qreal ThorCamSource::getExposureMax() const {
        if (!m_cam.isConnected()) return m_exposureRangeMax;
        else return std::min<>(m_exposureRangeMax, m_cam.getExposureMax());
    }

    qreal ThorCamSource::getExposure() const {
        if (!m_cam.isConnected()) return 0.0;
        else return m_cam.getExposure();
    }

    void ThorCamSource::setExposure(qreal exposure) {
        if (m_cam.isConnected()) m_cam.setExposure(exposure);
    }

    void ThorCamSource::idChanged(qint32 newID) {
        if (!m_cam.isID(newID))
        m_cam = ThorCam(newID);

        emit onExposureRangeChanged();
        emit onExposureChanged();
    }

    bool ThorCamSource::start() {
        try {
            if (m_cam.isConnected()) {
                m_cam.start();
                return true;
            }
        } catch (std::exception& ex) {
            qDebug() << ex.what();
        }

        return false;
    }

    bool ThorCamSource::stop() {
        try {
            if (m_cam.isConnected()) {
                m_cam.stop();
                return true;
            }
        } catch (std::exception &ex) {
            qDebug() << ex.what();
        }

        return false;
    }

    void ThorCamSource::snap(ByteImage *imgDisp) {
        try {
            if (m_cam.isConnected()) {
                m_cam.snap();
                imgDisp->setImage(m_cam.getImage(), m_cam.getWidth(), m_cam.getHeight());
            }
        } catch (std::exception &ex) {
            qDebug() << ex.what();
        }
    }

    void ThorCamSource::refreshCameras() {
        qDebug() << "Refresh";
        emit onAvailableCamerasChanged();
    }

    qreal ThorCamSource::getExposureRangeMax() { return m_exposureRangeMax; }
    void ThorCamSource::setExposureRangeMax(qreal max)
    {
        m_exposureRangeMax = max;
        emit onExposureRangeChanged();
    }
}
