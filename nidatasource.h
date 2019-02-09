#ifndef NIDATASOURCE_H
#define NIDATASOURCE_H

#include <QObject>
#include <QAbstractSeries>
#include <QStringList>
#include <QString>

#include "nicard.h"

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
public:
    explicit NIDataSource(QQuickView *appViewer, QObject *parent = nullptr);

    const QStringList availableDevices() const;
    const QStringList digitalOutLines() const;
    const QStringList counters() const;
    const QStringList counterLines() const;
    const QStringList timebases() const;

signals:
    void availableDevicesChanged();
    void digitalOutLinesChanged();
    void countersChanged();
    void counterLinesChanged();
    void timebasesChanged();

public slots:
    void updateAvailableDevices();
    void setCurrentDevice(const QString& devName);

private:

    QStringList m_availableDevices;
    QQuickView *m_appViewer;

    NICard m_device;
};

#endif // NIDATASOURCE_H
