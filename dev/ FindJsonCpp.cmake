find_path(JsonCpp_INCLUDE_DIR "json/json.h" PATH_SUFFIXES "jsoncpp")

find_library(JsonCpp_LIBRARY NAMES jsoncpp PATHS)
set(JsonCpp_INCLUDE_DIRS ${JsonCpp_INCLUDE_DIR})
set(JsonCpp_LIBRARIES "${JsonCpp_LIBRARY}")

set(_JsonCpp_version_args)
if (EXISTS "${JsonCpp_INCLUDE_DIR}/json/version.h")
  file(STRINGS "${JsonCpp_INCLUDE_DIR}/json/version.h" _JsonCpp_version_contents REGEX "JSONCPP_VERSION_[A-Z]+")
  foreach (_JsonCpp_version_part MAJOR MINOR PATCH)
    string(REGEX REPLACE ".*# *define +JSONCPP_VERSION_${_JsonCpp_version_part} +([0-9]+).*" "\\1" JsonCpp_VERSION_${_JsonCpp_version_part} "${_JsonCpp_version_contents}")
  endforeach ()

  set(JsonCpp_VERSION_STRING "${JsonCpp_VERSION_MAJOR}.${JsonCpp_VERSION_MINOR}.${JsonCpp_VERSION_PATCH}")

  set(_JsonCpp_version_args VERSION_VAR JsonCpp_VERSION_STRING)
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JsonCpp REQUIRED_VARS JsonCpp_LIBRARIES JsonCpp_INCLUDE_DIRS ${_JsonCpp_version_args})
mark_as_advanced(JsonCpp_INCLUDE_DIR JsonCpp_LIBRARY)
