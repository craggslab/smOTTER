#ifndef NICARD_H
#define NICARD_H

#include <vector>
#include <string>
#include <optional>

#include <NIDAQmx.h>

class NICard
{
public:
    NICard(const std::string& deviceName = "");

    std::vector<std::string> getDigitalOutLines() const;
    std::vector<std::string> getCounters() const;
    std::vector<std::string> getCounterLines() const;
    std::vector<std::string> getTimebases() const;
    std::string getDeviceName() const;

    static std::vector<std::string> getAvailableDevices();

private:
    std::vector<std::string> getNIStrings(int32(*strFunc)(const char *, char *, uInt32), std::optional<std::string> removeFromBegining = std::nullopt) const;
    static std::vector<std::string> splitNIStrings(std::vector<char>& niStrings, std::optional<std::string> removeFromBegining = std::nullopt);

    std::string m_deviceName;
};

#endif // NICARD_H
