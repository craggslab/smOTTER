#define _CRT_SECURE_NO_WARNINGS
#include "photonhdf5exporter.h"
#include "photonhdf5helper.h"


#include <H5Cpp.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std::chrono_literals;

PhotonHDF5Exporter::PhotonHDF5Exporter() {}

std::optional<std::string> PhotonHDF5Exporter::savePhotons(const Photon::constPhotonIterator& start, const Photon::constPhotonIterator& end, PhotonLock lock)
{
    lock.lock();
    std::vector<Photon> photonVec(start, end);
    lock.unlock();

    std::vector<uint64_t> timestamps(photonVec.size());
    std::vector<uint8_t> detectors(photonVec.size());

    std::transform(photonVec.begin(), photonVec.end(), timestamps.begin(),
                   [](Photon p) { return p.timeStamp/10ns; });
    std::transform(photonVec.begin(), photonVec.end(), detectors.begin(),
                   [](Photon p) { return p.type == PhotonType::DD ? 0 : 1; });

    try {
        auto file = H5::H5File(m_filename, H5F_ACC_RDWR);
        auto photonDataGroup = file.openGroup("/photon_data");

        expandDataset(timestamps, PhotonHDF5::timestamps_name, H5::PredType::NATIVE_UINT64, photonDataGroup);
        expandDataset(detectors, PhotonHDF5::detectors_name, H5::PredType::NATIVE_UINT8, photonDataGroup);
    }
    catch (H5::FileIException& err) {  return std::make_optional(err.getDetailMsg()); }
    catch (H5::DataSetIException& err) { return std::make_optional(err.getDetailMsg()); }
    catch (H5::DataSpaceIException& err) { return std::make_optional(err.getDetailMsg()); }
    catch (H5::GroupIException& err) { return std::make_optional(err.getDetailMsg()); }
    catch (PhotonHDF5::bad_photon_hdf5_error& err) { return std::make_optional(err.what()); }

    return std::nullopt;
}


void PhotonHDF5Exporter::setFilename(const std::string& filename) { m_filename = filename; }
void PhotonHDF5Exporter::setAcquisitionDuration(const std::chrono::seconds& acquisitionDuration) { m_acquisitionDuration = acquisitionDuration; }
void PhotonHDF5Exporter::setDescription(const std::string& description) { m_description = description; }
void PhotonHDF5Exporter::setAlexPeriod(const std::chrono::microseconds& alexPeriod) { m_alexPeriod = alexPeriod; }
void PhotonHDF5Exporter::setDonorExWavelength(float donorExWavelength) { m_donorExWavelength = donorExWavelength; }
void PhotonHDF5Exporter::setAcceptorExWavelength(float acceptorExWavelength) { m_acceptorExWavelength = acceptorExWavelength; }
void PhotonHDF5Exporter::setDonorDetWavelength(float donorDetWavelength) { m_donorDetWavelength = donorDetWavelength; }
void PhotonHDF5Exporter::setAcceptorDetWavelength(float acceptorDetWavelength) { m_acceptorDetWavelength = acceptorDetWavelength; }
void PhotonHDF5Exporter::setDonorInputPower(float donorInputPower) { m_donorInputPower = donorInputPower; }
void PhotonHDF5Exporter::setAcceptorInputPower(float acceptorInputPower) { m_acceptorInputPower = acceptorInputPower; }
void PhotonHDF5Exporter::setDonorDye(const std::string& donorDye) { m_donorDye = donorDye; }
void PhotonHDF5Exporter::setAcceptorDye(const std::string& acceptorDye) { m_acceptorDye = acceptorDye; }
void PhotonHDF5Exporter::setBufferDesc(const std::string& bufferDesc) { m_bufferDesc = bufferDesc; }
void PhotonHDF5Exporter::setSampleName(const std::string& sampleName) { m_sampleName = sampleName; }
void PhotonHDF5Exporter::setUserName(const std::string& userName) { m_userName = userName; }
void PhotonHDF5Exporter::setUserAffiliation(const std::string& userAffiliation) { m_userAffiliation = userAffiliation; }

