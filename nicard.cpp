#include "nicard.h"

#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std::chrono_literals;

#define RET_IF_FAILED(func) if (auto err = checkNIDAQError(func); err.has_value()) return err

int32 CVICALLBACK AcquisitionFinishedCallback(TaskHandle, int32, void *callbackData)
{
    auto card = static_cast<NICard*>(callbackData);
    card->stop();

    return 0;
}

NICard::NICard(const std::string& deviceName)
    : m_deviceName(deviceName),
      m_triggerTask(nullptr),
      m_donorCounterTask(nullptr),
      m_acceptorCounterTask(nullptr),
      m_running(false),
      m_stopReadPhotons(false),
      m_totalAcceptorPhotons(0),
      m_totalDonorPhotons(0)
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

uint64_t NICard::getTotalDonorPhotons() const { return m_totalDonorPhotons; }
uint64_t NICard::getTotalAcceptorPhotons() const { return m_totalAcceptorPhotons; }

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

void NICard::setDonorLaserPin(std::string pin) { m_donorLaserPin = std::move(pin); clearTriggers(); }
void NICard::setAcceptorLaserPin(std::string pin) { m_acceptorLaserPin = std::move(pin); clearTriggers(); }
void NICard::setDonorDetectorCounter(std::string pin) { m_donorDetectorCounter = std::move(pin); }
void NICard::setAcceptorDetectorCounter(std::string pin) { m_acceptorDetectorCounter = std::move(pin); }
void NICard::setDonorDetectorPin(std::string pin) { m_donorDetectorPin = std::move(pin); }
void NICard::setAcceptorDetectorPin(std::string pin) { m_acceptorDetectorPin = std::move(pin); }
void NICard::setDonorDetectorGate(std::string pin) { m_donorDetectorGate = std::move(pin); clearTriggers(); }
void NICard::setAcceptorDetectorGate(std::string pin) { m_acceptorDetectorGate = std::move(pin); clearTriggers(); }
void NICard::setTimebase(std::string pin) { m_timebase = std::move(pin); }
void NICard::setLaserControlResolution(std::chrono::nanoseconds res) { m_laserControlResolution = std::move(res); }
void NICard::setTimestampAdjustment(uint64_t val) { m_timestampAdjustment = val; }

void NICard::setAlexPeriod(std::chrono::microseconds micros)
{
    m_alexPeriod = micros;
    m_donorLaserDutyCycle.setPeriod(micros);
    m_acceptorLaserDutyCycle.setPeriod(micros);
}
void NICard::setDonorLaserOffPercentage(uint8_t percentage) { m_donorLaserDutyCycle.setOffPercent(percentage); }
void NICard::setDonorLaserOnPercentage(uint8_t percentage) { m_donorLaserDutyCycle.setOnPercent(percentage); }
void NICard::setAcceptorLaserOffPercentage(uint8_t percentage) { m_acceptorLaserDutyCycle.setOffPercent(percentage); }
void NICard::setAcceptorLaserOnPercentage(uint8_t percentage) { m_acceptorLaserDutyCycle.setOnPercent(percentage); }
void NICard::setExperimentLength(std::chrono::minutes length) { m_experimentLength = std::move(length); }

bool NICard::isRunning() { return m_running; }

std::optional<std::string> NICard::prime()
{
    if (auto err = setupTriggers(); err.has_value())
    {
        clearTasks();
        return err.value();
    }

    if (auto err = setupCounters(); err.has_value())
    {
        clearTasks();
        return err.value();
    }

    return std::nullopt;
}

std::optional<std::string> NICard::start()
{
    {
        auto lock = m_photonStore.getWriteLockObject();
        lock.lock();
        m_photonStore.clear(lock);
    }

    RET_IF_FAILED(DAQmxStartTask(m_donorCounterTask));
    RET_IF_FAILED(DAQmxStartTask(m_acceptorCounterTask));
    RET_IF_FAILED(DAQmxStartTask(m_triggerTask));

    m_totalDonorPhotons = 0;
    m_totalAcceptorPhotons = 0;
    m_stopReadPhotons = false;
    m_readPhotonsResult = std::async(std::launch::async, &NICard::readPhotons, this);

    m_running = true;
    m_acquisitionStart = m_acquisitionClock.now();

    return std::nullopt;
}

