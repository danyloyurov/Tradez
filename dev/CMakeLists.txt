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
# Find JSON
#-------------------------------------------------------------------------------
include_directories(${JsonCpp_INCLUDE_DIR})
list(APPEND LIBS ${JsonCpp_LIBRARY})

set(DIRS
common/api
common/utilities/include
trading/bot/include
trading_platforms/common
trading_platforms/kraken/include)

set(EXT_DIRS
external_lib/tgraphics)

set(SOURCES
main.cpp
common/utilities/source/multipurpose_converter.cpp
common/utilities/source/logger.cpp
common/utilities/source/drawers.cpp
common/utilities/source/trading_client_helper.cpp
trading/bot/source/trader.cpp
trading/bot/source/orders_handler.cpp
trading/bot/source/asset_pair_handler.cpp
trading/bot/source/trading_platform_observer.cpp
trading_platforms/kraken/source/kraken_client.cpp)

include_directories(${DIRS})
include_directories(${EXT_DIRS} -isystem)

#-------------------------------------------------------------------------------
# Add the executable 'Tradez'
#-------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${SOURCES})
#set_target_properties (${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_DEBUG "JSON_DEBUG;JSON_SAFE;JSON_ISO_STRICT")
target_link_libraries (${PROJECT_NAME} ${LIBS})