std::optional<std::string> PhotonHDF5Exporter::checkAllValuesPresent()
{
    auto ss = std::stringstream();

    if (m_filename == "")
        ss << "No Filename Provided!\n";
    if (m_description == "")
        ss << "No Experiment Description Provided!\n";
    if (m_donorExWavelength < 1e-9f || m_donorExWavelength > 1000e-9f)
        ss << "No Donor Excitation Wavelength Provided!\n";
    if (m_acceptorExWavelength < 1e-9f || m_acceptorExWavelength > 1000e-9f)
        ss << "No Acceptor Excitation Wavelength Provided\n";
    if (m_donorDetWavelength < 1e-9f || m_donorDetWavelength > 1000e-9f)
        ss << "No Donor Detection Wavelength Provided\n";
    if (m_acceptorDetWavelength < 1e-9f || m_acceptorDetWavelength > 1000e-9f)
        ss << "No Acceptor Detection Wavelength Provided\n";
    if (m_donorInputPower < 10e-9f || m_donorInputPower > 1e3f)
        ss << "No Donor Input Power Provided!\n";
    if (m_acceptorInputPower < 10e-9f || m_acceptorInputPower > 1e3f)
        ss << "No Acceptor Input Power Provided!\n";
    if (m_donorDye == "")
        ss << "No Donor Dye Name Provieded!\n";
    if (m_acceptorDye == "")
        ss << "No Acceptor Dye Name Provided!\n";
    if (m_bufferDesc == "")
        ss << "No Buffer Description Provided!\n";
    if (m_sampleName == "")
        ss << "No Sample Name Provided!\n";
    if (m_userName == "")
        ss << "No User Name Provided!\n";
    if (m_userAffiliation == "")
        ss << "No User Affiliation Provided!\n";

    auto str = ss.str();
    return str == "" ? std::nullopt : std::make_optional("File Creation Failed\n\n" + str);
}

