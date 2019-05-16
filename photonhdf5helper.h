#ifndef PHOTONHDF5HELPER_H
#define PHOTONHDF5HELPER_H

#include <vector>
#include <string>
#include <H5Cpp.h>
#include <iostream>

namespace PhotonHDF5 {
    const std::string acq_duration_name				= "acquisition_duration";
    const std::string acq_duration_desc 			= "Measurement duration in seconds.";

    const std::string description_name				= "description";
    const std::string description_desc				= "A user-defined comment describing the data file.";

    const std::string photon_data_name				= "/photon_data";
    const std::string photon_data_desc				= "Group containing arrays of photon-data.";

    const std::string timestamp_specs_name			= "/photon_data/timestamps_specs";
    const std::string timestamp_specs_desc			= "Specifications for timestamps.";

    const std::string timestamps_unit_name			= "timestamps_unit";
    const std::string timestamps_unit_desc			= "Value of 1-unit timestamp-increment in seconds.";

    const std::string measurement_specs_name		= "/photon_data/measurement_specs";
    const std::string measurement_specs_desc		= "Metadata necessary for interpretation of the particular type of measurement.";

    const std::string measurement_type_name			= "measurement_type";
    const std::string measurement_type_desc			= "Name of the measurement the data represents.";

    const std::string alex_period_name				= "alex_period";
    const std::string alex_period_desc				= "Period of laser alternation in us-ALEX measurements in timestamps units (defined in timestamps_specs/).";

    const std::string alex_excitation_period1_name	= "alex_excitation_period1";
    const std::string alex_excitation_period1_desc	= "Values pair (start-stop range, in timestamps units) identifying photons in the excitation period of wavelength 1 (the shortest).";

    const std::string alex_excitation_period2_name	= "alex_excitation_period2";
    const std::string alex_excitation_period2_desc	= "Values pair (start-stop range, in timestamps units) identifying photons in the excitation period of wavelength 2.";

    const std::string alex_offset_name				= "alex_offset";
    const std::string alex_offset_desc				= "Time offset (in timestamps unit) to apply to timestamps to obtain a properly aligned alternation histogram.";

    const std::string detectors_specs_name			= "/photon_data/measurement_specs/detectors_specs";
    const std::string detectors_specs_desc			= "Mapping between the pixel IDs and the detection channels.";

    const std::string spectral_ch1_name				= "spectral_ch1";
    const std::string spectral_ch1_desc				= "Pixel IDs for the first spectral channel (i.e. donor in a 2-color smFRET measurement).";

    const std::string spectral_ch2_name				= "spectral_ch2";
    const std::string spectral_ch2_desc				= "Pixel IDs for the first spectral channel (i.e. acceptor in a 2-color smFRET measurement).";

    const std::string setup_name					= "/setup";
    const std::string setup_desc					= "Information about the experimental setup.";

    const std::string num_pixels_name				= "num_pixels";
    const std::string num_pixels_desc				= "Total number of detector pixels.";

    const std::string num_spots_name				= "num_spots";
    const std::string num_spots_desc				= "Number of excitation (or detection) \"spots\" in the sample.";

    const std::string num_spectral_ch_name			= "num_spectral_ch";
    const std::string num_spectral_ch_desc			= "Number of distinct spectral bands which are acquired.";

    const std::string num_polarization_ch_name		= "num_polarization_ch";
    const std::string num_polarization_ch_desc		= "Number of distinct polarization states which are acquired.";

    const std::string num_split_ch_name				= "num_split_ch";
    const std::string num_split_ch_desc				= "Number of distinct detection channels detecting the same spectral band and polarization. This value is > 1 when using a non-polarizing beam splitter.";

    const std::string modulated_excitation_name		= "modulated_excitation";
    const std::string modulated_excitation_desc		= "True (i.e. 1) if there is any form of excitation modulation of excitation wavelength (as in us-ALEX or PAX) or polarization. This field is also True for pulse-interleaved excitation (PIE) or ns-ALEX measurements.";

