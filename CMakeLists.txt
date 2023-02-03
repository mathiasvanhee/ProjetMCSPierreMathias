cmake_minimum_required(VERSION 2.8)
project( Tuiitche )

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE CFILES "${CMAKE_SOURCE_DIR}/*.c")
SET_SOURCE_FILES_PROPERTIES(${CFILES} PROPERTIES LANGUAGE CXX )

add_executable( client.exe application/client.c )
add_executable( serveurPrincipal.exe application/serveurPrincipal.c )

list(APPEND EXTRA_LIBS gestionVideo)
add_subdirectory(gestionVideo)

add_library( data  data/data.c)

add_library( protocole  protocole/protocole.c)
add_library( session  session/session.c)



find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(Threads REQUIRED)


target_link_libraries( client.exe PUBLIC ${OpenCV_LIBS} data ${EXTRA_LIBS} protocole session )
target_link_libraries( serveurPrincipal.exe PUBLIC ${OpenCV_LIBS} data ${EXTRA_LIBS} protocole session )
target_link_libraries(client.exe PRIVATE Threads::Threads)
target_link_libraries(serveurPrincipal.exe PRIVATE Threads::Threads)
target_link_libraries(client.exe PUBLIC ${CURSES_LIBRARIES})


set(THREADS_PREFER_PTHREAD_FLAG ON)
set(NCURSES ON)



