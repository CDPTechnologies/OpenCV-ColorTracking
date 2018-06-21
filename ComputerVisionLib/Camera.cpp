#include "Camera.h"
#include <OSAPI/Process/OSAPISemaphore.h>
#include <CDPSystem/Application/Application.h>

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <ColorFilter.h>

using namespace ComputerVisionLib;
using namespace std;

Camera::Camera()
{
    m_capture.open(0);
}

Camera::~Camera()
{
}

void Camera::Create(const char* fullName)
{
    CDPComponent::Create(fullName);
    p_resolutionY.Create("p_resolutionY",this,CDPPropertyBase::e_Attribute,(CDPOBJECT_SETPROPERTY_HANDLER)nullptr,(CDPOBJECT_VALIDATEPROPERTY_HANDLER)nullptr);
    p_resolutionX.Create("p_resolutionX",this,CDPPropertyBase::e_Attribute,(CDPOBJECT_SETPROPERTY_HANDLER)nullptr,(CDPOBJECT_VALIDATEPROPERTY_HANDLER)nullptr);
    p_renderImage.Create("p_renderImage",this,CDPPropertyBase::e_Attribute,(CDPOBJECT_SETPROPERTY_HANDLER)nullptr,(CDPOBJECT_VALIDATEPROPERTY_HANDLER)nullptr);
}

void Camera::CreateModel()
{
    CDPComponent::CreateModel();

    RegisterStateProcess("Null", (CDPCOMPONENT_STATEPROCESS)&Camera::ProcessNull, "Initial Null state");
}

void Camera::Configure(const char* componentXML)
{
    CDPComponent::Configure(componentXML);
    m_capture.set(::cv::CAP_PROP_FRAME_WIDTH, p_resolutionX);
    m_capture.set(::cv::CAP_PROP_FRAME_HEIGHT, p_resolutionY);
}

void Camera::ProcessNull()
{
}

void Camera::Activate()
{
    CDPComponent::Activate();
    Application::RunInMainThread([=] { Main(); });
}

void Camera::Main()
{
    m_capture.read(m_currentFrame);

    {
        OSAPIMutexLocker(GetMemberAccessMutex());
        for (auto filter : m_filters)
            filter->ApplyFilter(m_currentFrame); // Accesses CDPProperties, so must lock mutex
    }

    if (p_renderImage)
        imshow(m_shortName, m_currentFrame);
    else
        cv::destroyWindow(m_shortName);

    cv::waitKey(30);
    Application::RunInMainThread([&] { Main(); });
}

void Camera::FillNodeChildren(CDP::StudioAPI::NodeStream &serializer) const
{
    CDPComponent::FillNodeChildren(serializer);
    serializer.StdContainerStreamer(m_filters);
}

bool Camera::HandleXMLElement(XMLElementEx *pEx)
{
    const XMLString& currentElement = pEx->GetName();
    std::string s_filter = "ColorFilter";

    if (currentElement == "ColorFilters") // this is just wrapper element for W1Sensor elements
        return false; //Call handleElement for childern also

    if (currentElement == s_filter.c_str())
    {
        std::string model = pEx->GetAttributeValue("Model");
        CDPMessage(model.c_str(), "\n");
        std::string name = pEx->GetAttributeValue("Name");
        CDPMessage(name.c_str(), "\n");

        ColorFilter* comp = new ColorFilter(pEx, this);
        m_filters.push_back(comp);

        return true;
    }

    return CDPComponent::HandleXMLElement(pEx);
}
