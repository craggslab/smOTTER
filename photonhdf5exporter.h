#ifndef PHOTONHDF5EXPORTER_H
#define PHOTONHDF5EXPORTER_H

#include <string>
#include <optional>
#include <chrono>
#include <iostream>

#include "photon.h"

class PhotonHDF5Exporter
{
public:
    using SaveResult = std::pair<PhotonStore::ConstPhotonIterator, std::optional<std::string>>;


    PhotonHDF5Exporter();

    std::optional<std::string> createFile();
    SaveResult savePhotons(std::optional<PhotonStore::ConstPhotonIterator> ph_start_op,
                           PhotonStore& store);

    void setFilename(const std::string& filename);
    void setAcquisitionDuration(const std::chrono::seconds& acquisitionDuration);
    void setDescription(const std::string& description);
    void setAlexPeriod(const std::chrono::microseconds& alexPeriod);
    void setDonorExWavelength(float donorExWavelength);
    void setAcceptorExWavelength(float acceptorExWavelength);
    void setDonorDetWavelength(float donorDetWavelength);
    void setAcceptorDetWavelength(float acceptorDetWavelength);
    void setDonorInputPower(float donorInputPower);
    void setAcceptorInputPower(float acceptorInputPower);
    void setDonorDye(const std::string& donorDye);
    void setAcceptorDye(const std::string& acceptorDye);
    void setBufferDesc(const std::string& bufferDesc);
    void setSampleName(const std::string& sampleName);
    void setUserName(const std::string& userName);
    void setUserAffiliation(const std::string& userAffiliation);
    void setLaserPowerSampleInterval(const std::chrono::nanoseconds& interval);

    std::string getFilename() const;

    template<FluorophoreType ft>
    void setLaserOnPercentage(uint8_t percent) {
        if constexpr (ft == FluorophoreType::Donor)
            m_donorDutyCycle.setOnPercent(percent);
        else
            m_acceptorDutyCycle.setOnPercent(percent);
    }

    template<FluorophoreType ft>
    void setLaserOffPercentage(uint8_t percent)
    {
        if constexpr (ft == FluorophoreType::Donor)
            m_donorDutyCycle.setOffPercent(percent);
        else
            m_acceptorDutyCycle.setOffPercent(percent);
    }

private:
    std::optional<std::string> checkAllValuesPresent();

    std::string m_filename;
    std::chrono::seconds m_acquisitionDuration;
    std::string m_description;
    std::chrono::microseconds m_alexPeriod;
    DutyCycle m_donorDutyCycle;
    DutyCycle m_acceptorDutyCycle;
    float m_donorExWavelength;
    float m_acceptorExWavelength;
    float m_donorDetWavelength;
    float m_acceptorDetWavelength;
    float m_donorInputPower;
    float m_acceptorInputPower;
    std::string m_donorDye;
    std::string m_acceptorDye;
    std::string m_bufferDesc;
    std::string m_sampleName;
    std::string m_userName;
    std::string m_userAffiliation;
    std::chrono::nanoseconds m_laserPowerSampleInterval;
};

#endif // PHOTONHDF5EXPORTER_H
