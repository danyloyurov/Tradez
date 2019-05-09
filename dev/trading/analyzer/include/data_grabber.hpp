#ifndef DATA_GRABBER_HPP_
#define DATA_GRABBER_HPP_

#include <memory>
#include <vector>

#include "error_interface.hpp"
#include "trading_platform.hpp"

class DataGrabber {
  public:
    DataGrabber(std::shared_ptr<ITradingPlatform> trading_platform);

    error::TradingError GetAssetData(const trading::common::asset_pair_t& asset_pair, std::vector<trading::analyzer::Sector>& asset_data);
  private:
    std::shared_ptr<ITradingPlatform> trading_platform_;
};

#endif // DATA_GRABBER_HPP_
