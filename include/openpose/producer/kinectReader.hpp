#ifndef OPENPOSE_PRODUCER_KINECT_READER_HPP
#define OPENPOSE_PRODUCER_KINECT_READER_HPP

#include <openpose/core/common.hpp>
#include <openpose/producer/producer.hpp>
#include <openpose/producer/spinnakerWrapper.hpp>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>


namespace op
{
    /**
     * KinectReader is an abstract class to extract frames from a FLIR stereo-camera system. Its interface imitates the
     * cv::VideoCapture class, so it can be used quite similarly to the cv::VideoCapture class. Thus,
     * it is quite similar to VideoReader and WebcamReader.
     */
    class OP_API KinectReader : public Producer
    {
    public:
        /**
         * Constructor of KinectReader. It opens all the available FLIR cameras
         */
        explicit KinectReader(const std::string& cameraParametersPath, const Point<int>& cameraResolution);

        ~KinectReader();

        std::vector<cv::Mat> getCameraMatrices();

        std::string getNextFrameName();

        bool isOpened() const;

        void release();

        double get(const int capProperty);

        void set(const int capProperty, const double value);

    private:
        SpinnakerWrapper mSpinnakerWrapper;
        Point<int> mResolution;
        long long mFrameNameCounter;

        cv::Mat getRawFrame();

        std::vector<cv::Mat> getRawFrames();

				libfreenect2::Freenect2 freenect2;
				libfreenect2::Freenect2Device *dev;
				libfreenect2::PacketPipeline *pipeline;
				std::string serial;

        DELETE_COPY(KinectReader);
    };
}

#endif // OPENPOSE_PRODUCER_KINECT_READER_HPP
