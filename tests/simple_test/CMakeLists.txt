cmake_minimum_required(VERSION 3.6.2)

set(APP_NAME mlc_cpp_test)

SET(ROOT ${CMAKE_SOURCE_DIR})
SET(GEN_SRC ${CMAKE_SOURCE_DIR}/gen)
SET(THIRD_SRC ${CMAKE_SOURCE_DIR}/../third_party)

project(${APP_NAME})

file(GLOB_RECURSE SRC 
    ${ROOT}/common/*.cpp *.h
    ${GEN_SRC}/*.cpp
    ${GEN_SRC}/*.h
    ${THIRD_SRC}/*.cpp
    ${THIRD_SRC}/*.h
    ${THIRD_SRC}/*.hpp
    )

add_definitions(-DXML=1)
add_definitions(-DJSON=2)
add_definitions(-DCLIENT=1)
add_definitions(-DSERVER=2)
add_definitions(-Wall -std=c++17 -DTESTS -DSERIALIZE_FORMAT=XML -DBUILD_SIDE=CLIENT)

include_directories(
    ${GEN_SRC}
    ${THIRD_SRC}
)

include_directories(
    ${ROOT}/generated_cpp/${TYPE}/
    ${ROOT}/common/
    ${ROOT}/external/
    ${ROOT}/external/ml
    ${THIRD_PARTY_DIR}
)

add_executable(${APP_NAME} ${SRC})
