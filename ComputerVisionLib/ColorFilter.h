#ifndef COMPUTERVISIONLIB_COLORFILTER_H
#define COMPUTERVISIONLIB_COLORFILTER_H

#include <StudioAPI/CDPNode.h>
#include <CDPSystem/Base/CDPProperty.h>
#include <opencv2/opencv.hpp>

namespace ComputerVisionLib {

class ColorFilter : public CDP::StudioAPI::CDPNode
{
public:
    ColorFilter(XMLElementEx *element, CDPBaseObject *owner);

    virtual const std::string GetNodeName() const override;
    virtual std::string GetNodeTypeName() const override;
    virtual void FillNodeChildren(CDP::StudioAPI::NodeStream &serializer) const override;

    cv::Mat ApplyFilter(cv::Mat frame);
private:
    CDPProperty<std::string> nodeName;
    CDPProperty<int> HMIN;
    CDPProperty<int> HMAX;
    CDPProperty<int> SMIN;
    CDPProperty<int> SMAX;
    CDPProperty<int> VMIN;
    CDPProperty<int> VMAX;
};

} // namespace ComputerVisionLib

#endif
