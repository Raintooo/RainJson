#ifndef JSONCODEC_H
#define JSONCODEC_H

#include "Json.h"

namespace RainJson
{

class JCodec
{

public:
    static bool encode(const Json& json, std::string& encRet);
    static Json parser(const std::string& data);
};

}

#endif