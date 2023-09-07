cmake_minimum_required(VERSION 3.22.1)

project("myapplication")

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ./../proto/jni.proto)

# jvm
find_package(Java REQUIRED)
# https://stackoverflow.com/questions/51047978/cmake-could-not-find-jni
set(JAVA_AWT_LIBRARY NotNeeded)
set(JAVA_JVM_LIBRARY NotNeeded)
find_package(JNI REQUIRED)


add_library(
        myapplication
        SHARED
        native-lib.cpp
        ${PROTO_SRCS}
        ${PROTO_HDRS}
)


target_link_libraries(myapplication ${Protobuf_LIBRARIES})

target_include_directories(
        myapplication
        PUBLIC
        ${JNI_INCLUDE_DIRS}
        ${PROTO_HDRS}

        ./../../../build/generated/sources/headers/java/main
)




add_executable(
        term
        native-lib.cpp
        term.cpp
        ${PROTO_SRCS}
        ${PROTO_HDRS}
)


target_link_libraries(term ${Protobuf_LIBRARIES})

target_include_directories(
        term
        PUBLIC
        ${JNI_INCLUDE_DIRS}
        ${PROTO_HDRS}

        ./../../../build/generated/sources/headers/java/main
)
