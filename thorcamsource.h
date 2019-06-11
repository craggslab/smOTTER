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
    public:
        explicit ThorCamSource(QObject *parent = nullptr);

        QStringList getAvailableCameras();

        qreal getExposureMin() const;
        qreal getExposureMax() const;
        qreal getExposure() const;
        void setExposure(qreal exposure);

    signals:
        void onAvailableCamerasChanged();
        void onExposureChanged();
        void onExposureRangeChanged();

    public slots:
        void idChanged(qint32 newID);
        bool start();
        bool stop();
        void snap(ByteImage *imgDisp);
        void refreshCameras();
    private:
        ThorCam m_cam;
    };

}

#endif // THORCAMSOURCE_H
