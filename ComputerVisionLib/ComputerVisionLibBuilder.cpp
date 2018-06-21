/**
ComputerVisionLibBuilder implementation.
*/

#include "ColorTracking.h"
#include "ColorFilter.h"
#include "Camera.h"
#include "ComputerVisionLibBuilder.h"

using namespace ComputerVisionLib;

ComputerVisionLibBuilder::ComputerVisionLibBuilder(const char* libName, const char* timeStamp)
    : CDPBuilder(libName, timeStamp)
{
}

CDPComponent* ComputerVisionLibBuilder::CreateNewComponent(const std::string& type)
{
    if (type=="ComputerVisionLib.ColorTracking")
        return new ColorTracking;
    if (type=="ComputerVisionLib.Camera")
        return new Camera;
    
    return CDPBuilder::CreateNewComponent(type);
}

CDPBaseObject* ComputerVisionLibBuilder::CreateNewCDPOperator(const std::string& modelName, const std::string& type, const CDPPropertyBase* inputProperty)
{
    return CDPBuilder::CreateNewCDPOperator(modelName, type, inputProperty);
}
