#include <openpose/headers.hpp>

// If the user needs his own variables, he can inherit the op::Datum struct and add them
// UserDatum can be directly used by the OpenPose wrapper because it inherits from op::Datum, just define
// Wrapper<UserDatum> instead of Wrapper<op::Datum>
struct UserDatum : public op::Datum
{
    bool boolThatUserNeedsForSomeReason;

    UserDatum(const bool boolThatUserNeedsForSomeReason_ = false) :
        boolThatUserNeedsForSomeReason{boolThatUserNeedsForSomeReason_}
    {}
};


using CallbackFunction = std::function<void(const std::vector<UserDatum>& arg1)>;

int openPoseTutorialWrapper(const std::vector<cv::Mat>& input_data, CallbackFunction callback);
