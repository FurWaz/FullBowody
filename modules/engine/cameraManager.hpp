#include <vector>
#include "./camera.hpp"

namespace owo
{
    class CameraManager
    {
    private:
        std::vector<Camera*> cameras;
        bool istracking;

    public:
        CameraManager()
        {
            this->istracking = false;
        }

        void addCamera(Camera* cam)
        {
            this->cameras.push_back(cam);
        }

        void toogleCameraTracking()
        {
            this->istracking = !this->istracking;
            std::cout << (this->istracking? "Starting": "Stopping") << " camera position tracking" << std::endl;
            return; // stopping the script because the tracking part is not done yet
            if (this->istracking)
                for(Camera* c: this->cameras)
                    c->getTracker()->startTracking();
            else
                for(Camera* c: this->cameras)
                    c->getTracker()->stopTracking();
        }

        ~CameraManager()
        {

        }
    };
}