std::optional<std::string> PhotonHDF5Exporter::createFile()
{
    if (auto ret = checkAllValuesPresent(); ret.has_value())
        return ret;

    try {
        H5::Exception::dontPrint();

        auto file = H5::H5File(m_filename, H5F_ACC_TRUNC);

        // ----- Root Group ------- //
        writeStringAttribute("Photon-HDF5", 			PhotonHDF5::format_name_name, 		file);
        writeStringAttribute("0.4", 					PhotonHDF5::format_version_name, 	file);
        writeDataset<float>	({ 60.0f }, 				PhotonHDF5::acq_duration_name, 		PhotonHDF5::acq_duration_desc, 	H5::PredType::NATIVE_FLOAT, H5::PredType::IEEE_F32LE, file);
        writeStringDataset	(m_description,				PhotonHDF5::description_name, 		PhotonHDF5::description_desc,	file);

        // ----- Photon Data Group ------- //
        auto photonDataGroup = createGroup(PhotonHDF5::photon_data_name, PhotonHDF5::photon_data_desc, file);
        createExpandableDataset(PhotonHDF5::timestamps_name, PhotonHDF5::timestamps_desc, H5::PredType::STD_U64LE, 9, photonDataGroup);
        createExpandableDataset(PhotonHDF5::detectors_name, PhotonHDF5::detectors_desc, H5::PredType::STD_U8LE, 9, photonDataGroup);

        // ----- Timestamp Specs Group ------- //
        auto timestampsSpecsGroup = createGroup(PhotonHDF5::timestamp_specs_name, PhotonHDF5::timestamp_specs_desc, file);
        writeDataset<float>({ 10e-9f }, PhotonHDF5::timestamps_unit_name, PhotonHDF5::timestamps_unit_desc, H5::PredType::NATIVE_FLOAT, H5::PredType::IEEE_F32LE, timestampsSpecsGroup);

        // ----- Measurement Specs Group ------- //
        auto measurementSpecsGroup = createGroup(PhotonHDF5::measurement_specs_name, PhotonHDF5::measurement_specs_desc, file);
        writeStringDataset			( "smFRET-usALEX", 																			PhotonHDF5::measurement_type_name, 			PhotonHDF5::measurement_specs_desc, 		measurementSpecsGroup);
        writeDataset<int64_t>		( { m_alexPeriod/10ns }, 																	PhotonHDF5::alex_period_name, 				PhotonHDF5::alex_period_desc, 				H5::PredType::NATIVE_INT64, 		H5::PredType::STD_I64LE, 			measurementSpecsGroup);
        writeDataset<int64_t>		( { m_donorDutyCycle.getOffEndTime()/10ns, m_donorDutyCycle.getOnEndTime()/10ns }, 			PhotonHDF5::alex_excitation_period1_name, 	PhotonHDF5::alex_excitation_period1_desc, 	H5::PredType::NATIVE_INT64, 		H5::PredType::STD_I64LE, 			measurementSpecsGroup);
        writeDataset<int64_t>		( { m_acceptorDutyCycle.getOffEndTime()/10ns, m_acceptorDutyCycle.getOnEndTime()/10ns }, 	PhotonHDF5::alex_excitation_period2_name, 	PhotonHDF5::alex_excitation_period2_desc, 	H5::PredType::NATIVE_INT64, 		H5::PredType::STD_I64LE, 			measurementSpecsGroup);
        writeDataset<uint64_t>		( { 0ull }, 																				PhotonHDF5::alex_offset_name, 				PhotonHDF5::alex_offset_desc, 				H5::PredType::NATIVE_UINT64, 		H5::PredType::STD_U64LE, 			measurementSpecsGroup);

        // ----- Detector Specs Group ------- //
        auto detectorSpecsGroup = createGroup(PhotonHDF5::detector_specs_name, 	PhotonHDF5::detector_specs_desc, 	file);
        writeDataset<uint8_t>({ 0 }, PhotonHDF5::spectral_ch1_name, 			PhotonHDF5::spectral_ch1_desc, 		H5::PredType::NATIVE_UINT8, H5::PredType::STD_U8LE, detectorSpecsGroup);
        writeDataset<uint8_t>({ 1 }, PhotonHDF5::spectral_ch2_name, 			PhotonHDF5::spectral_ch2_desc, 		H5::PredType::NATIVE_UINT8, H5::PredType::STD_U8LE, detectorSpecsGroup);

        // ----- Setup Group ------- //
        auto setupGroup = createGroup(PhotonHDF5::setup_name, PhotonHDF5::setup_desc, file);
        writeDataset<uint8_t>	( { 2 }, 												PhotonHDF5::num_pixels_name, 				PhotonHDF5::num_pixels_desc, 				H5::PredType::NATIVE_UINT8, H5::PredType::STD_U8LE, 	setupGroup);
        writeDataset<uint8_t>	( { 1 }, 												PhotonHDF5::num_spots_name, 				PhotonHDF5::num_spots_desc, 				H5::PredType::NATIVE_UINT8, H5::PredType::STD_U8LE, 	setupGroup);
        writeDataset<uint8_t>	( { 2 }, 												PhotonHDF5::num_spectral_ch_name, 			PhotonHDF5::num_spectral_ch_desc, 			H5::PredType::NATIVE_UINT8, H5::PredType::STD_U8LE, 	setupGroup);
        writeDataset<uint8_t>	( { 1 }, 												PhotonHDF5::num_polerization_ch_name, 		PhotonHDF5::num_spectral_ch_desc, 			H5::PredType::NATIVE_UINT8, H5::PredType::STD_U8LE, 	setupGroup);
        writeDataset<uint8_t>	( { 1 }, 												PhotonHDF5::num_split_ch_name, 				PhotonHDF5::num_split_ch_desc, 				H5::PredType::NATIVE_UINT8, H5::PredType::STD_U8LE, 	setupGroup);
        writeDataset<uint8_t>	( { 1 }, 												PhotonHDF5::modulated_excitation_name, 		PhotonHDF5::modulated_excitation_desc, 		H5::PredType::NATIVE_UINT8, H5::PredType::STD_U8LE, 	setupGroup);
        writeDataset<uint8_t>	( { 0 }, 												PhotonHDF5::lifetime_name, 					PhotonHDF5::lifetime_desc, 					H5::PredType::NATIVE_UINT8, H5::PredType::STD_U8LE, 	setupGroup);
        writeDataset<float>		( { m_donorExWavelength, m_acceptorExWavelength }, 		PhotonHDF5::excitation_wavelengths_name, 	PhotonHDF5::excitation_wavelengths_desc,	H5::PredType::NATIVE_FLOAT, H5::PredType::IEEE_F32LE, 	setupGroup);
        writeDataset<uint8_t>	( { 1, 1 }, 											PhotonHDF5::excitation_cw_name, 			PhotonHDF5::excitation_cw_desc, 			H5::PredType::NATIVE_UINT8, H5::PredType::STD_U8LE,		setupGroup);
        writeDataset<float>		( { m_donorInputPower, m_acceptorInputPower },			PhotonHDF5::excitation_input_powers_name,	PhotonHDF5::excitation_input_powers_desc, 	H5::PredType::NATIVE_FLOAT, H5::PredType::IEEE_F32LE, 	setupGroup);
        writeDataset<float>		( { m_donorDetWavelength, m_acceptorDetWavelength },	PhotonHDF5::detection_wavelengths_name, 	PhotonHDF5::detection_wavelengths_desc, 	H5::PredType::NATIVE_FLOAT, H5::PredType::IEEE_F32LE,	setupGroup);

        // ----- Sample Group ------- //
        auto sampleGroup = createGroup(PhotonHDF5::sample_name, PhotonHDF5::sample_desc, file);
        writeDataset<uint8_t> 		( { 2 }, 								PhotonHDF5::num_dyes_name, 		PhotonHDF5::num_dyes_desc, 		H5::PredType::NATIVE_UINT8, H5::PredType::STD_U8LE, sampleGroup);
        writeStringDataset			( m_donorDye + ", " + m_acceptorDye, 	PhotonHDF5::dye_names_name, 	PhotonHDF5::dye_names_desc, 	sampleGroup);
        writeStringDataset			( m_bufferDesc,							PhotonHDF5::buffer_name_name, 	PhotonHDF5::buffer_name_desc, 	sampleGroup);
        writeStringDataset			( m_sampleName, 						PhotonHDF5::sample_name_name, 	PhotonHDF5::sample_name_desc, 	sampleGroup);

        // ----- Identity Group ------- //
        auto time = std::time(nullptr);
        auto localTime = std::localtime(&time);
        auto ss = std::stringstream(); ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

        auto identityGroup = createGroup(PhotonHDF5::identity_name, PhotonHDF5::identity_desc, file);
        writeStringDataset(ss.str(), 					PhotonHDF5::creation_time_name, 		PhotonHDF5::creation_time_desc, 		identityGroup);
        writeStringDataset("smFRETBox Acquisition", 	PhotonHDF5::software_name, 				PhotonHDF5::software_desc, 				identityGroup);
        writeStringDataset("1.0",						PhotonHDF5::software_version_name, 		PhotonHDF5::software_version_desc, 		identityGroup);
        writeStringDataset("Photon-HDF5",				PhotonHDF5::format_name_name,			PhotonHDF5::format_name_desc,			identityGroup);
        writeStringDataset("0.4",						PhotonHDF5::format_version_name,		PhotonHDF5::format_version_desc,		identityGroup);
        writeStringDataset("http://photon-hdf5.org/", 	PhotonHDF5::format_url_name, 			PhotonHDF5::format_url_desc,			identityGroup);
        writeStringDataset(m_userName,					PhotonHDF5::author_name,				PhotonHDF5::author_desc,				identityGroup);
        writeStringDataset(m_userAffiliation,			PhotonHDF5::author_affiliation_name,	PhotonHDF5::author_affiliation_desc, 	identityGroup);
        writeStringDataset(m_userName,					PhotonHDF5::creator_name,				PhotonHDF5::creator_desc,				identityGroup);
        writeStringDataset(m_userAffiliation,			PhotonHDF5::creator_affiliation_name,	PhotonHDF5::creator_affiliation_desc,	identityGroup);

    }
    catch (H5::FileIException& err) {  return std::make_optional(err.getDetailMsg()); }
    catch (H5::DataSetIException& err) { return std::make_optional(err.getDetailMsg()); }
    catch (H5::DataSpaceIException& err) { return std::make_optional(err.getDetailMsg()); }
    catch (H5::GroupIException& err) { return std::make_optional(err.getDetailMsg()); }
    return std::nullopt;
}

