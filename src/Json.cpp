#include "Json.h"
#include "JsonValue.h"
#include "JsonCodec.h"

namespace RainJson
{

Json::Json(int val)
{
    m_value = std::make_shared<JValueInt>(val);
}

Json::Json(std::nullptr_t val)
{
    m_value = std::make_shared<JValueNULL>(val);
}

Json::Json(double val)
{
    m_value = std::make_shared<JValueDouble>(val);
}

Json::Json(bool val)
{
    m_value = std::make_shared<JValueBool>(val);
}

Json::Json(const char* val)
{
    m_value = std::make_shared<JValueString>(val);
}

Json::Json(const std::string& val)
{
    m_value = std::make_shared<JValueString>(val);
}

Json::Json(const JArray& val)
{
    m_value = std::make_shared<JValueArray>(val);
}

Json::Json(const JObject& val)
{
    m_value = std::make_shared<JValueObject>(val);
}

Json& Json::operator[] (const std::string& val)
{
    return (*m_value)[val];
}

Json& Json::operator[] (uint32_t i)
{
    return (*m_value)[i];
}

bool Json::empty() const
{
    return m_value == nullptr;
}

void Json::clear()
{
    m_value.reset();
}

bool Json::isContained(const std::string& key)
{
    bool ret = isObject();

    if(ret)
    {
        ret = m_value->object_value().count(key) > 0 ? true : false;
    }

    return ret;
}

bool Json::isNull() const
{
    return m_value->type() == NUL ? true : false;
}

bool Json::isArray() const
{
    return m_value->type() == ARRAY ? true : false;
}

bool Json::isNumber() const
{
    return m_value->type() == INT ? true : false;
}

bool Json::isDouble() const
{
    return m_value->type() == DOUBLE ? true : false;
}

bool Json::isString() const
{
    return m_value->type() == STRING ? true : false;
}

bool Json::isObject() const
{
    return m_value->type() == OBJECT ? true : false;
}

bool Json::isBool() const
{
    return m_value->type() == BOOL ? true : false;
}

int& Json::num_value()
{
    return m_value->num_value();
}

bool& Json::bool_value()
{
    return m_value->bool_value();
}

double& Json::double_value()
{
    return m_value->double_value();
}

std::string& Json::string_value()
{
    return m_value->string_value();
}

JObject& Json::object_value()
{
    return m_value->object_value();
}

JArray& Json::array_value()
{
    return m_value->array_value();
}

std::string Json::print() const
{
    std::string ret;

    bool isSuc = JCodec::encode(*this, ret);

    return isSuc ? ret : "";
}

bool Json::parser(const std::string& s, Json& json)
{
    return JCodec::parser(s, json);
}


}