cmake_minimum_required( VERSION 2.8 )
project(PlayVideo )
find_package( OpenCV REQUIRED )
add_executable( PlayVideo PlayVideo.cpp )
target_link_libraries( PlayVideo ${OpenCV_LIBS} )

