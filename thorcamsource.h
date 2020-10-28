#ifndef THORCAMSOURCE_H
#define THORCAMSOURCE_H

#include <QObject>
#include <QStringList>
#include <QDebug>
#include <QVector>
#include <exception>

#include "thorcam.h"
#include "byteimage.h"

namespace ThorCam {

    class ThorCamSource : public QObject {
        Q_OBJECT
        Q_PROPERTY(QStringList availableCameras READ getAvailableCameras NOTIFY onAvailableCamerasChanged)
        Q_PROPERTY(qreal exposureMin READ getExposureMin NOTIFY onExposureRangeChanged)
        Q_PROPERTY(qreal exposureMax READ getExposureMax NOTIFY onExposureRangeChanged)
        Q_PROPERTY(qreal exposure READ getExposure WRITE setExposure NOTIFY onExposureChanged)
        Q_PROPERTY(qreal exposureRangeMax READ getExposureRangeMax WRITE setExposureRangeMax NOTIFY onExposureRangeMaxChanged)
    public:
        explicit ThorCamSource(QObject *parent = nullptr);

        QStringList getAvailableCameras();

        qreal getExposureRangeMax();
        void setExposureRangeMax(qreal max);

        qreal getExposureMin() const;
        qreal getExposureMax() const;
        qreal getExposure() const;
        void setExposure(qreal exposure);

    signals:
        void onAvailableCamerasChanged();
        void onExposureChanged();
        void onExposureRangeChanged();
        void onExposureRangeMaxChanged();

    public slots:
        void idChanged(qint32 newID);
        bool start();
        bool stop();
        void snap(ByteImage *imgDisp);
        void refreshCameras();
    private:
        ThorCam m_cam;
        qreal m_exposureRangeMax;
    };

}

#endif // THORCAMSOURCE_H
