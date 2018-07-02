#include "ColorFilter.h"

#include <StudioAPI/NodeStream.h>
#include <Generic/CDPUtils.h>
#include <CDPSystem/Base/CDPBaseObject.h>

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace CDP::StudioAPI;
using namespace CDP::System::Base;
using namespace ComputerVisionLib;

ColorFilter::ColorFilter(XMLElementEx* element, CDPBaseObject* owner)
{
    std::string name = element->GetAttributeValue("Name");
    auto flags = PropertyCreateFlags() | e_PropertySaveOnChange | e_PropertyReparent;

    PropertyCreate(m_nodeName, owner, std::string(name).append(".Name"), flags, element);

    PropertyCreate(m_HMIN, owner, std::string(name).append(".HMIN"), flags, element);
    PropertyCreate(m_HMAX, owner, std::string(name).append(".HMAX"), flags, element);
    PropertyCreate(m_SMIN, owner, std::string(name).append(".SMIN"), flags, element);
    PropertyCreate(m_SMAX, owner, std::string(name).append(".SMAX"), flags, element);
    PropertyCreate(m_VMIN, owner, std::string(name).append(".VMIN"), flags, element);
    PropertyCreate(m_VMAX, owner, std::string(name).append(".VMAX"), flags, element);
}

const std::string ColorFilter::GetNodeName() const
{
    return m_nodeName;
}

std::string ColorFilter::GetNodeTypeName() const
{
    return "ComputerVisionLib.ColorFilter";
}

void ColorFilter::FillNodeChildren(CDP::StudioAPI::NodeStream &serializer) const
{
    serializer << AdoptedChild(m_nodeName)
               << AdoptedChild(m_HMIN)
               << AdoptedChild(m_HMAX)
               << AdoptedChild(m_SMIN)
               << AdoptedChild(m_SMAX)
               << AdoptedChild(m_VMIN)
               << AdoptedChild(m_VMAX);
}

cv::Mat ColorFilter::ApplyFilter(cv::Mat frame)
{
    cv::Mat threshold;
    cv::Mat HSV;

    cv::cvtColor(frame,HSV,cv::COLOR_BGR2HSV);
    cv::inRange(HSV,cv::Scalar(m_HMIN,m_SMIN,m_VMIN),cv::Scalar(m_HMAX,m_SMAX,m_VMAX),threshold);

    cv::Mat erodeElement = cv::getStructuringElement( cv::MORPH_RECT,cv::Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    cv::Mat dilateElement = cv::getStructuringElement( cv::MORPH_RECT,cv::Size(8,8));

    cv::erode(threshold,threshold,erodeElement);
    cv::erode(threshold,threshold,erodeElement);
    cv::erode(threshold,threshold,erodeElement);


    cv::dilate(threshold,threshold,dilateElement);
    cv::dilate(threshold,threshold,dilateElement);

    cv::imshow(m_nodeName.c_str(),threshold);
    return threshold;
}
