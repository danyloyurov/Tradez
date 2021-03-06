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
target_compile_options(kapi PRIVATE -w)
set_target_properties (kapi PROPERTIES COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT" DEBUG_POSTFIX "d")
list (INSERT LIBS 0 kapi)

#-------------------------------------------------------------------------------
# Add library libjson
#-------------------------------------------------------------------------------
set (LIBJSON_SOURCE_DIR external_lib/kraken/libjson/_internal/Source)
aux_source_directory (${LIBJSON_SOURCE_DIR} LIBJSON_SOURCE_FILES)
add_library (json STATIC ${LIBJSON_SOURCE_FILES})
target_compile_options(json PRIVATE -w)
set_target_properties (json PROPERTIES COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT" DEBUG_POSTFIX "d")
list (APPEND LIBS json)

set(DIRS
common/api
common/utilities/include
trading/bot/include
trading_platforms/common
trading_platforms/kraken/include)

set(EXT_DIRS
external_lib/kraken/
external_lib/kraken/libjson
external_lib/kraken/libjson/_internal
external_lib/kraken/libjson/_internal/Source
external_lib/kraken/libjson/_internal/Source/JSONDefs
external_lib/kraken/libjson/_internal/Dependencies/libbase64++
external_lib/kraken/libjson/_internal/Dependencies/mempool++
external_lib/tgraphics)

set(SOURCES
main.cpp
common/utilities/source/multipurpose_converter.cpp
common/utilities/source/logger.cpp
common/utilities/source/drawers.cpp
trading/bot/source/trader.cpp
trading/bot/source/orders_handler.cpp
trading/bot/source/asset_pair_handler.cpp
trading/bot/source/trading_platform_observer.cpp
trading_platforms/kraken/source/kraken.cpp)

include_directories(${DIRS})
include_directories(${EXT_DIRS} -isystem)

#-------------------------------------------------------------------------------
# Add the executable 'Tradez'
#-------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties (${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT")
target_link_libraries (${PROJECT_NAME} ${LIBS})

