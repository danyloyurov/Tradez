#include "multipurpose_converter.hpp"

#include <sstream>

std::string MultipurposeConverter::ConvertFloatToString(const float& value) {
    std::ostringstream out;

    if (!(out << value))
      return "";

    return out.str();
}