std::optional<std::string> NICard::stop()
{
    std::lock_guard guard(m_stopAcquisitionMutex);

    m_stopReadPhotons = true;

    std::optional<std::string> readPhotonsResult = std::nullopt;
    if (m_readPhotonsResult.valid())
        readPhotonsResult = m_readPhotonsResult.get();
    else
        readPhotonsResult = std::make_optional("Could not retrieve result of read photons thread!");

    clearTasks();

    m_running = false;

    auto clearTriggersRes = clearTriggers();

    if (clearTriggersRes.has_value() && readPhotonsResult.has_value())
        return std::make_optional(clearTriggersRes.value() + readPhotonsResult.value());
    else if (clearTriggersRes.has_value())
        return clearTriggersRes;
    else if (readPhotonsResult.has_value())
        return readPhotonsResult;
    else
        return std::nullopt;
}

std::chrono::milliseconds NICard::timeSinceAcqStart()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(m_acquisitionClock.now() - m_acquisitionStart);
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

std::optional<std::string> NICard::setupTriggers()
{
    if (m_laserControlResolution*100 > m_alexPeriod)
        return std::make_optional("Alex period (" + std::to_string(m_alexPeriod.count()) + "us) too small! Should be at least 100 * the Laser Control Resolution (" + std::to_string(m_laserControlResolution.count()) + "ns)");

    if (m_alexPeriod % (m_laserControlResolution*100) != 0us)
        return std::make_optional("Invalid alex period (" + std::to_string(m_alexPeriod.count()) + "us)! Alex period should be evenly divisible by 100*the laser control resolution (100 x " + std::to_string(m_laserControlResolution.count()) + "ns)!");

    if (m_experimentLength % m_alexPeriod != 0min)
        return std::make_optional("Invalid alex period (" + std::to_string(m_alexPeriod.count()) + "us)! Alex period should evenly divide the experement time (" + std::to_string(m_experimentLength.count()) + " minutes)");

    auto traceSize = static_cast<size_t>(m_alexPeriod / m_laserControlResolution);
    std::vector<uInt8> DOValues(4*traceSize);

    for (size_t i = 0; i < traceSize; i++)
    {
        auto t = i * m_laserControlResolution;
        DOValues[i				  ] = m_donorLaserDutyCycle.isLaserOn(t) ? 1 : 0;
        DOValues[i +     traceSize] = m_acceptorLaserDutyCycle.isLaserOn(t) ?  1 : 0;
        DOValues[i + 2 * traceSize] = 1;
        DOValues[i + 3 * traceSize] = 1;
    }

    auto DORate = std::chrono::duration_cast<std::chrono::nanoseconds>(1s) / m_laserControlResolution;
    auto nDOSamples = DORate * std::chrono::duration_cast<std::chrono::seconds>(m_experimentLength);

    RET_IF_FAILED(DAQmxCreateTask("Triggers", &m_triggerTask));

    RET_IF_FAILED(DAQmxCreateDOChan(m_triggerTask, (m_deviceName + "/" + m_donorLaserPin).c_str(), "Donor Laser Trigger", DAQmx_Val_ChanForAllLines));
    RET_IF_FAILED(DAQmxCreateDOChan(m_triggerTask, (m_deviceName + "/" + m_acceptorLaserPin).c_str(), "Acceptor Laser Trigger", DAQmx_Val_ChanForAllLines));
    RET_IF_FAILED(DAQmxCreateDOChan(m_triggerTask, (m_deviceName + "/" + m_donorDetectorGate).c_str(), "Donor Detector Gate Signal", DAQmx_Val_ChanForAllLines));
    RET_IF_FAILED(DAQmxCreateDOChan(m_triggerTask, (m_deviceName + "/" + m_acceptorDetectorGate).c_str(), "Acceptor Detector Gate Signal", DAQmx_Val_ChanForAllLines));

    RET_IF_FAILED(DAQmxCfgSampClkTiming(m_triggerTask, nullptr, static_cast<float64>(DORate), DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, static_cast<uInt64>(nDOSamples.count())));
    RET_IF_FAILED(DAQmxRegisterDoneEvent(m_triggerTask, 0, &AcquisitionFinishedCallback, this));

    RET_IF_FAILED(DAQmxWriteDigitalLines(m_triggerTask, static_cast<int32>(traceSize), false, DAQmx_Val_WaitInfinitely, DAQmx_Val_GroupByChannel, DOValues.data(), nullptr, nullptr));

    float64 delay;
    RET_IF_FAILED(DAQmxGetStartTrigDelay(m_triggerTask, &delay));
    m_digitalOutputDelay = static_cast<uInt32>(delay + m_timestampAdjustment);

    return std::nullopt;
}

