#ifndef NICARD_H
#define NICARD_H

#include <vector>
#include <string>
#include <optional>
#include <chrono>

#include <NIDAQmx.h>

class NICard
{
public:
    NICard(const std::string& deviceName = "");

    std::vector<std::string> getDigitalOutLines() const;
    std::vector<std::string> getCounters() const;
    std::vector<std::string> getCounterLines() const;
    std::vector<std::string> getTimebases() const;
    std::string getDeviceName() const;

    void setDonorLaserPin(std::string pin);
    void setAcceptorLaserPin(std::string pin);
    void setDonorDetectorCounter(std::string pin);
    void setAcceptorDetectorCounter(std::string pin);
    void setDonorDetectorPin(std::string pin);
    void setAcceptorDetectorPin(std::string pin);
    void setDonorDetectorGate(std::string pin);
    void setAcceptorDetectorGate(std::string pin);
    void setTimebase(std::string pin);
    void setLaserControlResolution(std::chrono::microseconds res);

    std::optional<std::string> prime();
    std::optional<std::string> start();
    std::optional<std::string> stop();

    static std::vector<std::string> getAvailableDevices();

private:
    std::vector<std::string> getNIStrings(int32(*strFunc)(const char *, char *, uInt32), std::optional<std::string> removeFromBegining = std::nullopt) const;
    static std::vector<std::string> splitNIStrings(std::vector<char>& niStrings, std::optional<std::string> removeFromBegining = std::nullopt);
    static std::optional<std::string> checkNIDAQError(int32 error);

    std::string m_deviceName;
    std::string m_donorLaserPin;
    std::string m_acceptorLaserPin;
    std::string m_donorDetectorCounter;
    std::string m_acceptorDetectorCounter;
    std::string m_donorDetectorPin;
    std::string m_acceptorDetectorPin;
    std::string m_donorDetectorGate;
    std::string m_acceptorDetectorGate;
    std::string m_timebase;
    std::chrono::microseconds m_laserControlResolution;

    TaskHandle m_triggerTask;
    TaskHandle m_counterTask;
};

#endif // NICARD_H
