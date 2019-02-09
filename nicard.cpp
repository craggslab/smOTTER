#include "nicard.h"

#include <iostream>
#include <algorithm>

NICard::NICard(const std::string& deviceName)
    : m_deviceName(deviceName)
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