std::optional<std::string> NICard::setupCounters()
{
    float64 counterRate;
    auto timeBaseString = ("/" + m_deviceName + "/" + m_timebase);
    if (auto pos = m_timebase.find("Hz"); pos != std::string::npos)
    {
        auto value = m_timebase.substr(0, pos-1);
        auto unit = m_timebase[pos-1];

        if (unit == 'M')
            counterRate = std::stod(value) * 1000000.0;
        else if (unit == 'k')
            counterRate = std::stod(value) * 1000.0;
        else if (unit >= '0' && unit <= '9')
            counterRate = std::stod(value);
        else
            return std::make_optional("Could not recognise timebase unit!");
    }
    else
    {
        return std::make_optional("Could not recognise timebase unit (could not find Hz)!");
    }

    RET_IF_FAILED(DAQmxCreateTask("Donor Photon Counter", &m_donorCounterTask));
    RET_IF_FAILED(DAQmxCreateTask("Acceptor Counter Task", &m_acceptorCounterTask));

    RET_IF_FAILED(DAQmxCreateCICountEdgesChan(m_donorCounterTask, (m_deviceName + "/" + m_donorDetectorCounter).c_str(), "Donor Photon Counter", DAQmx_Val_Rising, 0, DAQmx_Val_CountUp));
    RET_IF_FAILED(DAQmxCreateCICountEdgesChan(m_acceptorCounterTask, (m_deviceName + "/" + m_acceptorDetectorCounter).c_str(), "Acceptor Photon Counter", DAQmx_Val_Rising, 0, DAQmx_Val_CountUp));

    RET_IF_FAILED(DAQmxCfgSampClkTiming(m_donorCounterTask, ("/" + m_deviceName + "/" + m_donorDetectorPin).c_str(), counterRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 5000));
    RET_IF_FAILED(DAQmxCfgSampClkTiming(m_acceptorCounterTask, ("/" + m_deviceName + "/" + m_acceptorDetectorPin).c_str(), counterRate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 5000));

    RET_IF_FAILED(DAQmxSetCICountEdgesTerm(m_donorCounterTask, "Donor Photon Counter", ("/" + m_deviceName + "/" + m_timebase).c_str()));
    RET_IF_FAILED(DAQmxSetCICountEdgesTerm(m_acceptorCounterTask, "Acceptor Photon Counter", ("/" + m_deviceName + "/" + m_timebase).c_str()));

    RET_IF_FAILED(DAQmxSetArmStartTrigType(m_donorCounterTask, DAQmx_Val_DigEdge));
    RET_IF_FAILED(DAQmxSetArmStartTrigType(m_acceptorCounterTask, DAQmx_Val_DigEdge));

    RET_IF_FAILED(DAQmxSetDigEdgeArmStartTrigSrc(m_donorCounterTask, ("/" + m_deviceName + "/do/StartTrigger").c_str()));
    RET_IF_FAILED(DAQmxSetDigEdgeArmStartTrigSrc(m_acceptorCounterTask, ("/" + m_deviceName + "/do/StartTrigger").c_str()));

    RET_IF_FAILED(DAQmxSetDigEdgeArmStartTrigEdge(m_donorCounterTask, DAQmx_Val_Rising));
    RET_IF_FAILED(DAQmxSetDigEdgeArmStartTrigEdge(m_acceptorCounterTask, DAQmx_Val_Rising));

    RET_IF_FAILED(DAQmxSetSampClkOverrunBehavior(m_donorCounterTask, DAQmx_Val_IgnoreOverruns));
    RET_IF_FAILED(DAQmxSetSampClkOverrunBehavior(m_acceptorCounterTask, DAQmx_Val_IgnoreOverruns));

    return std::nullopt;
}

