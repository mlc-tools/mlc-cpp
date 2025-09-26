cmake_minimum_required(VERSION 3.16)

set(APP_NAME mlc)
project(${APP_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Provide standard install directory variables like CMAKE_INSTALL_BINDIR
include(GNUInstallDirs)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mlc-cpp)
set(THIRD_PARTY ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
file(GLOB_RECURSE SOURCES 
    ${SRC_DIR}/*.h
    ${SRC_DIR}/*.hpp
    ${SRC_DIR}/*.cpp
    ${THIRD_PARTY}/*.h
    ${THIRD_PARTY}/*.hpp
    ${THIRD_PARTY}/*.cpp
    )
foreach(FILE ${SOURCES}) 
  get_filename_component(PARENT_DIR ${FILE} PATH)
  string(REPLACE "${CMAKE_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
  string(REPLACE "/" "\\" GROUP "${GROUP}")
  set(GROUP "${GROUP}")
  source_group("${GROUP}" FILES "${FILE}")
endforeach()


add_executable(${APP_NAME} ${SOURCES})

find_package(RE2 REQUIRED CONFIG)  # CONFIG на случай, если пакет установлен с экспортом targets
target_link_libraries(${APP_NAME} PRIVATE re2::re2)

target_include_directories(${APP_NAME} PRIVATE ${SRC_DIR})
target_include_directories(${APP_NAME} PRIVATE ${SRC_DIR}/models)
target_include_directories(${APP_NAME} PRIVATE ${SRC_DIR}/module_cpp)
target_include_directories(${APP_NAME} PRIVATE ${SRC_DIR}/parser)
target_include_directories(${APP_NAME} PRIVATE ${SRC_DIR}/tests)
target_include_directories(${APP_NAME} PRIVATE ${SRC_DIR}/utils)
target_include_directories(${APP_NAME} PRIVATE ${SRC_DIR}/core)
target_include_directories(${APP_NAME} PRIVATE ${THIRD_PARTY})

# Install the utility into the system binary directory
install(TARGETS ${APP_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Install macOS update helper alongside the binary as `mlc-update`
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/update_osx.sh)
  install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/update_osx.sh
          DESTINATION ${CMAKE_INSTALL_BINDIR}
          RENAME mlc-update)
endif()
