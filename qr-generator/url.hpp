#pragma once
#include <string>

namespace sellekna {
  class UrlBuilder {
  public:
    static std::string createPaymentUrl(
      const std::string& merchantId,
      double amount,
      const std::string& description
    ) {
      std::string url = "https://sellekna.com/pay?";
      url += "merchant=" + encodeParam(merchantId);
      url += "&amount=" + std::to_string(amount);
      url += "&desc=" + encodeParam(description);
      return url;
    }
    
  private:
    static std::string encodeParam(const std::string& param) {
      // Basic URL encoding
      std::string encoded;
      for (char c : param) {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
          encoded += c;
        } else {
          encoded += '%' + toHex(c);
        }
      }
      return encoded;
    }
    
    static std::string toHex(char c) {
      static const char hex[] = "0123456789ABCDEF";
      std::string result;
      result += hex[(c >> 4) & 0xF];
      result += hex[c & 0xF];
      return result;
    }
  };
}