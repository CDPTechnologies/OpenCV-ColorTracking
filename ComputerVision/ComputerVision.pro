CDPVERSION = 4.5
TYPE = system
load(cdp)

DISTFILES += $$files(*.xml, false)

SUBDIRS += \
    ComputerVisionApp
