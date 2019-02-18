#ifndef PHOTON_H
#define PHOTON_H

#include <cstdint>
#include <list>
#include <chrono>

enum class PhotonType {
    DD,
    AA,
    DA
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
    void addPhoton(std::chrono::nanoseconds time)
    {
        photons.emplace_back(time, pt);
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

    std::list<Photon> photons;
    uint64_t nAA;
    uint64_t nDD;
    uint64_t nDA;
};

#endif // PHOTON_H
