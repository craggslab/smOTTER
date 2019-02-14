#ifndef PHOTON_H
#define PHOTON_H

#include <cstdint>

enum class PhotonType {
    DD,
    AA,
    DA
};

struct Photon
{
    uint64_t timeStamp;
    PhotonType type;
};

#endif // PHOTON_H
