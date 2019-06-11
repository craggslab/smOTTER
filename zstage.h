#ifndef ZSTAGE_H
#define ZSTAGE_H

#include <QObject>

class ZStage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableCOMPorts READ getAvailableCOMPorts NOTIFY onAvailableCOMPortsChanged)
public:
    explicit ZStage(QObject *parent = nullptr);

    QStringList getAvailableCOMPorts();

signals:
    void onAvailableCOMPortsChanged();

public slots:
    void refreshCOMPorts();

};

#endif // ZSTAGE_H
