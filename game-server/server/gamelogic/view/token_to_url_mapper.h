#pragma once

#include <unordered_map>
#include <string>
#include <sstream>

namespace Morphling::Gamelogic {
  class Token_to_url_mapper
  {
  public:
    void add_url(char token, std::string url);
    std::string get_url(char token) const;

    class Token_not_mapped : public std::runtime_error {
    public:
      Token_not_mapped(char token) : runtime_error(str(token)) {}
      static std::string str(char token) {
        std::ostringstream error;
        error << "Token_to_url_mapper: No URL mapped for token '" << token << "'";
        return error.str();
      }
    };

    class Token_already_mapped : public std::runtime_error {
    public:
      Token_already_mapped(char token) : runtime_error(str(token)) {}
      static std::string str(char token) {
        std::ostringstream error;
        error << "Token_to_url_mapper: Token '" << token << "' already mapped";
        return error.str();
      }
    };

  private:
    std::unordered_map<char, std::string> token_to_url_map;
  };

}
