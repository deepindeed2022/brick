CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12 FATAL_ERROR)

# ---[ Project
PROJECT(libsfordev C CXX)

# ---[ Options.
SET(LIBSFORDEV_LIBRARY_TYPE "default" CACHE STRING "Type of library (shared, static, or default) to build")
SET_PROPERTY(CACHE LIBSFORDEV_LIBRARY_TYPE PROPERTY STRINGS default static shared)
OPTION(LIBSFORDEV_BUILD_TESTS "Build libsfordev unit tests" ON)
# OPTION(LIBSFORDEV_DOWNLOAD_LIBS "Build libsfordev unit tests" OFF)

# ---[ CMake options
IF(LIBSFORDEV_BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

MACRO(LIBSFORDEV_TARGET_ENABLE_C99 target)
    IF(${CMAKE_VERSION} VERSION_LESS "3.1")
        IF(NOT MSVC)
            TARGET_COMPILE_OPTIONS(${target} PRIVATE -std=c99)
        ENDIF()
    ELSE()
        SET_TARGET_PROPERTIES(${target} PROPERTIES
            C_STANDARD 99
            C_EXTENSIONS NO)
        ENDIF()
ENDMACRO()

# ---[ Download deps
SET(CONFU_DEPENDENCIES_SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps 
    CACHE PATH "Confu-style dependencies source directory")

SET(CONFU_DEPENDENCIES_BINARY_DIR ${CMAKE_BINARY_DIR}/deps
    CACHE PATH "Confu-style dependencies binary directory")

IF(LIBSFORDEV_BUILD_TESTS AND NOT DEFINED GOOGLETEST_SOURCE_DIR)
    MESSAGE(STATUS "Downloading Google Test to ${CONFU_DEPENDENCIES_SOURCE_DIR}/googletest (define GOOGLETEST_SOURCE_DIR to avoid it)")
    CONFIGURE_FILE(cmake/DownloadGoogleTest.cmake "${CONFU_DEPENDENCIES_BINARY_DIR}/googletest-download/CMakeLists.txt")
    
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/googletest-download")
    
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/googletest-download")
    
    SET(GOOGLETEST_SOURCE_DIR "${CONFU_DEPENDENCIES_SOURCE_DIR}/googletest" CACHE STRING "Google Test source directory")
ENDIF() 


IF(LIBSFORDEV_BUILD_TESTS AND NOT DEFINED ZLIB_SOURCE_DIR)
    MESSAGE(STATUS "Downloading Zlib to ${CONFU_DEPENDENCIES_SOURCE_DIR}/zlib (define ZLIB_SOURCE_DIR to avoid it)")
    CONFIGURE_FILE(cmake/DownloadZlib.cmake "${CONFU_DEPENDENCIES_BINARY_DIR}/zlib-download/CMakeLists.txt")
    
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/zlib-download")
    
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/zlib-download")
    
    SET(ZLIB_SOURCE_DIR "${CONFU_DEPENDENCIES_SOURCE_DIR}/zlib" CACHE STRING "Zlib source directory")
ENDIF() 

IF(LIBSFORDEV_BUILD_TESTS AND NOT DEFINED PROTOBUF_SOURCE_DIR)
    MESSAGE(STATUS "Downloading Protobuf to ${CONFU_DEPENDENCIES_SOURCE_DIR}/protobuf (define PROTOBUF_SOURCE_DIR to avoid it)")
    CONFIGURE_FILE(cmake/DownloadProtobuf.cmake "${CONFU_DEPENDENCIES_BINARY_DIR}/protobuf-download/CMakeLists.txt")
    
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/protobuf-download")
    
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/protobuf-download")
    
    SET(PROTOBUF_SOURCE_DIR "${CONFU_DEPENDENCIES_SOURCE_DIR}/protobuf" CACHE STRING "Protobuf source directory")
ENDIF()

IF(LIBSFORDEV_BUILD_TESTS AND NOT DEFINED BEARSSL_SOURCE_DIR)
    MESSAGE(STATUS "Downloading Bearssl to ${CONFU_DEPENDENCIES_SOURCE_DIR}/bearssl (define BEARSSL_SOURCE_DIR to avoid it)")
    CONFIGURE_FILE(cmake/DownloadBearssl.cmake "${CONFU_DEPENDENCIES_BINARY_DIR}/bearssl-download/CMakeLists.txt")
    
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/bearssl-download")
    
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/bearssl-download")
    
    SET(BEARSSL_SOURCE_DIR "${CONFU_DEPENDENCIES_SOURCE_DIR}/bearssl" CACHE STRING "Bearssl source directory")
ENDIF()

IF(LIBSFORDEV_BUILD_TESTS AND NOT DEFINED CURL_SOURCE_DIR)
    MESSAGE(STATUS "Downloading Curl to ${CONFU_DEPENDENCIES_SOURCE_DIR}/curl (define CURL_SOURCE_DIR to avoid it)")
    CONFIGURE_FILE(cmake/DownloadCurl.cmake "${CONFU_DEPENDENCIES_BINARY_DIR}/curl-download/CMakeLists.txt")
    
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/curl-download")
    
    EXECUTE_PROCESS(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CONFU_DEPENDENCIES_BINARY_DIR}/curl-download")
    
    SET(CURL_SOURCE_DIR "${CONFU_DEPENDENCIES_SOURCE_DIR}/curl" CACHE STRING "Curl source directory")
ENDIF()

# test zlib
INCLUDE_DIRECTORIES(${ZLIB_SOURCE_DIR})
ADD_EXECUTABLE(test_sample_zlib ${CMAKE_SOURCE_DIR}/samples/sample_zlib.cpp)
TARGET_LINK_LIBRARIES(test_sample_zlib z)

ADD_EXECUTABLE(test_sample_protobuf
${CMAKE_SOURCE_DIR}/samples/sample_protobuf.cpp
${CMAKE_SOURCE_DIR}/samples/protobuf/addressbook.pb.cc)
TARGET_LINK_LIBRARIES(test_sample_protobuf protobuf)

ADD_EXECUTABLE(test_sample_curl ${CMAKE_SOURCE_DIR}/samples/sample_curl.cpp)
TARGET_LINK_LIBRARIES(test_sample_curl curl pthread)

ADD_EXECUTABLE(test_sample_gflag ${CMAKE_SOURCE_DIR}/samples/sample_gflag.cpp)
TARGET_LINK_LIBRARIES(test_sample_gflag gflags glog pthread)