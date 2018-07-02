#ifndef COMPUTERVISIONLIB_COLORTRACKING_H
#define COMPUTERVISIONLIB_COLORTRACKING_H

#include <CDPSystem/Base/CDPComponent.h>
#include <Signal/CDPSignal.h>
#include <CDPParameter/CDPParameter.h>
#include <CDPAlarm/CDPAlarm.h>
#include <OSAPI/Process/OSAPIThread.h>
#include <OSAPI/Process/OSAPIEvent.h>
#include <opencv2/opencv.hpp>

namespace ComputerVisionLib {

class ColorTracking : public CDPComponent
{
public:
    ~ColorTracking() override;

    virtual void Create(const char* fullName) override;
    virtual void CreateModel() override;
    virtual void Configure(const char* componentXML) override;
    void ProcessNull() override;
    virtual void Activate();
private:
    void Main();
    CDPSignal<int> xCoord;
    CDPSignal<int> yCoord;

    using CDPComponent::requestedState;
    using CDPComponent::ts;
    using CDPComponent::fs;

    bool calibrationMode = true;
    cv::VideoCapture capture;
};

} // namespace ComputerVisionLib

#endif
