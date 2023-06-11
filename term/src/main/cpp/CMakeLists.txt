# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.22.1)

project("myapplication")

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ./../proto/jni.proto)
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS EXPORT_MACRO DLL_EXPORT foo.proto)
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS DESCRIPTORS PROTO_DESCS foo.proto)
#protobuf_generate_python(PROTO_PY foo.proto)
#add_executable(bar bar.cc ${PROTO_SRCS} ${PROTO_HDRS})


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
        #        ./../../../build/generated/source/proto/main/cpp/jni.pb.cc
)


target_link_libraries(myapplication ${Protobuf_LIBRARIES})

target_include_directories(
        myapplication
        PUBLIC
        ${JNI_INCLUDE_DIRS}
        ${PROTO_HDRS}

        ./../../../build/generated/sources/headers/java/main
        #        ./../../../build/generated/source/proto/main/cpp
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
        #        ./../../../build/generated/source/proto/main/cpp
)
