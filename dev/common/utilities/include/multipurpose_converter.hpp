#ifndef UTILITIES_MULTIPURPOSE_CONVERTER_HPP_
#define UTILITIES_MULTIPURPOSE_CONVERTER_HPP_

#include <string>
#include "trading_interface.hpp"

class MultipurposeConverter {
    public:
        static std::string ConvertFloatToString(const float& value);
        static std::string ConvertCurrencyToString(const trading::Currency& value);
        static std::string ConvertOrderTypeToKrakenType(const trading::OrderType& type);
        static time_t ConvertBittrexTimeToSeconds(std::string& time_to_parse);
};

#endif // UTILITIES_MULTIPURPOSE_CONVERTER_HPP_