    const std::string lifetime_name					= "lifetime";
    const std::string lifetime_desc					= "True (i.e. 1) if the measurement includes a nanotimes array of photon arrival times with respect to a laser pulse (as in TCSPC measurements).";

    const std::string excitation_wavelengths_name	= "excitation_wavelengths";
    const std::string excitation_wavelengths_desc	= "List of excitation wavelengths (center wavelength if broad-band) in increasing order (unit: meter).";

    const std::string excitation_cw_name			= "excitation_cw";
    const std::string excitation_cw_desc			= "For each excitation source, this field indicates whether excitation is continuous wave (CW), True (i.e. 1), or pulsed, False (i.e. 0).";

    const std::string excitation_input_powers_name	= "excitation_input_powers";
    const std::string excitation_input_powers_desc	= "Excitation power in Watts for each excitation source. This is the excitation power entering the optical system.";

    const std::string detection_wavelengths_name	= "detection_wavelengths";
    const std::string detection_wavelengths_desc	= "Reference wavelengths (units: meter) for each detected spectral band.";

    const std::string sample_name					= "/sample";
    const std::string sample_desc					= "Information about the measured sample.";

    const std::string num_dyes_name					= "num_dyes";
    const std::string num_dyes_desc					= "Number of different dyes present in the samples.";

    const std::string dye_names_name				= "dye_names";
    const std::string dye_names_desc				= "String containing a comma-separated list of dye or fluorophore names.";

    const std::string buffer_name_name				= "buffer_name";
    const std::string buffer_name_desc				= "A descriptive name for the buffer.";

    const std::string sample_name_name				= "sample_name";
    const std::string sample_name_desc				= "A descriptive name for the sample.";

    const std::string identity_name					= "/identity";
    const std::string identity_desc					= "Information about the Photon-HDF5 data file.";

    const std::string creation_time_name			= "creation_time";
    const std::string creation_time_desc			= "Creation time of the current Photon-HDF5 file.";

    const std::string software_name					= "software";
    const std::string software_desc					= "Name of the software used to create the current Photon-HDF5 file.";

    const std::string software_version_name			= "software_version";
    const std::string software_version_desc			= "Version of the software used to create current the Photon-HDF5 file.";

    const std::string format_name_name				= "format_name";
    const std::string format_name_desc				= "Name of the file format.";

    const std::string format_version_name			= "format_version";
    const std::string format_version_desc			= "Version for the Photon-HDF5 format.";

    const std::string format_url_name				= "format_url";
    const std::string format_url_desc				= "Official URL for the Photon-HDF5 format.";

    const std::string author_name					= "author";
    const std::string author_desc					= "Author of the current data file.";

    const std::string author_affiliation_name		= "author_affiliation";
    const std::string author_affiliation_desc		= "Company or institution the author is affiliated with.";

    const std::string creator_name					= "creator";
    const std::string creator_desc					= "The Photon-HDF5 file creator. Used when the data was previously stored in another format and the conversion is performed by a different person than the author.";

    const std::string creator_affiliation_name		= "creator_affiliation";
    const std::string creator_affiliation_desc		= "The company or institution the creator is affiliated with.";

    const std::string timestamps_name				= "timestamps";
    const std::string timestamps_desc				= "Array of photon timestamps. Units specified in timestamps_units (defined in timestamps_specs/).";

    const std::string detectors_name				= "detectors";
    const std::string detectors_desc				= "Array of pixel IDs for each timestamp.";

    const std::string laser_power_name				= "laser_power";
    const std::string laser_power_desc				= "Array containing the instantaneous laser power at several timepoints during the experiment";

    const std::string laser_power_interval_name		= "laser_power_interval";
    const std::string laser_power_interval_desc		= "The time between laser power readings";


    class bad_photon_hdf5_error : public std::exception
    {
    public:
        bad_photon_hdf5_error(char const* const errMsg)
        : std::exception(errMsg), m_errMsg(errMsg) {}

