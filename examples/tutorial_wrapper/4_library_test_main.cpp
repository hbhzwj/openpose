#include <chrono>  // `std::chrono::` functions and classes, e.g. std::chrono::milliseconds
#include <thread>  // std::this_thread
// Other 3rdparty dependencies
// GFlags: DEFINE_bool, _int32, _int64, _uint64, _double, _string
#include <gflags/gflags.h>
// Allow Google Flags in Ubuntu 14
#ifndef GFLAGS_GFLAGS_H_
namespace gflags = google;
#endif
// OpenPose dependencies
#include <openpose/headers.hpp>

// Producer
DEFINE_string(image_path, "examples/media/COCO_val2014_000000000241.jpg",
              "Image path");

void TestFunction(const std::vector<LibraryUserDatum>& op_data) {
  op::log("Test Function is being called. op_data.size=" +
              std::to_string(op_data.size()),
          op::Priority::High);
  if (op_data.size() == 0) {
    return;
  }
  LibraryUserDatum ud = op_data[0];
  op::log("Left hand keypoints: " + ud.handKeypoints[0].toString());
  op::log("Right hand keypoints: " + ud.handKeypoints[1].toString());

  const auto& poseKeypoints = ud.poseKeypoints;
  op::log("Person pose keypoints:");
  for (auto person = 0; person < poseKeypoints.getSize(0); person++) {
    op::log("Person " + std::to_string(person) + " (x, y, score): ");
    for (auto bodyPart = 0; bodyPart < poseKeypoints.getSize(1); bodyPart++) {
      std::string valueToPrint;
      for (auto xyscore = 0; xyscore < poseKeypoints.getSize(2); xyscore++) {
        valueToPrint +=
            std::to_string(poseKeypoints[{person, bodyPart, xyscore}]) + " ";
      }
      op::log(valueToPrint);
    }
  }
}

int main(int argc, char* argv[]) {
  // Parsing command line flags
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // Running openPoseTutorialWrapper2
  cv::Mat test_data = cv::imread(FLAGS_image_path, CV_LOAD_IMAGE_COLOR);
  return openPoseTutorialWrapper(std::vector<cv::Mat>({test_data}),
                                 TestFunction);
}
