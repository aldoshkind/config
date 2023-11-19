#pragma once

#include <adn/config/config_base.hpp>

namespace adn::config::examples
{

class config : public adn::config::config_base
{
    ADN_SINGLETON_CONSTR_AND_GETTER(config)

    ADN_VAR(bool, bool_value, true)
    ADN_VAR(std::string, string_value, "string value")
    ADN_VAR(int, integer_value, 5000)
    ADN_VAR(nlohmann::json, json_value, nlohmann::json::array())
};

}
