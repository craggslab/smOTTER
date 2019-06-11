#ifndef THORCAM_H
#define THORCAM_H

#include <uc480.h>
#include <uc480_tools.h>

#include <exception>
#include <vector>
#include <string>
#include <iostream>

namespace ThorCam {

    class ThorCam {
    public:
        ThorCam();
        ThorCam(int id);

        ThorCam(const ThorCam&) = delete;
        ThorCam& operator=(const ThorCam&) = delete;
        ThorCam& operator=(ThorCam&& other) noexcept;
        ThorCam(ThorCam&& other) noexcept;

        void cleanup();
        void start();
        void stop();

        void snap();
        std::vector<uint8_t> getImage();
        ~ThorCam();

        static std::vector<std::string> getAvailableIDs();

        bool isConnected() const;
        bool isID(int id) const;
        unsigned long getWidth() const;
        unsigned long getHeight() const;

        double getExposureMin() const;
        double getExposureMax() const;

        double getExposure() const;
        void setExposure(double exposure);

    private:
        HCAM m_cam;
        unsigned long m_width, m_height;
        int m_format = -1;
        std::vector<uint8_t> m_image;
        int m_bufId = -1;
        bool m_initialised;
    };

}
#endif // THORCAM_H
