#pragma once

#define LOG_I dummy_logger()

class dummy_logger
{
public:
    template <class T>
    dummy_logger & operator << (const T &/*op*/)
    {
        // do nothing
        return *this;
    }
};
