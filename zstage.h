#ifndef ZSTAGE_H
#define ZSTAGE_H

#include <QObject>
#include <QSerialPort>

class ZStage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availableCOMPorts READ getAvailableCOMPorts NOTIFY onAvailableCOMPortsChanged)
    Q_PROPERTY(quint32 zPos READ getZPos WRITE setZPos NOTIFY onZPosChanged)
public:
    explicit ZStage(QObject *parent = nullptr);

    QStringList getAvailableCOMPorts();

    quint32 getZPos();
    void setZPos(quint32 pos);

signals:
    void onAvailableCOMPortsChanged();
    void onZPosChanged();
    void error(QString errMsg);

public slots:
    void refreshCOMPorts();
    bool connect(const QString& port);

private:
    bool readResponse(QString& out, int timeout = 5000);
    bool write(const QString& msg, int timeout = 5000);

    bool m_connected;
    QSerialPort m_port;
};

#endif // ZSTAGE_H
