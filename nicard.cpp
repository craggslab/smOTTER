#include "nicard.h"

#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std::chrono_literals;

using float64_seconds = std::chrono::duration<float64>;

#define RET_IF_FAILED(func) if (auto err = checkNIDAQError(func); err.has_value()) return err

NICard::NICard(const std::string& deviceName)
    : m_deviceName(deviceName),
      m_triggerTask(nullptr),
      m_counterTask(nullptr)
{
}

std::vector<std::string> NICard::getAvailableDevices()
{
    int32 buffSize = DAQmxGetSystemInfoAttribute(DAQmx_Sys_DevNames, nullptr);
    std::vector<char> buff(static_cast<size_t>(buffSize));
    DAQmxGetSystemInfoAttribute(DAQmx_Sys_DevNames, buff.data(), buff.size());

    return splitNIStrings(buff);
}

std::vector<std::string> NICard::getDigitalOutLines() const
{
    return getNIStrings(&DAQmxGetDevDILines, std::make_optional(m_deviceName + "/"));
}

std::vector<std::string> NICard::getCounters() const
{
    return getNIStrings(&DAQmxGetDevCIPhysicalChans, std::make_optional(m_deviceName + "/"));
}

std::vector<std::string> NICard::getCounterLines() const
{
    auto terminals = getNIStrings(&DAQmxGetDevTerminals, std::make_optional("/" + m_deviceName + "/"));

    auto isNotPFI = [](const std::string& str) {
        return str.find("PFI") == std::string::npos;
    };

    terminals.erase(std::remove_if(terminals.begin(), terminals.end(),
                                   isNotPFI), terminals.end());

    return terminals;
}

std::vector<std::string> NICard::getTimebases() const
{
   auto terminals = getNIStrings(&DAQmxGetDevTerminals, std::make_optional("/" + m_deviceName + "/"));

   auto isNotTimebase = [](const std::string& str) {
       return str.find("HzTimebase") == std::string::npos;
   };

   terminals.erase(std::remove_if(terminals.begin(), terminals.end(),
                                  isNotTimebase), terminals.end());

   return terminals;
}

std::string NICard::getDeviceName() const
{
    return m_deviceName;
}

std::vector<std::string> NICard::getNIStrings(int32(*strFunc)(const char *, char *, uInt32), std::optional<std::string> removeFromBegining) const
{
    std::vector<std::string> out;

    if (m_deviceName != "")
    {
        int32 buffSize = strFunc(m_deviceName.c_str(), nullptr, 0);
        std::vector<char> buff(static_cast<size_t>(buffSize));
        strFunc(m_deviceName.c_str(), buff.data(), static_cast<uInt32>(buff.size()));

        out = splitNIStrings(buff, removeFromBegining);
    }

    return out;
}

std::vector<std::string> NICard::splitNIStrings(std::vector<char>& niStrings, std::optional<std::string> removeFromBegining)
{
    std::vector<std::string> out;

    char *token = nullptr, *next_token = nullptr;
    if (niStrings.size() != 0)
    {
        token = strtok_s(niStrings.data(), ", ", &next_token);
        while (token != nullptr)
        {
            std::string name(token);

            if (removeFromBegining.has_value())
            {
                size_t pos = name.find(removeFromBegining.value());
                out.push_back(name.substr(pos + removeFromBegining->length()));
            }
            else
            {
                out.push_back(name);
            }

            token = strtok_s(nullptr, ", ", &next_token);
        }
    }

    return out;
}

void NICard::setDonorLaserPin(std::string pin) { m_donorLaserPin = std::move(pin); }
void NICard::setAcceptorLaserPin(std::string pin) { m_acceptorLaserPin = std::move(pin); }
void NICard::setDonorDetectorCounter(std::string pin) { m_donorDetectorCounter = std::move(pin); }
void NICard::setAcceptorDetectorCounter(std::string pin) { m_acceptorDetectorCounter = std::move(pin); }
void NICard::setDonorDetectorPin(std::string pin) { m_donorDetectorPin = std::move(pin); }
void NICard::setAcceptorDetectorPin(std::string pin) { m_acceptorDetectorPin = std::move(pin); }
void NICard::setDonorDetectorGate(std::string pin) { m_donorDetectorGate = std::move(pin); }
void NICard::setAcceptorDetectorGate(std::string pin) { m_acceptorDetectorGate = std::move(pin); }
void NICard::setTimebase(std::string pin) { m_timebase = std::move(pin); }
void NICard::setLaserControlResolution(std::chrono::microseconds res) { m_laserControlResolution = std::move(res); }

std::optional<std::string> NICard::prime()
{
    RET_IF_FAILED(DAQmxCreateTask("Triggers", &m_triggerTask));

    RET_IF_FAILED(DAQmxCreateDOChan(m_triggerTask, (m_deviceName + "/" + m_donorLaserPin).c_str(), "Donor Laser Trigger", DAQmx_Val_ChanForAllLines));
    RET_IF_FAILED(DAQmxCreateDOChan(m_triggerTask, (m_deviceName + "/" + m_acceptorLaserPin).c_str(), "Acceptor Laser Trigger", DAQmx_Val_ChanForAllLines));
    RET_IF_FAILED(DAQmxCreateDOChan(m_triggerTask, (m_deviceName + "/" + m_donorDetectorGate).c_str(), "Donor Detector Gate Signal", DAQmx_Val_ChanForAllLines));
    RET_IF_FAILED(DAQmxCreateDOChan(m_triggerTask, (m_deviceName + "/" + m_acceptorDetectorGate).c_str(), "Acceptor Detector Gate Signal", DAQmx_Val_ChanForAllLines));

    auto DORate = std::chrono::duration_cast<float64_seconds>(1s) / m_laserControlResolution;
    RET_IF_FAILED(DAQmxCfgSampClkTiming(m_triggerTask, nullptr, DORate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 0));

    return std::nullopt;
}

std::optional<std::string> NICard::start()
{
    std::stringstream ss {};
    ss << m_deviceName << ",  "
       << m_donorLaserPin << ",  "
       << m_acceptorLaserPin << ",  "
       << m_donorDetectorCounter << ",  "
       << m_acceptorDetectorCounter << ",  "
       << m_donorDetectorPin << ",  "
       << m_acceptorDetectorPin << ",  "
       << m_donorDetectorGate << ",  "
       << m_acceptorDetectorGate << ",  "
       << m_timebase << ",  "
       << m_laserControlResolution.count() << "us";

    return std::make_optional(ss.str());
}

std::optional<std::string> NICard::stop()
{
    if (m_triggerTask)
        DAQmxClearTask(m_triggerTask);
    return std::nullopt;
}

std::optional<std::string> NICard::checkNIDAQError(int32 error)
{
    if (DAQmxFailed(error))
    {
        int32 buffSize = DAQmxGetExtendedErrorInfo(nullptr, 0);
        std::vector<char> buff(static_cast<size_t>(buffSize));
        DAQmxGetExtendedErrorInfo(buff.data(), static_cast<uInt32>(buff.size()));

        return std::make_optional(buff.data());
    }

    return std::nullopt;
}
