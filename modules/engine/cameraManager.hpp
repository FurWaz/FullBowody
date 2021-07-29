#include <vector>
#include "./camera.hpp"

namespace owo
{
    /**
     * @brief CameraManager class, only toogle the camera's body trackers for the moment
     */
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

        /**
         * @brief Adds the given camera to the CameraManager's camera list
         * @param cam The camera to add
         */
        void addCamera(Camera* cam)
        {
            this->cameras.push_back(cam);
        }

        /**
         * @brief Toogle all the CameraManager's cameras body trackers for body tracking
         * @return If the cameras' trackers are now on or off
         */
        void toogleCameraTracking()
        {
            bool possible = true;
            for (Camera* c: this->cameras)
                possible = possible & c->getPath() != "";
            if (!possible) return;

            if (this->tracking)
                for (Camera* c: this->cameras)
                    c->getTracker()->stopTracking();
            else
                for (Camera* c: this->cameras)
                    c->getTracker()->startTracking();

            this->tracking = !this->tracking;
        }

        ~CameraManager()
        {

        }
    };
}