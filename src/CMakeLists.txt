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
)

set(RESOURCE
icon.icns
)

set(DYLIB
${KINECT_DEP}/lib/libOpenNI.dylib
${KINECT_DEP}/lib/libusb-1.0.0.dylib
)

SOURCE_GROUP("Resources" FILES ${RESOURCE} ${DYLIB})

add_executable(angryman MACOSX_BUNDLE ${SOURCE} ${RESOURCE} ${DYLIB})
target_link_libraries(angryman ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARIES} libOpenNI.dylib)

SET( MACOSX_BUNDLE_INFO_STRING "Jiawei's Viewer")
SET( MACOSX_BUNDLE_BUNDLE_VERSION "2.0.0")
SET( MACOSX_BUNDLE_ICON_FILE "icon.icns" )
SET( MACOSX_BUNDLE_GUI_IDENTIFIER "dartmouth.edu")
SET( MACOSX_BUNDLE_BUNDLE_NAME "J Viewer")
SET( MACOSX_BUNDLE_COPYRIGHT "Jiawei Ou allright reserved..")

SET_SOURCE_FILES_PROPERTIES(icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

SET_SOURCE_FILES_PROPERTIES(${DYLIB} PROPERTIES MACOSX_PACKAGE_LOCATION Frameworks)
