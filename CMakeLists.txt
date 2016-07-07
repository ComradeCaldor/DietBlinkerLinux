cmake_minimum_required(VERSION 3.0)
project( DisplayImge )
find_package( OpenCV REQUIRED )
add_executable( DietBlinker DietBlinker.cpp )
target_link_libraries( DietBlinker ${OpenCV_LIBS} )

