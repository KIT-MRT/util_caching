cmake_minimum_required(VERSION 3.5.1)
project(util_caching)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




############################
## Read source code files ##
############################
file(GLOB_RECURSE PROJECT_HEADER_FILES_INC "include/${PROJECT_NAME}/*.h" "include/${PROJECT_NAME}/*.hpp")


###########
## Build ##
###########
# Declare a cpp library
add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)


###################
## Cmake Package ##
###################
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION 0.1
    COMPATIBILITY AnyNewerVersion
)
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)


#############
## Install ##
#############


install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib COMPONENT Runtime
    ARCHIVE DESTINATION lib COMPONENT Development
    RUNTIME DESTINATION bin COMPONENT Runtime
    PUBLIC_HEADER DESTINATION include COMPONENT Development
    BUNDLE DESTINATION bin COMPONENT Runtime
)
install(EXPORT ${PROJECT_NAME}Targets DESTINATION lib/cmake/${PROJECT_NAME})
install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        DESTINATION lib/cmake/${PROJECT_NAME})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)


#############
## Testing ##
#############
find_package(GTest)

if(GTEST_FOUND)

# define catkin compatible targets to build and run all tests
# - tests, tests_${PROJECT_NAME} and ${TEST_TARGET_NAME} targets compile the tests
# - run_tests, run_tests_${PROJECT_NAME} and test targets run the tests
#   (the latter is a CMake/CTest produce, while the others are set by the catkin_add_gtest macro)

if(NOT TARGET tests)
  add_custom_target(tests ${all})
endif()
add_dependencies(tests ${PROJECT_NAME})

if(NOT TARGET run_tests)
  add_custom_target(run_tests COMMAND make test)
  add_dependencies(run_tests tests)
  enable_testing()
endif()

add_custom_target(tests_${PROJECT_NAME})
add_dependencies(tests tests_${PROJECT_NAME})

if(NOT TARGET run_tests_${PROJECT_NAME})
  add_custom_target(run_tests_${PROJECT_NAME})
  add_dependencies(run_tests run_tests_${PROJECT_NAME})
endif()


# define catkin compatible targets to build and run each test in this repo

file(GLOB_RECURSE _tests "test/*.cpp" "test/*.cc")

  foreach(_test ${_tests})
    get_filename_component(_test_name ${_test} NAME_WE)
    # make sure we add only one -test to the target
    string(REGEX REPLACE "-test" "" TEST_TARGET_NAME ${_test_name})
    set(TEST_TARGET_NAME ${PROJECT_NAME}-gtest-${TEST_TARGET_NAME})

    message(STATUS
      "Adding gtest unittest \"${TEST_TARGET_NAME}\" with working dir ${PROJECT_SOURCE_DIR}/${TEST_FOLDER} \n _test: ${_test}"
    )

    add_executable(${TEST_TARGET_NAME} ${_test})

    set(TEST_RESULTS_DIR ${CMAKE_BINARY_DIR}/test_results)
    if(DEFINED CATKIN_TEST_RESULTS_DIR)
        # support overriding to be compatible with catkin
        set(TEST_RESULTS_DIR ${CATKIN_TEST_RESULTS_DIR})
    endif()
    set(RESULT_XML_PATH ${TEST_RESULTS_DIR}/${PROJECT_NAME}/gtest-${TEST_TARGET_NAME}.xml)
    add_test(NAME ${TEST_TARGET_NAME} COMMAND $<TARGET_FILE:${TEST_TARGET_NAME}> --gtest_color=yes --gtest_output=xml:${RESULT_XML_PATH})

    set_target_properties(${TEST_TARGET_NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    target_link_libraries(${TEST_TARGET_NAME} PUBLIC
        ${GTEST_BOTH_LIBRARIES} pthread
        ${PROJECT_NAME}
    )
    add_dependencies(tests_${PROJECT_NAME} ${TEST_TARGET_NAME})
  endforeach()
endif()
