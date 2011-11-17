set(SOURCE
main.cpp
icon.icns
)

add_executable(angryman MACOSX_BUNDLE ${SOURCE})
target_link_libraries(angryman ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARIES})

if(APPLE)
	SET( MACOSX_BUNDLE_INFO_STRING "Jiawei's Viewer")
	SET( MACOSX_BUNDLE_BUNDLE_VERSION "2.0.0")
	SET( MACOSX_BUNDLE_ICON_FILE "icon.icns" )
	SET( MACOSX_BUNDLE_GUI_IDENTIFIER "dartmouth.edu")
	SET( MACOSX_BUNDLE_BUNDLE_NAME "J Viewer")
	SET( MACOSX_BUNDLE_COPYRIGHT "Jiawei Ou allright reserved..")

	SET_SOURCE_FILES_PROPERTIES(icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()