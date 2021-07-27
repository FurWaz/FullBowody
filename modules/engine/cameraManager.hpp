#include <vector>
#include "./camera.hpp"

namespace owo
{
    class CameraManager
    {
    private:
        std::vector<Camera*> cameras;
        bool tracking;

    public:
        CameraManager()
        {
            this->tracking = false;
        }

        void addCamera(Camera* cam)
        {
            this->cameras.push_back(cam);
        }

        void toogleCameraTracking()
        {
            bool possible = true;
            for (Camera* c: this->cameras)
                possible = possible & c->getPath() != "";
            if (!possible)
            {
                std::cout << "A camera does not have video source" << std::endl;
                return;
            }

            if (this->tracking)
                for (Camera* c: this->cameras)
                    c->getTracker()->stopTracking();
            else
                for (Camera* c: this->cameras)
                    c->getTracker()->startTracking();

            this->tracking = !this->tracking;
            std::cout << (this->tracking? "Starting": "Stopping") << " camera position tracking" << std::endl;
            this->tracking = !this->tracking;
        }

        ~CameraManager()
        {

        }
    };
}