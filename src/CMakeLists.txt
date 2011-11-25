set(SOURCE
debug.h
func.h
vec.h
array.h
driver.h
driver.cpp
stdmath.h
stdmath.cpp
main.cpp
motion.h
motion.cpp
)

set(RESOURCE
icon.icns
)

set(DYLIB
${KINECT_DEP}/lib/libusb-1.0.0.dylib
${KINECT_DEP}/lib/libOpenNI.dylib
${KINECT_DEP}/lib/libXnCore.dylib
${KINECT_DEP}/lib/libXnDDK.dylib
${KINECT_DEP}/lib/libXnDeviceFile.dylib
${KINECT_DEP}/lib/libXnDeviceSensorV2KM.dylib
${KINECT_DEP}/lib/libXnFormats.dylib
${KINECT_DEP}/lib/libXnVCNITE_1_4_1.dylib
${KINECT_DEP}/lib/libXnVFeatures_1_4_1.dylib
${KINECT_DEP}/lib/libXnVHandGenerator_1_4_1.dylib
${KINECT_DEP}/lib/libXnVNite_1_4_1.dylib
)

set(CFG_FILES
${KINECT_DEP}/config/modules.xml
${KINECT_DEP}/config/licenses.xml
${KINECT_DEP}/config/FeatureExtraction.ini
${KINECT_DEP}/config/h.dat
${KINECT_DEP}/config/s.dat
${KINECT_DEP}/config/GlobalDefaults.ini
${KINECT_DEP}/config/Nite.ini
)

SOURCE_GROUP("Resources" FILES ${RESOURCE})
SOURCE_GROUP("Resources\\Dynamic Libs" FILES ${DYLIB})
SOURCE_GROUP("Resources\\Config Files" FILES ${CFG_FILES})

add_executable(angryman MACOSX_BUNDLE ${SOURCE} ${RESOURCE} ${DYLIB} ${CFG_FILES})
target_link_libraries(angryman ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARIES} ${DYLIB} ${BOX2D})

SET( MACOSX_BUNDLE_INFO_STRING "Jiawei's Viewer")
SET( MACOSX_BUNDLE_BUNDLE_VERSION "2.0.0")
SET( MACOSX_BUNDLE_ICON_FILE "icon.icns" )
SET( MACOSX_BUNDLE_GUI_IDENTIFIER "dartmouth.edu")
SET( MACOSX_BUNDLE_BUNDLE_NAME "J Viewer")
SET( MACOSX_BUNDLE_COPYRIGHT "Jiawei Ou allright reserved..")

SET_SOURCE_FILES_PROPERTIES(icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
SET_SOURCE_FILES_PROPERTIES(${DYLIB} PROPERTIES MACOSX_PACKAGE_LOCATION Frameworks)
SET_SOURCE_FILES_PROPERTIES(${CFG_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
