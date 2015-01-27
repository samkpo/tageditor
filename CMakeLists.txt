project(tageditor)
include(FindPkgConfig)
cmake_minimum_required(VERSION 2.8)

#todos los archivos cpp
set(APP_SOURCES fileinfo.cpp tageditor.cpp main.cpp)

#todas las cabeceras que deben ser tratadas con moc
set(APP_MOC_HEADERS tageditor.h)

#los archivos ui
set(APP_UI tageditor.ui)

#busca y agrega qt
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

#busco y agrego taglib (en t_l_l(${TAGLIB_LIBRARIES}))
pkg_check_modules(TAGLIB REQUIRED taglib>=1.6)
link_directories(${TAGLIB_LIBRARY_DIRS})
include_directories(${TAGLIB_INCLUDE_DIRS})

#crea los archivos ui.h
QT4_WRAP_UI(UI_HEADERS ${APP_UI})

#crea los cpp de moc
QT4_WRAP_CPP(APP_MOC_SOURCES ${APP_MOC_HEADERS})

#hago que agrege las cabeceras y las fuentes generadas
include_directories(${CMAKE_BINARY_DIR})

add_executable(tageditor ${APP_SOURCES} ${UI_HEADERS} ${APP_MOC_SOURCES})
target_link_libraries(tageditor ${QT_LIBRARIES} ${TAGLIB_LIBRARIES})