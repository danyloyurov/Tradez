#include "kraken.hpp"
#include "multipurpose_converter.hpp"
#include <regex>

KrakenPlatform::KrakenPlatform(const std::string& public_key, const std::string& private_key)
  : kraken_client_(public_key, private_key) {
  Kraken::initialize();
}

KrakenPlatform::~KrakenPlatform() {
  Kraken::terminate();
}

error::TradingError KrakenPlatform::PlaceOrder(trading::Order& order) {
  try {
    Kraken::KInput order_to_place;
    order_to_place["pair"] = order.asset_pair_;
    order_to_place["type"] = MultipurposeConverter::ConvertOrderTypeToKrakenType(order.type_);
    order_to_place["ordertype"] = "limit";

    std::string price = std::to_string(order.price_);
    int commaIdx = price.find(".");

    price = price.substr(0, commaIdx + order.price_presset_.right_side_symbols_count_);

    order_to_place["price"] = price;
    order_to_place["volume"] = std::to_string(order.volume_);
    order_to_place["oflags"] = "fcib,nompp";

    json_string response = libjson::to_json_string(kraken_client_.private_method("AddOrder", order_to_place));     
    JSONNode root = libjson::parse(response);

    if (false == root.at("error").empty()) return error::FAILED;
    if (true == root.at("result").empty()) return error::FAILED;

    JSONNode& result = root["result"];

    std::string ID = libjson::to_std_string(response);

    int lindex = ID.find("txid") + 8;
    ID = ID.substr(lindex, ID.size() - 1);
    int rindex = ID.find("\"");
    ID = ID.substr(0, rindex);

    order.trading_patform_ID_ = ID;
    return error::SUCCESS;
  } catch(...) {
    return error::FAILED;
  }
}

error::TradingError KrakenPlatform::RemoveOrder(const trading::id_t& order_id) {
  try {
    Kraken::KInput remove;
    remove["txid"] = order_id;

    json_string response = libjson::to_json_string(kraken_client_.private_method("CancelOrder", remove)); 
    JSONNode root = libjson::parse(response);

    if (false == root.at("error").empty()) return error::FAILED;
    if (true == root.at("result").empty()) return error::FAILED;

    JSONNode &result = root["result"];
    std::string count = libjson::to_std_string( result.at("count").as_string() );

    bool is_removed = atoi(count.c_str());

    if (false == is_removed) return error::FAILED;

    return error::SUCCESS;
  } catch(...) {
    return error::FAILED;
  }
}

error::TradingError KrakenPlatform::GetAssetPairs(std::vector<trading::asset_pair_t>& asset_pairs) {
  try {
    Kraken::KInput pairs;

    std::string data = kraken_client_.public_method("AssetPairs", pairs);

    for(int i = 0; i < data.size(); i++){
      if(std::string::npos == data.find("altname")) break;

      std::string result = data.substr(data.find("altname") + 10, 10);

      int slide_start_index = data.find("altname") + 10 + ( result.find_last_of(',') - 1);

      data = data.substr(slide_start_index, data.length());
      result = result.substr(0, result.find_last_of(',') - 1);

      if(std::string::npos == result.find(".d")) {
        asset_pairs.push_back(result);
      }
    }

    if(true == asset_pairs.empty()) return error::FAILED;

    return error::SUCCESS;
  } catch(...) {
    return error::FAILED;
  }
}

error::TradingError KrakenPlatform::GetAssetRawDump(const trading::common::asset_pair_t& asset_pair,
                                                    std::vector<trading::analyzer::RawAsset>& raw_asset_dump) {
  try {
    raw_asset_dump.clear();
    kraken_client_.trades(asset_pair, "current", raw_asset_dump);
    return error::SUCCESS;
  } catch(...) {
    return error::FAILED;
  }
}

error::TradingError KrakenPlatform::GetClosedOrders(std::vector<trading::id_t>& closed_orders) {
  try {
    Kraken::KInput input_closed_orders;
    input_closed_orders["ofs"] = "0";
    json_string response = libjson::to_json_string(kraken_client_.private_method("ClosedOrders", input_closed_orders)); 
    JSONNode root = libjson::parse(response);

    if (false == root.at("error").empty()) return error::FAILED;
    if (true == root.at("result").empty()) return error::FAILED;

    std::smatch match;
    std::regex ebat ("[0-9A-Z]{6}[-]{1}[0-9A-Z]{5}[-]{1}[0-9A-Z]{6}");

    while(std::regex_search (response, match, ebat)) {
      for(auto& closed_orded : match) {
        closed_orders.push_back(closed_orded);
      }

      response = match.suffix().str();
    }

    return error::SUCCESS;
  } catch(...) {
     return error::FAILED;
  }
}

