#ifndef JSONCPP_H
#define JSONCPP_H

#include <map>
#include <string>
#include <memory>
#include <vector>


namespace RainJson
{

class Json;
class JValue;

typedef std::map<std::string, Json> JObject;
typedef std::vector<Json> JArray;

enum JType
{
    INT,
    DOUBLE,
    BOOL,
    STRING,
    ARRAY,
    OBJECT
};

class JValue
{
public:
    virtual std::string print();

    virtual JType type() const = 0;
    virtual int num_value() const;
    virtual bool bool_value() const;
    virtual double double_value() const;
    virtual std::string string_value() const;
    virtual JObject object_value() const;
    virtual JArray array_value() const;

    virtual Json& operator[] (const std::string& key);
    virtual Json& operator[] (uint32_t i);
};

class Json
{
    std::shared_ptr<JValue> m_value;
public:
    Json() = default;
    Json(int val);
    Json(double val);
    Json(bool val);
    Json(const std::string& val);
    Json(const char* val);
    Json(void* val) = delete;

    // limit T must a vector like
    // template<typename T>
    // 暂时先使用vector
    // Json(const std::vector<Json>& val);
    Json(const JArray& val);
    Json(const JObject& val);

    Json& operator[] (const std::string& val);
    Json& operator[] (uint32_t i);

    bool isContained(const std::string& key);

    bool isNull() const;

    bool isArray() const;
    bool isNumber() const;
    bool isDouble() const;
    bool isString() const;
    bool isObject() const;
    bool isBool() const;

    int num_value() const;
    bool bool_value() const;
    double double_value() const;
    std::string string_value() const;
    JObject object_value() const;
    JArray array_value() const;

    std::string print() const;

    static Json parser(const std::string& s);

};


}

#endif