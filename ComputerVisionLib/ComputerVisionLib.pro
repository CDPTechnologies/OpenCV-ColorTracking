CDPVERSION = 4.5
TYPE = library
PROJECTNAME = ComputerVisionLib

DEPS += \

HEADERS += \
    computervisionlib.h \
    ComputerVisionLibBuilder.h \
    ColorTracking.h \
    ColorFilter.h \
    Camera.h

SOURCES += \
    ComputerVisionLibBuilder.cpp \
    ColorTracking.cpp \
    ColorFilter.cpp \
    Camera.cpp

DISTFILES += $$files(*.xml, true) \
    Templates/Models/ComputerVisionLib.ColorTracking.xml \
    Templates/Models/ComputerVisionLib.Camera.xml \
    Templates/Models/ComputerVisionLib.ColorFilter.xml

load(cdp)

QT += core gui
