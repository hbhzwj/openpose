#include <openpose/utilities/fastMath.hpp>
#include <openpose/utilities/string.hpp>
#include <openpose/producer/kinectReader.hpp>
#include <iostream>

namespace op
{
    KinectReader::KinectReader(const std::string& cameraParametersPath, const Point<int>& cameraResolution) :
        Producer{ProducerType::KinectCamera},
        mFrameNameCounter{0},
        undistorted(libfreenect2::Frame(512, 424, 4)), 
        registered(libfreenect2::Frame(512, 424, 4))
    {
        try
				{

						std::string serial = "";

						pipeline = new libfreenect2::CpuPacketPipeline();
            // pipeline = new libfreenect2::OpenGLPacketPipeline();

						if(freenect2.enumerateDevices() == 0)
						{
								op::log("no device connected!", op::Priority::High);
								return;
						}
						serial = freenect2.getDefaultDeviceSerialNumber();

						if(pipeline) {
								dev = freenect2.openDevice(serial, pipeline);
						}

						if(dev == 0)
						{
								std::cout << "failure opening device!" << std::endl;
								return;
						}

						int types = libfreenect2::Frame::Color | libfreenect2::Frame::Ir | libfreenect2::Frame::Depth;
						// int types = libfreenect2::Frame::Ir;

            listener = new libfreenect2::SyncMultiFrameListener(types);

						if (!dev->start())
						{
								return;
						}

						std::cout << "device serial: " << dev->getSerialNumber() << std::endl;
						std::cout << "device firmware: " << dev->getFirmwareVersion() << std::endl;

						dev->setColorFrameListener(listener);
						dev->setIrAndDepthFrameListener(listener);

						registration =
								new libfreenect2::Registration(dev->getIrCameraParams(),
												dev->getColorCameraParams());

						// Get resolution
						// const auto resolution = mSpinnakerWrapper.getResolution();
						// Set resolution
						// set(CV_CAP_PROP_FRAME_WIDTH, resolution.x);
						// set(CV_CAP_PROP_FRAME_HEIGHT, resolution.y);
						// set(CV_CAP_PROP_FRAME_WIDTH, 512);
						// set(CV_CAP_PROP_FRAME_HEIGHT, 424);

						set(CV_CAP_PROP_FRAME_WIDTH, 1920);
						set(CV_CAP_PROP_FRAME_HEIGHT, 1082);
           //1920, 1082,

        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }

    KinectReader::~KinectReader()
    {
        try
        {
            release();
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }

    std::vector<cv::Mat> KinectReader::getCameraMatrices()
    {
      return {};
    }

    std::string KinectReader::getNextFrameName()
    {
      return "";
    }

    bool KinectReader::isOpened() const
    {
      return true;
    }

    void KinectReader::release()
    {
				// delete registration;
				// dev->stop();
				// dev->close();
        //delete dev;
        //delete pipeline;
				// delete listener;
    }

    cv::Mat KinectReader::getRawFrame()
    {
      log("enter getRawFrame");

				if (!listener->waitForNewFrame(frames, 10*1000)) // 10 sconds
				{
						std::cout << "timeout!" << std::endl;
						return cv::Mat();
				}
				libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
				libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
				libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];

				registration->apply(rgb, depth, &undistorted, &registered);
				cv::Mat rgbMat = cv::Mat(
								depth->height, depth->width,
								CV_32FC1, depth->data);
				listener->release(frames);
        return rgbMat;
    }

    std::vector<cv::Mat> KinectReader::getRawFrames()
		{
        log("enter getRawFrames");

        if (!listener->waitForNewFrame(frames, 10 * 1000)) // 10 milli sconds
        {
            log("timeout!");
            return {};
        }
        log("run to line 140 get frames");
        libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
        libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
        libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];
        log("data size:"+ std::to_string(depth->width));

        // registration->apply(rgb, depth, &undistorted, &registered);
				// cv::Mat rgbMat = cv::Mat(
				//         ir->height, ir->width,
				//         CV_32FC1, ir->data);

				int colorDepthIndex[512 * 424];
				libfreenect2::Frame bigDepth(1920, 1082, 4);

				registration->apply(rgb, depth, &undistorted, &registered, true, &bigDepth, colorDepthIndex);
				// cv::Mat temp1 = cv::Mat(1082, 1920, CV_32F, bigDepth.data);
				cv::Mat temp1 = cv::Mat(1082, 1920, CV_32F, rgb->data);
				cv::Mat temp2;
				temp1.convertTo(temp2, CV_16UC1);
				// cv::imshow("depth", 50 * temp2);

        listener->release(frames);
        return {temp2};

        // try
        // {
        //     mFrameNameCounter++; // Simple counter: 0,1,2,3,...
        //     return mSpinnakerWrapper.getRawFrames();
        // }
        // catch (const std::exception& e)
        // {
        //     error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        //     return {};
        // }
    }

    double KinectReader::get(const int capProperty)
    {
        try
        {
            if (capProperty == CV_CAP_PROP_FRAME_WIDTH)
            {
                if (Producer::get(ProducerProperty::Rotation) == 0.
                    || Producer::get(ProducerProperty::Rotation) == 180.)
                    return mResolution.x;
                else
                    return mResolution.y;
            }
            else if (capProperty == CV_CAP_PROP_FRAME_HEIGHT)
            {
                if (Producer::get(ProducerProperty::Rotation) == 0.
                    || Producer::get(ProducerProperty::Rotation) == 180.)
                    return mResolution.y;
                else
                    return mResolution.x;
            }
            else if (capProperty == CV_CAP_PROP_POS_FRAMES)
                return (double)mFrameNameCounter;
            else if (capProperty == CV_CAP_PROP_FRAME_COUNT)
                return -1.;
            else if (capProperty == CV_CAP_PROP_FPS)
                return -1.;
            else
            {
                log("Unknown property.", Priority::Max, __LINE__, __FUNCTION__, __FILE__);
                return -1.;
            }
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
            return 0.;
        }
    }

    void KinectReader::set(const int capProperty, const double value)
    {
        try
        {
            if (capProperty == CV_CAP_PROP_FRAME_WIDTH)
                mResolution.x = {(int)value};
            else if (capProperty == CV_CAP_PROP_FRAME_HEIGHT)
                mResolution.y = {(int)value};
            else if (capProperty == CV_CAP_PROP_POS_FRAMES)
                log("This property is read-only.", Priority::Max, __LINE__, __FUNCTION__, __FILE__);
            else if (capProperty == CV_CAP_PROP_FRAME_COUNT || capProperty == CV_CAP_PROP_FPS)
                log("This property is read-only.", Priority::Max, __LINE__, __FUNCTION__, __FILE__);
            else
                log("Unknown property.", Priority::Max, __LINE__, __FUNCTION__, __FILE__);
        }
        catch (const std::exception& e)
        {
            error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }
}
