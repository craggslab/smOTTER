#ifndef PHOTON_H
#define PHOTON_H

#include <cstdint>
#include <list>
#include <chrono>
#include <shared_mutex>

enum class PhotonType {
    DD,
    AA,
    DA
};

struct Photon
{
    using constPhotonIterator = std::list<Photon>::const_iterator;

    Photon(std::chrono::nanoseconds ts, PhotonType phType)
    {
        timeStamp = ts;
        type = phType;
    }
    std::chrono::nanoseconds timeStamp;
    PhotonType type;
};

struct PhotonBlock
{
    PhotonBlock() : nAA(0), nDD(0), nDA(0) {}

    template<PhotonType pt>
    void addPhoton(Photon::constPhotonIterator& photon)
    {
        photons.push_back(photon);
        if constexpr (pt == PhotonType::DD) nDD++;
        else if constexpr (pt == PhotonType::DA) nDA++;
        else if constexpr (pt == PhotonType::AA) nAA++;
    }

    void combine(PhotonBlock& other)
    {
        photons.splice(photons.end(), other.photons);
        nAA += other.nAA;
        nDD += other.nDD;
        nDA += other.nDA;
    }

    std::list<Photon::constPhotonIterator> photons;
    uint64_t nAA;
    uint64_t nDD;
    uint64_t nDA;
};

enum class FluorophoreType {
    Donor,
    Acceptor
};

struct DutyCycle {
    void setOffPercent(uint8_t percent);
    void setOnPercent(uint8_t percent);
    void setPeriod(std::chrono::microseconds period);

    uint8_t getOffPercent() const;
    uint8_t getOnPercent() const;
    std::chrono::nanoseconds getOffEndTime() const;
    std::chrono::nanoseconds getOnEndTime() const;

    bool isLaserOn(std::chrono::nanoseconds t) const;
private:
    void updateTimings();

    uint8_t off_percent;
    uint8_t on_percent;
    std::chrono::nanoseconds period;

    std::chrono::nanoseconds onePercent;
    std::chrono::nanoseconds offEndTime;
    std::chrono::nanoseconds onEndTime;
};

using PhotonMutex = std::shared_timed_mutex;
using PhotonLock = std::shared_lock<PhotonMutex>;

#endif // PHOTON_H
