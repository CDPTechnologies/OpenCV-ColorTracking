#ifndef COMPUTERVISIONLIB_CAMERA_H
#define COMPUTERVISIONLIB_CAMERA_H

#include <CDPSystem/Base/CDPComponent.h>
#include <Signal/CDPSignal.h>
#include <CDPParameter/CDPParameter.h>
#include <CDPAlarm/CDPAlarm.h>
#include <OSAPI/Process/OSAPIThread.h>
#include <OSAPI/Process/OSAPIEvent.h>
#include <opencv2/opencv.hpp>
#include <CDPSystem/Base/CDPProperty.h>


namespace ComputerVisionLib {
class ColorFilter;

class Camera : public CDPComponent
{
public:
    Camera();
    ~Camera() override;

    virtual void Create(const char* fullName) override;
    virtual void CreateModel() override;
    virtual void Configure(const char* componentXML) override;
    void ProcessNull() override;
    bool HandleXMLElement(XMLElementEx *pEx) override;
    void FillNodeChildren(CDP::StudioAPI::NodeStream &serializer) const override;
    virtual void Activate();
protected:
    virtual void Main();

    using CDPComponent::requestedState;
    using CDPComponent::ts;
    using CDPComponent::fs;

private:
    std::vector<ColorFilter*> m_filters;
    cv::VideoCapture m_capture;
    cv::Mat m_currentFrame;
    CDPProperty<int> p_resolutionY;
    CDPProperty<int> p_resolutionX;
    CDPProperty<bool> p_renderImage;
};

} // namespace ComputerVisionLib

#endif
