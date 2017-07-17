cmake_minimum_required (VERSION 2.8)

set (PROJECT_NAME wxcvui)
project (${PROJECT_NAME})

set (OPENCV_LIBS
    -lopencv_videostab
    -lopencv_videoio
    -lopencv_video
    -lopencv_superres
    -lopencv_stitching
    -lopencv_shape
    -lopencv_photo
    -lopencv_objdetect
    -lopencv_ml
    -lopencv_imgproc
    -lopencv_imgcodecs
    -lopencv_highgui
    -lopencv_flann
    -lopencv_features2d
    -lopencv_core
    -lopencv_calib3d)

# Setup the THIRD_PREFIX variable.
string (REPLACE "\\" "/" __tparty_home_path "$ENV{TPARTY_HOME}/local")
if (IS_DIRECTORY "${__tparty_home_path}")
    list (INSERT CMAKE_PROGRAM_PATH 0 "${__tparty_home_path}/bin")
    list (INSERT CMAKE_INCLUDE_PATH 0 "${__tparty_home_path}/include")
    list (INSERT CMAKE_LIBRARY_PATH 0 "${__tparty_home_path}/lib")
    set (wxWidgets_ROOT_DIR "${__tparty_home_path}")
else ()
    message (STATUS "Not found tparty local directory: ${__tparty_home_path}")
endif ()

find_package (wxWidgets REQUIRED base core aui)
include_directories (${wxWidgets_INCLUDE_DIRS})
include_directories (${CMAKE_SOURCE_DIR})

foreach (__define_cursor ${wxWidgets_DEFINITIONS})
    add_definitions (-D${__define_cursor})
endforeach ()
add_definitions (-DUSE_OPENCV)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${wxWidgets_CXX_FLAGS}")

set (SOURCE_FILES
    main.cpp
    MainFrame.cpp
    MugcupPanel.cpp)

add_executable (${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries (${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES} ${OPENCV_LIBS})

