#ifndef NICARD_H
#define NICARD_H

#include <vector>
#include <string>
#include <optional>
#include <chrono>
#include <mutex>

#include <NIDAQmx.h>

class NICard
{
public:
    struct DutyCycle {
        void setOffPercent(uint8_t percent);
        void setOnPercent(uint8_t percent);
        void setPeriod(std::chrono::microseconds period);

        bool isLaserOn(std::chrono::nanoseconds t);
    private:
        void updateTimings();

        uint8_t off_percent;
        uint8_t on_percent;
        std::chrono::nanoseconds period;

        std::chrono::nanoseconds onePercent;
        std::chrono::nanoseconds offEndTime;
        std::chrono::nanoseconds onEndTime;
    };

    NICard(const std::string& deviceName = "");

    std::vector<std::string> getDigitalOutLines() const;
    std::vector<std::string> getCounters() const;
    std::vector<std::string> getCounterLines() const;
    std::vector<std::string> getTimebases() const;
    std::string getDeviceName() const;

    /* NIDAQ SETTINGS */
    void setDonorLaserPin(std::string pin);
    void setAcceptorLaserPin(std::string pin);
    void setDonorDetectorCounter(std::string pin);
    void setAcceptorDetectorCounter(std::string pin);
    void setDonorDetectorPin(std::string pin);
    void setAcceptorDetectorPin(std::string pin);
    void setDonorDetectorGate(std::string pin);
    void setAcceptorDetectorGate(std::string pin);
    void setTimebase(std::string pin);
    void setLaserControlResolution(std::chrono::nanoseconds res);

    /* Acquisition Settings */
    void setAlexPeriod(std::chrono::microseconds micros);
    void setDonorLaserOffPercentage(uint8_t percentage);
    void setDonorLaserOnPercentage(uint8_t percentage);
    void setAcceptorLaserOffPercentage(uint8_t percentage);
    void setAcceptorLaserOnPercentage(uint8_t percentage);
    void setExperimentLength(std::chrono::minutes length);

    bool isRunning();

    std::optional<std::string> prime();
    std::optional<std::string> start();
    std::optional<std::string> stop();

    static std::vector<std::string> getAvailableDevices();

private:
    std::vector<std::string> getNIStrings(int32(*strFunc)(const char *, char *, uInt32), std::optional<std::string> removeFromBegining = std::nullopt) const;
    static std::vector<std::string> splitNIStrings(std::vector<char>& niStrings, std::optional<std::string> removeFromBegining = std::nullopt);
    static std::optional<std::string> checkNIDAQError(int32 error);

    std::optional<std::string> setupTriggers();
    std::optional<std::string> setupCounters();

    std::optional<std::string> clearTriggers();
    void clearTasks();

    std::unique_ptr<std::mutex> stopMutex;

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
    std::chrono::nanoseconds m_laserControlResolution;

    std::chrono::microseconds m_alexPeriod;
    DutyCycle m_donorLaserDutyCycle;
    DutyCycle m_acceptorLaserDutyCycle;
    std::chrono::minutes m_experimentLength;

    TaskHandle m_triggerTask;
    TaskHandle m_counterTask;

    bool m_running;
};

#endif // NICARD_H
