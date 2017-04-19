#include "token_to_url_mapper.h"

#include <sstream>

namespace Morphling::Gamelogic {
  void Token_to_url_mapper::add_url(char token, std::string url) {
    if (token_to_url_map.find(token) != token_to_url_map.end())
      throw Token_already_mapped(token);
    token_to_url_map.insert({ token, url });
  }

  std::string Token_to_url_mapper::get_url(char token) const {
    try {
      return token_to_url_map.at(token);
    } catch(...) {
      throw Token_not_mapped(token);
    }
  }
}
