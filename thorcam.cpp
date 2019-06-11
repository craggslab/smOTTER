#include "thorcam.h"

namespace ThorCam {
    ThorCam::ThorCam() : m_cam(0), m_initialised(false) { }
    ThorCam::ThorCam(int id)
        : m_cam(static_cast<HCAM>(id)),
          m_initialised(false)
    {
        if (is_InitCamera(&m_cam, nullptr) != IS_SUCCESS)
        {
            m_cam = 0;
            throw std::exception("Failed to initialise camera");
        }

        if (is_SetDisplayMode(m_cam, IS_SET_DM_DIB) != IS_SUCCESS)
        {
            cleanup();
            throw std::exception("Failed to set render mode");
        }

        SENSORINFO pInfo;
        if (is_GetSensorInfo(m_cam, &pInfo) != IS_SUCCESS)
        {
            cleanup();
            throw std::exception("Failed to query sensor info!");
        }

        if (pInfo.nColorMode != IS_COLORMODE_MONOCHROME)
        {
            cleanup();
            throw std::exception("Color cameras not supported!");
        }

        m_width = pInfo.nMaxWidth; m_height = pInfo.nMaxHeight;

        if (is_SetExternalTrigger(m_cam, IS_SET_TRIGGER_SOFTWARE) != IS_SUCCESS)
        {
            cleanup();
            throw std::exception("Failed to set trigger mode!");
        }

        if (is_SetColorMode(m_cam, IS_CM_SENSOR_RAW8) != IS_SUCCESS)
        {
            cleanup();
            throw std::exception("Failed to set bit depth");
        }

        UINT clock = 16;
        is_PixelClock(m_cam, IS_PIXELCLOCK_CMD_SET, &clock, sizeof(UINT));

        m_image.resize(m_width * m_height);
        if (is_SetAllocatedImageMem(m_cam, static_cast<INT>(m_width), static_cast<INT>(m_height), 8, reinterpret_cast<char*>(m_image.data()), &m_bufId) != IS_SUCCESS)
        {
            cleanup();
            throw std::exception("Failed to set allocated image buffer");
        }

        m_initialised = true;
    }

    ThorCam& ThorCam::operator=(ThorCam&& other) noexcept {
        if (this != &other) {
            this->cleanup();
            m_cam = std::exchange(other.m_cam, 0);
            m_format = std::exchange(other.m_format, -1);
            m_width = std::exchange(other.m_width, 0);
            m_height = std::exchange(other.m_height, 0);
            m_bufId = std::exchange(other.m_bufId, -1);
            m_initialised = std::exchange(other.m_initialised, false),
            m_image = std::move(other.m_image);
        }

        return *this;
    }

    ThorCam::ThorCam(ThorCam&& other) noexcept { *this = std::move(other); }

    void ThorCam::cleanup() {
        if (m_cam != 0)
        {
            if (m_bufId != -1)
                is_FreeImageMem(m_cam, reinterpret_cast<char*>(m_image.data()), m_bufId);
            is_ExitCamera(m_cam);
            m_cam = 0;
        }
    }

    void ThorCam::start() {
        if (m_cam == 0 || !m_initialised) return;
        if (is_CameraStatus(m_cam, IS_STANDBY_SUPPORTED, 0) == IS_SUCCESS) {
            is_CameraStatus(m_cam, IS_STANDBY, FALSE);
        }
    }

    void ThorCam::stop() {
        if (m_cam == 0 || !m_initialised) return;
        if (is_CameraStatus(m_cam, IS_STANDBY_SUPPORTED, 0) == IS_SUCCESS) {
            is_CameraStatus(m_cam, IS_STANDBY, FALSE);
        }
    }

    void ThorCam::snap() {
        if (m_cam == 0)
            throw std::exception("No camera selected!");

        if (!m_initialised)
            throw std::exception("Camera not initialised");

        if (auto err = is_SetImageMem(m_cam, reinterpret_cast<char*>(m_image.data()), m_bufId); err != IS_SUCCESS)
        {
            throw std::exception(("Failed to set image memory! " + std::to_string(err)).c_str());
        }

        if (auto res = is_FreezeVideo(m_cam, IS_WAIT); res != IS_SUCCESS)
        {
            throw std::exception(("Failed to snap image! " + std::to_string(res)).c_str());
        }
    }

    std::vector<uint8_t> ThorCam::getImage() {
        return m_image;
    }

    ThorCam::~ThorCam() { cleanup(); }

    std::vector<std::string> ThorCam::getAvailableIDs() {
        INT nCam;
        if (is_GetNumberOfCameras(&nCam) == IS_SUCCESS) {
            auto out = std::vector<std::string>(static_cast<size_t>(nCam));
            if (nCam >= 1) {
                auto cameraBuff = std::vector<BYTE>(sizeof(DWORD) + static_cast<size_t>(nCam) * sizeof(UC480_CAMERA_INFO));

                UC480_CAMERA_LIST* pucl;
                pucl = reinterpret_cast<UC480_CAMERA_LIST*>(cameraBuff.data());
                pucl->dwCount = static_cast<ULONG>(nCam);

                if (is_GetCameraList(pucl) == IS_SUCCESS) {
                    for (size_t iCamera = 0; iCamera < pucl->dwCount; iCamera++) {
                        out[iCamera] = std::to_string(pucl->uci[iCamera].dwCameraID);
                    }
                }
                else
                {
                        throw std::exception("Failed to query camera list");
                }
            }

            return out;
        }
        else
        {
            throw std::exception("Failed to query cameras");
        }

    }

    bool ThorCam::isConnected() const { return m_initialised; }
    bool ThorCam::isID(int id) const { return static_cast<int>(m_cam) == id; }
    unsigned long ThorCam::getWidth() const { return m_width; }
    unsigned long ThorCam::getHeight() const { return m_height; }

    double ThorCam::getExposureMin() const {
        double min;
        if (is_Exposure(m_cam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE_MIN, &min, sizeof(double)) != IS_SUCCESS)
        throw std::exception("Failed to get minimum exposure time");

        return min;
    }

    double ThorCam::getExposureMax() const {
        double max;
        if (is_Exposure(m_cam, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE_MAX, &max, sizeof(double)) != IS_SUCCESS)
        throw std::exception("Failed to get maximum exposure time");

        return max;
    }

    double ThorCam::getExposure() const {
        double exposure;
        if (is_Exposure(m_cam, IS_EXPOSURE_CMD_GET_EXPOSURE, &exposure, sizeof(double)) != IS_SUCCESS)
        throw std::exception("Failed to get exposure");

        return exposure;
    }

    void ThorCam::setExposure(double exposure) {
        if (is_Exposure(m_cam, IS_EXPOSURE_CMD_SET_EXPOSURE, &exposure, sizeof(double)) != IS_SUCCESS)
        throw std::exception("Failed to set exposure");
    }
}
