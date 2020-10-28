#ifndef PHOTON_H
#define PHOTON_H

#include <cstdint>
#include <list>
#include <chrono>
#include <shared_mutex>
#include <array>
#include <unordered_map>

enum class PhotonType {
    DD,
    AA,
    DA
};

struct Photon;
struct PhotonBlock;

class PhotonStore {
public:
    using PhotonMutex = std::shared_timed_mutex;
    using PhotonLock = std::shared_lock<PhotonMutex>;
    using PhotonWriteLock = std::unique_lock<PhotonMutex>;
    using ConstPhotonIterator = std::list<Photon>::const_iterator;
    using ConstPhotonBlockIterator = std::unordered_map<uint64_t, PhotonBlock>::const_iterator;

    using LaserPowerMutex = std::shared_timed_mutex;
    using LaserPowerLock = std::shared_lock<LaserPowerMutex>;
    using LaserPowerWriteLock = std::unique_lock<LaserPowerMutex>;
    using ConstLaserPowerIterator = std::list<double>::const_iterator;

    static constexpr size_t nPhotonArrivalBins = 100;
    using PhotonArrivalMutex = std::shared_timed_mutex;
    using PhotonArrivalLock = std::shared_lock<PhotonArrivalMutex>;
    using PhotonArrivalWriteLock = std::unique_lock<PhotonArrivalMutex>;
    using PhotonArrivalType = std::array<uint32_t, 4*nPhotonArrivalBins>;
    using ConstPhotonArrivalIterator = PhotonArrivalType::const_iterator;

    template<PhotonType photonType>
    static constexpr size_t photonArrivalArrayOffset() {
        if constexpr (photonType == PhotonType::DD) return nPhotonArrivalBins;
        else if constexpr (photonType == PhotonType::AA) return 2 * nPhotonArrivalBins;
        else return 3 * nPhotonArrivalBins;
    }

    template<typename T>
    struct IteratorStruct {
        friend class PhotonStore;

        T begin() { return m_begin; }
        T end() { return m_end; }
    private:
        IteratorStruct(T begin, T end)
            : m_begin(begin), m_end(end) { }

        T m_begin;
        T m_end;
    };


    PhotonStore() = default;
    PhotonStore(const PhotonStore& other) = delete;
    PhotonStore& operator=(const PhotonStore&) = delete;

    PhotonLock getReadLockObject();
    PhotonWriteLock getWriteLockObject();

    LaserPowerLock getLaserPowerLockObject();
    LaserPowerWriteLock getLaserPowerWriteLockObject();

    PhotonArrivalLock getPhotonArrivalLockObject();
    PhotonArrivalWriteLock getPhotonArrivalWriteLockObject();

    IteratorStruct<ConstPhotonIterator> photons(const PhotonLock& lock) const;
    IteratorStruct<ConstPhotonBlockIterator> binnedPhotons(const PhotonLock& lock) const;
    ConstPhotonBlockIterator findBin(uint64_t bin, const PhotonLock& lock) const;
    IteratorStruct<ConstPhotonArrivalIterator> photonArrivalTimes(const PhotonArrivalLock& lock) const;

    IteratorStruct<ConstLaserPowerIterator> laserPowers(const LaserPowerLock& lock) const;

    void spliceNewPhotons(std::list<Photon>& newPhotons, const PhotonWriteLock& lock);
    void combinePhotonBlock(uint64_t bin, PhotonBlock& block, const PhotonWriteLock& lock);
    void spliceNewLaserPowers(std::list<double>& newLaserPowers, const LaserPowerWriteLock& lock);
    void spliceLaserPowersTo(std::list<double>& target, const LaserPowerWriteLock& lock);
    void clear(const PhotonWriteLock& ph_lock, const LaserPowerWriteLock& lp_lock, const PhotonArrivalWriteLock& pa_lock);
    void updateArrivalTimes(const PhotonArrivalType& arrivals, PhotonArrivalWriteLock& lock);
private:
    PhotonMutex m_detectedPhotonsMutex;
    LaserPowerMutex m_laserPowerMutex;
    PhotonArrivalMutex m_photonArrivalMutex;

    std::list<Photon> m_photons;
    std::unordered_map<uint64_t, PhotonBlock> m_binnedPhotons;
    std::list<double> m_laserPowers;
    PhotonArrivalType m_arrivalTimes;
};

struct Photon
{

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
    void countPhoton()
    {
        if constexpr (pt == PhotonType::DD) nDD++;
        else if constexpr (pt == PhotonType::DA) nDA++;
        else if constexpr (pt == PhotonType::AA) nAA++;
    }

    void combine(PhotonBlock& other)
    {
        nAA += other.nAA;
        nDD += other.nDD;
        nDA += other.nDA;
    }

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


#endif // PHOTON_H
