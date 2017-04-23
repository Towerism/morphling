#pragma once

#include <vector>
#include <string>

namespace Morphling::Gamelogic {
  class Model_to_strings_mapper {
  public:
    typedef std::vector<std::string> strings_t;
    virtual ~Model_to_strings_mapper() = default;

    virtual strings_t get_strings() = 0;
  };
}
