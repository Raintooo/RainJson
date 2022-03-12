#include "JsonValue.h"

namespace RainJson
{

static Json& nul_val()
{
    static Json nul;
    return nul;
}

int JValue::num_value() const
{
    return 0;
}

bool JValue::bool_value() const
{
    return false;
}

double JValue::double_value() const
{
    return 0.0f;
}

// ====================================================

std::string JValue::print()
{
    return "";
}

std::string JValue::string_value() const
{
    return "";
}

JObject JValue::object_value() const
{
    return JObject();
}

JArray JValue::array_value() const
{
    return JArray();
}

Json& JValue::operator[] (const std::string& key)
{
    return nul_val();
}

Json& JValue::operator[] (uint32_t i)
{
    return nul_val();
}

//===================================================================

bool JValueBool::bool_value() const
{
    return m_value;
}


int JValueInt::num_value() const
{
    return m_value;
}
 

double JValueDouble::double_value() const
{
    return m_value;
}


std::string JValueString::string_value() const
{
    return m_value;
}


JArray JValueArray::array_value() const
{
    return m_value;
}


Json& JValueArray::operator[] (uint32_t i)
{
    return m_value.size() < i ? m_value[i] : nul_val();
} 

JObject JValueObject::object_value() const
{
    return m_value;
}

std::string JValueObject::print()
{
    std::string ret;


    return ret;
} 

Json& JValueObject::operator[] (const std::string& key)
{
    return m_value[key];
}


}
