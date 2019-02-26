#include "photon.h"



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
