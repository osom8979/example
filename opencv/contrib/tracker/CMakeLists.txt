cmake_minimum_required (VERSION 2.8)

set (PROJECT_NAME main)
project (${PROJECT_NAME})

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set (SOURCE_FILES main.cpp)
add_executable (${PROJECT_NAME} ${SOURCE_FILES})

set (OPENCV_LIBS
    -lopencv_xphoto
    -lopencv_xobjdetect
    -lopencv_ximgproc
    -lopencv_xfeatures2d
    -lopencv_tracking
    -lopencv_text
    -lopencv_surface_matching
    -lopencv_structured_light
    -lopencv_stereo
    -lopencv_sfm
    -lopencv_saliency
    -lopencv_rgbd
    -lopencv_reg
    -lopencv_plot
    -lopencv_optflow
    -lopencv_line_descriptor
    -lopencv_hdf
    -lopencv_fuzzy
    -lopencv_face
    -lopencv_dpm
    -lopencv_dnn
    -lopencv_datasets
    -lopencv_ccalib
    -lopencv_bioinspired
    -lopencv_bgsegm
    -lopencv_aruco
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

#add_dependencies (${PROJECT_NAME} dep-target)
target_compile_definitions (${PROJECT_NAME} PRIVATE -DUSE_OPENCV)
target_include_directories (${PROJECT_NAME} PRIVATE $ENV{TPARTY_HOME}/local/include)
target_compile_options (${PROJECT_NAME} PRIVATE -std=c++11)
target_link_libraries (${PROJECT_NAME} PRIVATE -L$ENV{TPARTY_HOME}/local/lib ${OPENCV_LIBS})