        char const* what() const override
        {
            return m_errMsg.c_str();
        }
    private:
        std::string m_errMsg;
    };
}

H5::PredType createStringType(const std::string& str)
{
    auto strType = H5::PredType(H5::PredType::C_S1);
    strType.setSize(str.length());
    return strType;
}

void writeStringAttribute(const std::string& str, const std::string& name, H5::H5Object& object, bool utf8 = false)
{
    auto strType = H5::StrType(H5::PredType::C_S1, str.length());
    if (utf8)
        strType.setCset(H5T_CSET_UTF8);
    auto strAttr = object.createAttribute(name, strType, H5::DataSpace(H5S_SCALAR));
    strAttr.write(strType, str);
}

template<typename T>
void writeDataset(const std::vector<T>& values, const std::string& name, const std::string& description, const H5::PredType& dataType, const H5::PredType& saveType,  H5::Group& group, bool forceArray = false)
{
    hsize_t dim = values.size();
    auto dataSpace = (dim == 1 && ! forceArray) ? H5::DataSpace(H5S_SCALAR) : H5::DataSpace(1, &dim);
    auto dataSet = group.createDataSet(name, saveType, dataSpace);

    if constexpr (std::is_same_v<T, std::string>)
        dataSet.write(values[0], dataType);
    else
        dataSet.write(values.data(), dataType);

    writeStringAttribute(description, "TITLE", dataSet);
}

void writeStringDataset(const std::string& str, const std::string& name, const std::string& description, H5::Group& group)
{
    auto type = H5::StrType(H5::PredType::C_S1, str.length());
    auto dataSpace = H5::DataSpace(H5S_SCALAR);
    auto dataSet = group.createDataSet(name, type, dataSpace);
    dataSet.write(str, type);

    writeStringAttribute(description, "TITLE", dataSet);
    writeStringAttribute("ARRAY", "CLASS", dataSet, true);
    writeStringAttribute("python", "FLAVOR", dataSet, true);
    writeStringAttribute("2.4", "VERSION", dataSet, true);
}

H5::Group createGroup(const std::string& name, const std::string& description, H5::H5File& file)
{
    H5::Group grp = file.createGroup(name);
    writeStringAttribute(description, "TITLE", grp);
    return grp;
}

void createExpandableDataset(const std::string& name, const std::string& description, const H5::PredType& type, int compressionLevel, H5::Group& group)
{
    hsize_t maxDim = H5S_UNLIMITED;
    hsize_t chunkDim = 1000;
    hsize_t initialDim = 0;

    auto propList = H5::DSetCreatPropList();
    propList.setChunk(1, &chunkDim);
    propList.setDeflate(compressionLevel);

    auto dataSpace = H5::DataSpace(1, &initialDim, &maxDim);
    auto dataSet = group.createDataSet(name, type, dataSpace, propList);
    writeStringAttribute(description, "TITLE", dataSet);
}


template<typename T>
void expandDataset(const std::vector<T>& newData, const std::string& name, const H5::PredType& dataType, H5::Group& group)
{
    hsize_t extraSize = newData.size();

    auto dataset = group.openDataSet(name);
    auto space = dataset.getSpace();

    if (space.getSimpleExtentNdims() != 1)
        throw PhotonHDF5::bad_photon_hdf5_error(("Unexpected Dimensionality for Dataset " + name).c_str());

    hsize_t currentDim;
    space.getSimpleExtentDims(&currentDim);
    auto newDim = currentDim + newData.size();

    dataset.extend(&newDim);

    auto newFileSpace = dataset.getSpace();
    newFileSpace.selectHyperslab(H5S_SELECT_SET, &extraSize, &currentDim);

    auto newMemSpace = H5::DataSpace(1, &extraSize, nullptr);

    dataset.write(newData.data(), dataType, newMemSpace, newFileSpace);
}

#endif // PHOTONHDF5HELPER_H
