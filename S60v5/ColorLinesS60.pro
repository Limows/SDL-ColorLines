# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
res_dir.source = ColorLinesData
res_dir.target = \private\e336d524
DEPLOYMENTFOLDERS += res_dir

res_dir.source = ColorLinesData

symbian:TARGET.UID3 = 0xE336D524

# Smart Installer package's UID
# This UID is from the protected range 
# and therefore the package will fail to install if self-signed
# By default qmake uses the unprotected range value if unprotected UID is defined for the application
# and 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
#symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

symbian {

    # Note, on Symbian you may run into troubles with LGPL.
    # The point is, if your application uses some version of QuaZip,
    # and a newer binary compatible version of QuaZip is released, then
    # the users of your application must be able to relink it with the
    # new QuaZip version. For example, to take advantage of some QuaZip
    # bug fixes.

    # This is probably best achieved by building QuaZip as a static
    # library and providing linkable object files of your application,
    # so users can relink it.

    SOURCES += main.cpp
    INCLUDEPATH += C:\\build_c\\symbian\\S60_5th_Edition_SDK_v1.0\\SDL-1.2.14\\symbian\\inc
    LIBS += -lSDL -lsdlexe

    vendorinfo = "%{\"LimSoft\"}" ":\"LimSoft\""

    ColorLines_template.pkg_prerules = vendorinfo
    DEPLOYMENT += ColorLines_template

    CONFIG += qt
}

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

RESOURCES += \
    resources.qrc
