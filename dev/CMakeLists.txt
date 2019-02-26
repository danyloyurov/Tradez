cmake_minimum_required(VERSION 2.6)
project(Tradez)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pthread")

#-------------------------------------------------------------------------------
# Find OpenSSL
#-------------------------------------------------------------------------------
find_package (OpenSSL REQUIRED)
include_directories (${OPENSSL_INCLUDE_DIR})
list(APPEND LIBS ${OPENSSL_LIBRARIES}) 

#-------------------------------------------------------------------------------
# Find CURL
#-------------------------------------------------------------------------------
find_package (CURL REQUIRED)
include_directories (${CURL_INCLUDE_DIR})
list(APPEND LIBS ${CURL_LIBRARIES})

#-------------------------------------------------------------------------------
# Add library kapi (krakenapi)
#-------------------------------------------------------------------------------
set(KAPI_SOURCE_DIR external_lib/kraken/)
aux_source_directory(${KAPI_SOURCE_DIR} KAPI_SOURCE_FILES)
add_library (kapi STATIC ${KAPI_SOURCE_FILES})

# set some properties when CMAKE_BUILD_TYPE is "Debug"
set_target_properties (kapi PROPERTIES COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT" DEBUG_POSTFIX "d")

list (INSERT LIBS 0 kapi)

#-------------------------------------------------------------------------------
# Add library libjson
#-------------------------------------------------------------------------------
set (LIBJSON_SOURCE_DIR external_lib/kraken/libjson/_internal/Source)
aux_source_directory (${LIBJSON_SOURCE_DIR} LIBJSON_SOURCE_FILES)
add_library (libjson STATIC ${LIBJSON_SOURCE_FILES})

# remove "lib" prefix from the name of the libjson archive
set_target_properties (libjson PROPERTIES PREFIX "")

# set some properties when CMAKE_BUILD_TYPE is "Debug"
set_target_properties (libjson PROPERTIES COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT" DEBUG_POSTFIX "d")

list (APPEND LIBS libjson)

set(DIRS
common/api
common/utilities/include
external_lib/kraken/
external_lib/krakenlibjson/
trading/bot/include/
trading_platforms/common
trading_platforms/kraken/include)

set(SOURCES
main.cpp
common/utilities/source/multipurpose_converter.cpp
trading/bot/source/trader.cpp
trading/bot/source/trading_platform_observer.cpp
trading_platforms/kraken/source/kraken.cpp)

include_directories(${DIRS})

#-------------------------------------------------------------------------------
# Add the executable 'Tradez'
#-------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties (${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT")
target_link_libraries (${PROJECT_NAME} ${LIBS})
