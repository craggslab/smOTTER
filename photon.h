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

#endif // PHOTON_H
