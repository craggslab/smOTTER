QT += quick charts widgets quickcontrols2
CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    nicard.cpp \
    nidatasource.cpp \
    photon.cpp \
    photonhdf5exporter.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L'C:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/lib64/msvc' -lNIDAQmx

INCLUDEPATH += 'C:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/include'
DEPENDPATH += 'C:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/include'

win32:!win32-g++: PRE_TARGETDEPS += 'C:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/lib64/msvc/NIDAQmx.lib'
else:win32-g++: PRE_TARGETDEPS += 'C:/Program Files (x86)/National Instruments/Shared/ExternalCompilerSupport/C/lib64/msvc/NIDAQmx.a'

HEADERS += \
    nicard.h \
    nidatasource.h \
    photon.h \
    photonhdf5exporter.h \
    photonhdf5helper.h

win32:CONFIG(release, debug|release): LIBS += $$PWD/../HDF5/lib64/libhdf5.lib
else:win32:CONFIG(debug, debug|release): LIBS += $$PWD/../HDF5/lib64/libhdf5_D.lib

INCLUDEPATH += $$PWD/../HDF5/include
DEPENDPATH += $$PWD/../HDF5/include

win32:CONFIG(release, debug|release): LIBS += $$PWD/../HDF5/lib64/libhdf5_cpp.lib
else:win32:CONFIG(debug, debug|release): LIBS += $$PWD/../HDF5/lib64/libhdf5_cpp_D.lib

INCLUDEPATH += $$PWD/../HDF5/include
DEPENDPATH += $$PWD/../HDF5/include

win32:CONFIG(release, debug|release): LIBS += $$PWD/../HDF5/lib64/libszip.lib
else:win32:CONFIG(debug, debug|release): LIBS += $$PWD/../HDF5/lib64/libszip_D.lib

INCLUDEPATH += $$PWD/../HDF5/include
DEPENDPATH += $$PWD/../HDF5/include

win32:CONFIG(release, debug|release): LIBS += $$PWD/../HDF5/lib64/libzlib.lib
else:win32:CONFIG(debug, debug|release): LIBS += $$PWD/../HDF5/lib64/libzlib_D.lib

INCLUDEPATH += $$PWD/../HDF5/include
DEPENDPATH += $$PWD/../HDF5/include