std::optional<std::string> NICard::clearTriggers()
{
    if (m_deviceName == "" || m_donorLaserPin == "" || m_acceptorLaserPin == "" ||
            m_donorDetectorGate == "" || m_acceptorDetectorGate == "") return std::nullopt;

    TaskHandle clearTriggers;
    uInt8 values[] = { 0, 0, 0, 0 };

    RET_IF_FAILED(DAQmxCreateTask("Trigger Channel Clear", &clearTriggers));

    RET_IF_FAILED(DAQmxCreateDOChan(clearTriggers, (m_deviceName + "/" + m_donorLaserPin).c_str(), "Donor Laser Trigger", DAQmx_Val_ChanForAllLines));
    RET_IF_FAILED(DAQmxCreateDOChan(clearTriggers, (m_deviceName + "/" + m_acceptorLaserPin).c_str(), "Acceptor Laser Trigger", DAQmx_Val_ChanForAllLines));
    RET_IF_FAILED(DAQmxCreateDOChan(clearTriggers, (m_deviceName + "/" + m_donorDetectorGate).c_str(), "Donor Detector Gate Signal", DAQmx_Val_ChanForAllLines));
    RET_IF_FAILED(DAQmxCreateDOChan(clearTriggers, (m_deviceName + "/" + m_acceptorDetectorGate).c_str(), "Acceptor Detector Gate Signal", DAQmx_Val_ChanForAllLines));

    RET_IF_FAILED(DAQmxWriteDigitalLines(clearTriggers, 1, true, DAQmx_Val_WaitInfinitely, DAQmx_Val_GroupByChannel, values, nullptr, nullptr));

    RET_IF_FAILED(DAQmxWaitUntilTaskDone(clearTriggers, DAQmx_Val_WaitInfinitely));

    RET_IF_FAILED(DAQmxStopTask(clearTriggers));
    RET_IF_FAILED(DAQmxClearTask(clearTriggers));

    return std::nullopt;
}

void NICard::clearTasks()
{

    if (m_triggerTask != nullptr)
    {
        DAQmxStopTask(m_triggerTask);
        DAQmxClearTask(m_triggerTask);
        m_triggerTask = nullptr;
    }

    if (m_donorCounterTask != nullptr)
    {
        DAQmxStopTask(m_donorCounterTask);
        DAQmxClearTask(m_donorCounterTask);
        m_donorCounterTask = nullptr;
    }

    if (m_acceptorCounterTask != nullptr)
    {
        DAQmxStopTask(m_acceptorCounterTask);
        DAQmxClearTask(m_acceptorCounterTask);
        m_acceptorCounterTask = nullptr;
    }
}

std::optional<std::string> NICard::readPhotonsIntoBuffer(TaskHandle counterTask, std::vector<uInt32> &buff) const
{
    uInt32 nPhotons = 0;
    RET_IF_FAILED(DAQmxGetReadAvailSampPerChan(counterTask, &nPhotons));

    buff.resize(nPhotons);
    if (nPhotons == 0)
        return std::nullopt;

    RET_IF_FAILED(DAQmxReadCounterU32(counterTask, static_cast<int32>(nPhotons), DAQmx_Val_WaitInfinitely, buff.data(), static_cast<uInt32>(buff.size()), nullptr, nullptr));

    return std::nullopt;
}

