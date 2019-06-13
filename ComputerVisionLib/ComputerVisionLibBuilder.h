/**
ComputerVisionLibBuilder header file.
*/

#ifndef COMPUTERVISIONLIB_COMPUTERVISIONLIBBUILDER_H
#define COMPUTERVISIONLIB_COMPUTERVISIONLIBBUILDER_H

#include <CDPSystem/Application/CDPBuilder.h>

namespace ComputerVisionLib {

class ComputerVisionLibBuilder : public CDPBuilder
{
public:
    ComputerVisionLibBuilder(const char* libName,const char* timeStamp);
    CDPComponent* CreateNewComponent(const std::string& type) override;
    CDPBaseObject* CreateNewCDPOperator(const std::string& modelName,const std::string& type,const CDPPropertyBase* inputProperty) override;
    CDPObject* CreateNewObject(const std::string& type) override;
    CDP::StudioAPI::CDPNode* CreateNewCDPNode(const std::string& type) override;
};

}

#endif // COMPUTERVISIONLIB_COMPUTERVISIONLIBBUILDER_H