// error::TradingError KrakenPlatform::GetMargin(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& currency_margin) {
//   try {
//     pair_dump_.clear();
//     kraken_client_.trades(pair, "current", pair_dump_);

//     time_t curtime;
//     time(&curtime);

//     trading::price_t low = 1000000;
//     trading::price_t high = 0;
//     for(auto& item : pair_dump_) {

//       if((curtime - item.time) < time_period) {

//         if(item.price < 0.0001) {
//           currency_margin = -100;
//           break;
//         }

//         if(low > item.price) {
//           low = item.price;
//         }

//         if(high < item.price) {
//           high = item.price;
//         }

//       }

//     }

//     currency_margin = ( (high - low) / (low / 100) );

//     return error::SUCCESS;
//   } catch(...) {
//     return error::FAILED;
//   }
// }

// error::TradingError KrakenPlatform::GetAveragePrice(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& price) {
//   try {
//     pair_dump_.clear();
//     kraken_client_.trades(pair, "current", pair_dump_);

//     time_t curtime;
//     time(&curtime);

//     trading::price_t low = 1000000;
//     trading::price_t high = 0;
//     for(auto& item : pair_dump_) {

//       if((curtime - item.time) < time_period) {

//         if(low > item.price) {
//           low = item.price;
//         }

//         if(high < item.price) {
//           high = item.price;
//         }

//       }
//     }

//     price = (high + low) / 2;

//     return error::SUCCESS;
//   } catch(...) {
//     return error::FAILED;
//   }
// }

// error::TradingError KrakenPlatform::GetHighestPrice(const trading::asset_pair_t& pair, const int& time_period, trading::price_t& price) {
//   try {
//     pair_dump_.clear();
//     kraken_client_.trades(pair, "current", pair_dump_);

//     time_t curtime;
//     time(&curtime);

//     trading::price_t high = 0;
//     for(auto& item : pair_dump_) {

//       if((curtime - item.time) < time_period) {

//         if(item.price > high) {
//           high = item.price;
//         }

//       }
//     }

//     price = high;

//     return error::SUCCESS;
//   }
//   catch(...) {
//     return error::FAILED;
//   }
// }

// error::TradingError KrakenPlatform::GetPairPriceFormat(const trading::asset_pair_t& pair, trading::PricePresset& presset) {
//   try {
//     pair_dump_.clear();
//     kraken_client_.trades(pair, "current", pair_dump_);

//     time_t curtime;
//     time(&curtime);

//     int before_comma_max_count = 0;
//     int after_comma_max_count = 0;
//     std::string price;

//     for(auto& item : pair_dump_) {
//       price = MultipurposeConverter::ConvertFloatToString(item.price);

//       int comma_index = price.find(".");

//       if(std::string::npos == comma_index || price == "") continue;

//       if(comma_index > before_comma_max_count) {
//         before_comma_max_count = comma_index;
//       }

//       if((price.length() - 1) - comma_index > after_comma_max_count) {
//         after_comma_max_count = (price.length() - 1) - comma_index;
//       }
//     }

//     presset.left_side_symbols_count_ = before_comma_max_count;
//     presset.right_side_symbols_count_ = after_comma_max_count;

//     return error::SUCCESS;
//   } catch(...) {
//     return error::FAILED;
//   }
// }

// error::TradingError KrakenPlatform::GetVolumeToBuy(const trading::asset_pair_t& pair, const trading::price_t& base_volume, trading::volume_t& crypto_volume) {
//   try {
//     pair_dump_.clear();
//     kraken_client_.trades(pair, "last", pair_dump_);

//     if(true == pair_dump_.empty()) return error::FAILED;

//     trading::price_t price = pair_dump_[pair_dump_.size() - 1].price;

//     double mult = 1 / price;
//     crypto_volume = mult * base_volume;

//     return error::SUCCESS;
//   } catch(...) {
//     return error::FAILED;
//   }
// }
