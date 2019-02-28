#include "photon.h"

#include <iostream>


bool DutyCycle::isLaserOn(std::chrono::nanoseconds t) const
{
    if (t < offEndTime) return false;
    if (t >= offEndTime && t < onEndTime) return true;
    else return false;
}

void DutyCycle::setOnPercent(uint8_t percent)
{
    on_percent = percent;
    updateTimings();
}

void DutyCycle::setOffPercent(uint8_t percent)
{
    off_percent = percent;
    updateTimings();
}

void DutyCycle::setPeriod(std::chrono::microseconds period)
{
    this->period = std::move(period);
    updateTimings();
}

void DutyCycle::updateTimings()
{
    onePercent = period / 100;
    offEndTime = onePercent * off_percent;
    onEndTime = offEndTime + onePercent * on_percent;
}

uint8_t DutyCycle::getOffPercent() const {
    return off_percent;
}

uint8_t DutyCycle::getOnPercent() const {
    return on_percent;
}

std::chrono::nanoseconds DutyCycle::getOffEndTime() const
{
    return offEndTime;
}

std::chrono::nanoseconds DutyCycle::getOnEndTime() const
{
    return onEndTime;
}





PhotonStore::PhotonLock PhotonStore::getReadLockObject() {
    return PhotonLock(m_detectedPhotonsMutex, std::defer_lock);
}

PhotonStore::PhotonWriteLock PhotonStore::getWriteLockObject() {
    return PhotonWriteLock(m_detectedPhotonsMutex, std::defer_lock);
}

void PhotonStore::clear(const PhotonWriteLock& lock)
{
    if (!lock.owns_lock())
        throw std::invalid_argument("Lock object does not own lock (PhotonStore::clear)!");

    m_binnedPhotons.clear();
    m_photons.clear();
}

void PhotonStore::spliceNewPhotons(std::list<Photon>& newPhotons, const PhotonWriteLock& lock)
{
    if (!lock.owns_lock())
        throw std::invalid_argument("Cannot perform splice! Lock object does not own lock (PhotonStore::spliceNewPhotons)!");

    m_photons.splice(m_photons.end(), newPhotons);
}
void PhotonStore::combinePhotonBlock(uint64_t bin, PhotonBlock& block, const PhotonWriteLock& lock)
{
    if (!lock.owns_lock())
        throw std::invalid_argument("Cannont perform combination! Lock object does not own lock (PhotonStore::combinePhotonBlock)!");

    m_binnedPhotons[bin].combine(block);
}

PhotonStore::IteratorStruct<PhotonStore::ConstPhotonIterator> PhotonStore::photons(const PhotonLock& lock) const
{
    if (!lock.owns_lock())
        throw std::invalid_argument("Lock object does not own lock (PhotonStore::photons)!");

    return IteratorStruct(m_photons.cbegin(), m_photons.cend());
}
PhotonStore::IteratorStruct<PhotonStore::ConstPhotonBlockIterator> PhotonStore::binnedPhotons(const PhotonLock& lock) const
{
    if (!lock.owns_lock())
        throw std::invalid_argument("Lock object does not own lock (PhotonStore::binnedPhotons)!");

    return IteratorStruct(m_binnedPhotons.cbegin(), m_binnedPhotons.cend());
}

PhotonStore::ConstPhotonBlockIterator PhotonStore::findBin(uint64_t bin, const PhotonLock& lock) const
{
    if (!lock.owns_lock())
        throw std::invalid_argument("Lock object does not own lock (PhotonStore::findBin)!");

    return m_binnedPhotons.find(bin);
}
