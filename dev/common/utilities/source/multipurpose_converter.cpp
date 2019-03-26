#include "multipurpose_converter.hpp"

#include <sstream>
#include <regex>
#include <string>

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

std::string MultipurposeConverter::ConvertOrderTypeToKrakenType(const trading::OrderType& type) {
    std::string output;

    switch (type) {
        case trading::BUY: {
            output = "buy";
            break;
        }
        case trading::SELL: {
            output = "sell";
            break;
        }
        default: {
            break;
        }
    }

    return output;
}

time_t MultipurposeConverter::ConvertBittrexTimeToSeconds(std::string& time_to_parse) {
    std::string year;
    std::string month_day;
    std::string hours_minutes_seconds;

    std::smatch match_year;
    std::smatch match_month_day;
    std::smatch match_hours_minutes_seconds;

    std::regex reg_year_ ("[0-9]{4}");
    std::regex reg_month_day_ ("[0-9]{2}[-]{1}[0-9]{2}");
    std::regex reg_hours_minutes_seconds_ ("[0-9]{2}[:]{1}[0-9]{2}[:]{1}[0-9]{2}");

    while(std::regex_search (time_to_parse, match_year, reg_year_)) {
        for(auto& a : match_year) year = a;
        time_to_parse = match_year.suffix().str();
    }
    while(std::regex_search (time_to_parse, match_month_day, reg_month_day_)) {
        for(auto& a : match_month_day) month_day = a;
        time_to_parse = match_month_day.suffix().str();
    }
    while(std::regex_search (time_to_parse, match_hours_minutes_seconds, reg_hours_minutes_seconds_)) {
        for(auto& a : match_hours_minutes_seconds) hours_minutes_seconds = a;
        time_to_parse = match_hours_minutes_seconds.suffix().str();
    }

    time_t timer = time(NULL);
    struct tm *timeinfo = localtime(&timer);

    timeinfo->tm_year = stoi(year) - 1900;
    timeinfo->tm_mon = stoi(month_day.substr(0,2))-1;
    timeinfo->tm_mday = stoi(month_day.substr(3,2));
    timeinfo->tm_hour = stoi(hours_minutes_seconds.substr(0,2));
    timeinfo->tm_min = stoi(hours_minutes_seconds.substr(3,2));
    timeinfo->tm_sec = stoi(hours_minutes_seconds.substr(6,2));

    timer = mktime(timeinfo);
    return timer;
}