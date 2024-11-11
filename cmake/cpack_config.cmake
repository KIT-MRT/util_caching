# CPACK_PACKAGE_NAME and CPACK_PACKAGE_DESCRIPTION_SUMMARY are loaded from package variables
set(CPACK_PACKAGE_VENDOR "Karlsruhe Institute of Technology")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/KIT-MRT/util_caching")

set(CPACK_VERBATIM_VARIABLES YES)

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/packages")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "https://github.com/orzechow")

set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")


set(CPACK_GENERATOR "TGZ;ZIP;DEB")

set(CPACK_DEBIAN_PACKAGE_NAME "libutil-caching-dev")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}")
set(CPACK_DEBIAN_FILE_NAME "${CPACK_DEBIAN_PACKAGE_NAME}.deb")

set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)
set(CPACK_DEB_COMPONENT_INSTALL YES)
set(CPACK_ARCHIVE_COMPONENT_INSTALL YES)

set(CPACK_DEBIAN_PACKAGE_SECTION "libdevel")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "all")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
