#ifndef NICARD_H
#define NICARD_H

#include <vector>
#include <string>
#include <optional>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <future>
#include <atomic>
#include <unordered_map>

#include "photon.h"

#include <NIDAQmx.h>

class NICard
{
public:
    NICard(const std::string& deviceName = "");

    static std::vector<std::string> getAvailableDevices();
    std::vector<std::string> getDigitalOutLines() const;
    std::vector<std::string> getCounters() const;
    std::vector<std::string> getCounterLines() const;
    std::vector<std::string> getTimebases() const;
    std::string getDeviceName() const;
    uint64_t getTotalDonorPhotons() const;
    uint64_t getTotalAcceptorPhotons() const;

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
    void setTimestampAdjustment(uint64_t val);

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


    PhotonLock getPhotonLockObject();
    std::unordered_map<uint64_t, PhotonBlock>& getCurrentBinnedPhotons(const PhotonLock& lock);
    std::list<Photon>& getCurrentPhotons(const PhotonLock& lock);

private:
    std::vector<std::string> getNIStrings(int32(*strFunc)(const char *, char *, uInt32), std::optional<std::string> removeFromBegining = std::nullopt) const;
    static std::vector<std::string> splitNIStrings(std::vector<char>& niStrings, std::optional<std::string> removeFromBegining = std::nullopt);
    static std::optional<std::string> checkNIDAQError(int32 error);

    std::optional<std::string> readPhotonsIntoBuffer(TaskHandle counterTask, std::vector<uInt32>& buff) const;
    std::optional<std::string> analysePhotons(const std::vector<uInt32>& buffer, uInt32& previousValue, uInt64& offset, FluorophoreType detector,
                                              std::list<Photon>& newPhotons, std::unordered_map<uint64_t, PhotonBlock>& newPhotonsBinned);
    std::optional<std::string> readPhotons();

    std::optional<std::string> setupTriggers();
    std::optional<std::string> setupCounters();

    std::optional<std::string> clearTriggers();
    void clearTasks();

    std::mutex m_stopAcquisitionMutex;

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
    uint64_t m_timestampAdjustment;
    std::chrono::nanoseconds m_laserControlResolution;

    std::chrono::microseconds m_alexPeriod;
    DutyCycle m_donorLaserDutyCycle;
    DutyCycle m_acceptorLaserDutyCycle;
    std::chrono::minutes m_experimentLength;

    TaskHandle m_triggerTask;
    uInt32 m_digitalOutputDelay;

    TaskHandle m_donorCounterTask;
    TaskHandle m_acceptorCounterTask;

    bool m_running;
    std::atomic<bool> m_stopReadPhotons;
    std::future<std::optional<std::string>> m_readPhotonsResult;
    std::atomic<uint64_t> m_totalAcceptorPhotons;
    std::atomic<uint64_t> m_totalDonorPhotons;

    PhotonMutex m_detectedPhotonsMutex;
    std::list<Photon> m_photons;
    std::unordered_map<uint64_t, PhotonBlock> m_binnedPhotons;
};

#endif // NICARD_H
