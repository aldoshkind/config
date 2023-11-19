#pragma once

#include <string>
#include <list>

#include <nlohmann/json.hpp>

#define ADN_VAR_DEF(type, name)\
    adn::config::var<type> name{#name, type(), registry};

#define ADN_VAR_NO_DEF(type, name, defautl_value)\
    adn::config::var<type> name{#name, defautl_value, registry};

// this macros allows to use ADN_VAR with two or three arguments
#define ADN_GET_MACRO(_1,_2,_3,NAME,...) NAME
#define ADN_VAR(...) ADN_GET_MACRO(__VA_ARGS__, ADN_VAR_NO_DEF, ADN_VAR_DEF)(__VA_ARGS__)

namespace adn::config
{

template <class type>
inline type deserialize_impl(const std::string &s)
{
    return nlohmann::json::parse(s).get<type>();
}

template <>
inline std::string deserialize_impl<std::string>(const std::string &s)
{
    return s;
}


class var_base;

typedef std::list<var_base *> registry_t;

class var_base
{
public:
    var_base(const std::string &name, registry_t &registry) : name_(name)
    {
        registry.push_back(this);
    }
    virtual ~var_base(){}
    
    std::string name() const {return name_;}
    
    virtual std::string serialized() const = 0;
    virtual void deserialize(const std::string &s) = 0;
    
private:
    const std::string name_;
};

template <class T>
class var : public var_base
{
public:
    var(const std::string name, registry_t &registry) : var_base(name, registry), value(T()) {}
    var(const std::string name, T default_value, registry_t &registry) : var_base(name, registry), value(default_value) {}
    
    std::string serialized() const
    {
        return nlohmann::json(value).dump(1);
    }
    void deserialize(const std::string &s)
    {
        value = deserialize_impl<T>(s);
    }
    
    T operator()() const
    {
        return value;
    }
private:
    T value;
};


}