std::optional<std::string> NICard::analysePhotons(const std::vector<uInt32> &buffer, uInt32 &previousPhotonArrival, uInt64 &offset, FluorophoreType detector,
                                                  std::list<Photon>& newPhotons, std::unordered_map<uint64_t, PhotonBlock>& newPhotonsBinned)
{
    using namespace std::chrono;
    using unsigned_ms = duration<uint64_t, std::ratio<1,1000>>;

    for (auto photonArrivalTime: buffer)
    {
        if (previousPhotonArrival > photonArrivalTime)
            offset += 0x1'0000'0000;

        previousPhotonArrival = photonArrivalTime;

        auto time = (offset + photonArrivalTime - m_digitalOutputDelay)*10ns;
        auto timeRelativeToAlex = time % m_alexPeriod;

        if (detector == FluorophoreType::Donor)
        {
            if (m_donorLaserDutyCycle.isLaserOn(timeRelativeToAlex))
            {
                newPhotons.emplace_back(time, PhotonType::DD);
                newPhotonsBinned[duration_cast<unsigned_ms>(time).count()].countPhoton<PhotonType::DD>();
            }
            else
            {
                continue;
            }

            m_totalDonorPhotons++;
        }
        else if (detector == FluorophoreType::Acceptor)
        {
            if (m_donorLaserDutyCycle.isLaserOn(timeRelativeToAlex))
            {
                newPhotons.emplace_back(time, PhotonType::DA);
                newPhotonsBinned[duration_cast<unsigned_ms>(time).count()].countPhoton<PhotonType::DA>();
            }
            else if (m_acceptorLaserDutyCycle.isLaserOn(timeRelativeToAlex))
            {
                newPhotons.emplace_back(time, PhotonType::AA);
                newPhotonsBinned[duration_cast<unsigned_ms>(time).count()].countPhoton<PhotonType::AA>();
            }
            else
            {
                continue;
            }

            m_totalAcceptorPhotons++;
        }
    }

    return std::nullopt;
}

std::optional<std::string> NICard::readPhotons()
{
    uInt32 previousDonorArrivalTime = 0, previousAcceptorArrivalTime = 0;
    uInt64 donorOffsetValue = 0, acceptorOffsetValue = 0;

    std::vector<uInt32> donorBuffer, acceptorBuffer;

    std::list<Photon> newPhotons;
    std::unordered_map<uint64_t, PhotonBlock> newPhotonsBinned;

    while (!m_stopReadPhotons)
    {
        if (auto err = readPhotonsIntoBuffer(m_donorCounterTask, donorBuffer); err.has_value())
            return err;

        if (auto err = readPhotonsIntoBuffer(m_acceptorCounterTask, acceptorBuffer); err.has_value())
            return err;

        if (donorBuffer.size())
            analysePhotons(donorBuffer, previousDonorArrivalTime, donorOffsetValue, FluorophoreType::Donor, newPhotons, newPhotonsBinned);

        if (acceptorBuffer.size())
            analysePhotons(acceptorBuffer, previousAcceptorArrivalTime, acceptorOffsetValue, FluorophoreType::Acceptor, newPhotons,  newPhotonsBinned);

        if (newPhotons.size() > 0)
        {
            auto lock = m_photonStore.getWriteLockObject();

            if (lock.try_lock_for(1us))
            {
                m_photonStore.spliceNewPhotons(newPhotons, lock);
                for (auto& [t, photons] : newPhotonsBinned)
                {
                    m_photonStore.combinePhotonBlock(t, photons, lock);
                }

                newPhotonsBinned.clear();
            }
        }
    }

    return std::nullopt;
}

PhotonStore& NICard::getPhotonStore()
{
    return m_photonStore;
}
