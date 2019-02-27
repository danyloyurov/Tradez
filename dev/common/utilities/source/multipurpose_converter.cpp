#include "multipurpose_converter.hpp"

#include <sstream>

std::string MultipurposeConverter::ConvertFloatToString(const float& value) {
    std::ostringstream out;

    if (!(out << value))
      return "";

    return out.str();
}

std::string MultipurposeConverter::ConvertCurrencyToString(const trading::Currency& value) {
    std::string output;

    switch(value) {
        case trading::USD: {
            output = "USD";
            break;
        }
        case trading::EUR: {
            output = "EUR";
            break;
        }
        case trading::XBT: {
            output = "XBT";
            break;
        }
        default: {
            break;
        }
    }

    return output;
}
