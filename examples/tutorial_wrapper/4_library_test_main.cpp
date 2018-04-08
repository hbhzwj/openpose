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
DEFINE_string(image_path1, "examples/media/COCO_val2014_000000000241.jpg",
              "Image path");
DEFINE_string(image_path2, "examples/media/COCO_val2014_000000000328.jpg",
              "Image path");


void TestFunction(const std::vector<LibraryUserDatum>& op_data) {
  op::log("Test Function is being called. op_data.size=" +
              std::to_string(op_data.size()),
          op::Priority::High);
  if (op_data.size() == 0) {
    return;
  }

  int i = -1;
  for (const auto &ud : op_data) {
    ++i;
    op::log("Image: " + std::to_string(i) + " Left hand keypoints: " +
            ud.handKeypoints[0].toString());
    op::log("Image: " + std::to_string(i) + " Right hand keypoints: " +
            ud.handKeypoints[1].toString());

    const auto &poseKeypoints = ud.poseKeypoints;
    op::log("Image: " + std::to_string(i) + " Person pose keypoints:");
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
}

int main(int argc, char* argv[]) {
  // Parsing command line flags
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // Running openPoseTutorialWrapper2
  cv::Mat test_data1 = cv::imread(FLAGS_image_path1, CV_LOAD_IMAGE_COLOR);
  cv::Mat test_data2 = cv::imread(FLAGS_image_path2, CV_LOAD_IMAGE_COLOR);
  return openPoseTutorialWrapper(std::vector<cv::Mat>({test_data1, test_data2}),
                                 TestFunction);
}
