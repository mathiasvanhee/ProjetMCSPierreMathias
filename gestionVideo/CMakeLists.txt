cmake_minimum_required(VERSION 2.8)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_library( gestionVideo gestionVideo.cxx )
target_link_libraries( gestionVideo PUBLIC ${OpenCV_LIBS} )

target_include_directories(gestionVideo
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(gestionVideo PRIVATE Threads::Threads)