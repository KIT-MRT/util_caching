cmake_minimum_required(VERSION 3.22)


######################
## Project settings ##
######################

# We support building this as top-level project, e.g. in order to test the lib installation
project(util_caching_tests
  LANGUAGES CXX
)


###############
## C++ setup ##
###############

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  # Require C++17
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)

  # Let's ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_CXX_EXTENSIONS OFF)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Allow clangd and others to properly understand this C++ project
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  # Testing only available if this is the main project
  # Note this needs to be done in the main CMakeLists
  # since it calls enable_testing, which must be in the
  # main CMakeLists.
  include(CTest)
endif()


###################
## Find packages ##
###################

find_package(GTest)
find_package(pybind11 CONFIG)

if(NOT GTEST_FOUND AND NOT pybind11_FOUND)
  message(WARNING "Neither GTest nor pybind11 found. Cannot compile tests!")
endif()

# Find installed lib and its dependencies, if this is build as top-level project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  find_package(util_caching REQUIRED)
endif()


####################
## C++ Unit Tests ##
####################

if(GTEST_FOUND)
  file(GLOB_RECURSE _tests CONFIGURE_DEPENDS "*.cpp" "*.cc")
  list(FILTER _tests EXCLUDE REGEX "${CMAKE_CURRENT_BINARY_DIR}")
  list(REMOVE_ITEM _tests "${CMAKE_CURRENT_SOURCE_DIR}/python_bindings.cpp")

  foreach(_test ${_tests})
    get_filename_component(_test_name ${_test} NAME_WE)
    # make sure we add only one -test to the target
    string(REGEX REPLACE "-test" "" TEST_TARGET_NAME ${_test_name})
    set(TEST_TARGET_NAME util_caching-gtest-${TEST_TARGET_NAME})

    message(STATUS
      "Adding gtest unittest \"${TEST_TARGET_NAME}\" with working dir ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_FOLDER} \n _test: ${_test}"
    )

    add_executable(${TEST_TARGET_NAME} ${_test})

    target_link_libraries(${TEST_TARGET_NAME} PUBLIC
      ${GTEST_BOTH_LIBRARIES} pthread
      util_caching
    )

    add_test(NAME ${TEST_TARGET_NAME}
      COMMAND ${TEST_TARGET_NAME}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
  endforeach()
endif()

#######################
## Python Unit Tests ##
#######################

if(pybind11_FOUND)
  # Find Python3 to run tests via ctest
  find_package(Python3 REQUIRED)

  # Python bindings modules
  pybind11_add_module(util_caching_py
    python_bindings.cpp
  )
  target_link_libraries(util_caching_py PUBLIC
    util_caching
  )

  file(GLOB_RECURSE _py_tests CONFIGURE_DEPENDS "*.py")

  # Copy Python test files to build directory
  foreach(_py_test ${_py_tests})
    get_filename_component(_py_test_name ${_py_test} NAME)
    string(REGEX REPLACE "-test" "" PY_TEST_NAME ${_py_test_name})
    set(PY_TEST_NAME ${PROJECT_NAME}-pytest-${PY_TEST_NAME})

    message(STATUS
      "Adding python unittest \"${PY_TEST_NAME}\" with working dir ${PROJECT_SOURCE_DIR}/${TEST_FOLDER} \n _test: ${_py_test}"
    )

    configure_file(${_py_test} ${PY_TEST_NAME} COPYONLY)

    add_test(NAME ${PY_TEST_NAME}
      COMMAND ${Python3_EXECUTABLE} ${PY_TEST_NAME}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  endforeach()
endif()

