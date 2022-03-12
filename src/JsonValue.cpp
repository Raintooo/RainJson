#include "JsonValue.h"

namespace RainJson
{

static Json& nul_val()
{
    static Json nul;
    return nul;
}

int& JValue::num_value()
{
    static int nul = 0;
    return nul;
}

bool& JValue::bool_value()
{
    static bool nul = false;
    return nul;
}

double& JValue::double_value()
{
    static double nul = 0.0f;
    return nul;
}

// ====================================================

std::string JValue::print()
{
    return "";
}

std::string& JValue::string_value()
{
    static std::string nul;
    return nul;
}

JObject& JValue::object_value()
{
    static JObject nul;
    return nul;
}

JArray& JValue::array_value()
{
    static JArray nul;
    return nul;
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

bool& JValueBool::bool_value()
{
    return m_value;
}


int& JValueInt::num_value()
{
    return m_value;
}
 

double& JValueDouble::double_value()
{
    return m_value;
}


std::string& JValueString::string_value()
{
    return m_value;
}


JArray& JValueArray::array_value()
{
    return m_value;
}

void JValueArray::push_back(const Json& val)
{
    m_value.push_back(val);
}

Json& JValueArray::operator[] (uint32_t i)
{
    return m_value.size() < i ? m_value[i] : nul_val();
} 

JObject& JValueObject::object_value()
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
