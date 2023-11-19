#pragma once

#include <typeinfo>

#include <cxxabi.h>

#ifdef ADN_CONFIG_USE_DUMMY_LOGGER
#include "dummy_logger.hpp"
#endif

#include "var.hpp"

#define ADN_SINGLETON_GETTER(class_name)     public: static class_name &instance(){static class_name c; return c;}

#define ADN_SINGLETON_CONSTR_AND_GETTER_EXPLICIT_PREFIX(class_name, prefix)     private: class_name (): config_base(prefix){init();} \
                                                        ADN_SINGLETON_GETTER(class_name)

#define ADN_SINGLETON_CONSTR_AND_GETTER(class_name)     ADN_SINGLETON_CONSTR_AND_GETTER_EXPLICIT_PREFIX(class_name, adn::config::get_prefix(typeid(*this).name()))

namespace adn::config
{

static std::string capitalized(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), 
                   [](unsigned char c)
                   {
                       return std::toupper(c);
                   }
                  );
    return s;
}

static std::string get_prefix(const std::string &id)
{
    std::string prefix_var_name;
    
    int status = 0;
    char *demangled = abi::__cxa_demangle(id.c_str(), 0, 0, &status);
    
    char prev = 0;
    for(size_t i = 0 ; i < strlen(demangled) ; i += 1)
    {
        char c = demangled[i];
        if(isalnum(c))
        {
            prefix_var_name += c;
        }
        if(c == ':' and prev == ':')
        {
            prefix_var_name += '_';
        }
        prev = c;
    }
    free(demangled);
    
    prefix_var_name += "_prefix";
    prefix_var_name = capitalized(prefix_var_name);
    
    char *prefix_env_str = getenv(prefix_var_name.c_str());
    std::string prefix;
    if(prefix_env_str)
    {
        prefix = prefix_env_str;
    }
    
    return prefix;
}

class config_base
{
public:
    void dump()
    {
        for (auto &v : registry)
        {
            LOG_I << name(v) << ":\t" << v->serialized();
        }
    }

protected:
    registry_t registry;
    config_base(const std::string &pref): prefix(pref)
    {
        //
    }
    
    void init()
    {
        for (auto &v : registry)
        {
            char *val = getenv(name(v).c_str());
            if(val)
            {
                v->deserialize(val);
            }
        }

        //dump();
    }
    
    std::string prefix = "";

private:
    std::string name(const var_base *v) const
    {
        std::string res = prefix + capitalized(v->name());
        return res;
    }
};

}
