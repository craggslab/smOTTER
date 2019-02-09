#include "nidatasource.h"

#include <QDebug>

NIDataSource::NIDataSource(QQuickView *appViewer, QObject *parent)
    : QObject(parent),
      m_appViewer(appViewer)
{
    updateAvailableDevices();
}

void NIDataSource::updateAvailableDevices()
{
    qDebug() << "Update Available Devices Called";

    m_availableDevices = QStringList();

    for (auto card: NICard::getAvailableDevices())
        m_availableDevices.push_back(QString::fromStdString(card));

    emit availableDevicesChanged();
}

const QStringList NIDataSource::availableDevices() const
{
    return m_availableDevices;
}

const QStringList NIDataSource::digitalOutLines() const
{
    qDebug() << "Get Digital Out Lines Called";
    QStringList out;

    for (auto line: m_device.getDigitalOutLines())
        out.push_back(QString::fromStdString(line));

    return out;
}

const QStringList NIDataSource::counters() const
{
    qDebug() << "Get Counters Called";
    QStringList out;

    for (auto counter: m_device.getCounters())
        out.push_back(QString::fromStdString(counter));

    return out;
}

const QStringList NIDataSource::counterLines() const
{
    qDebug() << "Get Counter Lines Called";
    QStringList out;

    for (auto line: m_device.getCounterLines())
        out.push_back(QString::fromStdString(line));

    return out;
}

const QStringList NIDataSource::timebases() const
{
    qDebug() << "Get Timebases Called";
    QStringList out;

    for (auto line: m_device.getTimebases())
        out.push_back(QString::fromStdString(line));

    return out;
}

void NIDataSource::setCurrentDevice(const QString& devName)
{
    qDebug() << "Set Current Device Called";
    m_device = NICard(devName.toStdString());
    emit digitalOutLinesChanged();
    emit counterLinesChanged();
    emit timebasesChanged();
    emit countersChanged();
}

