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
    virtual int& num_value();
    virtual bool& bool_value();
    virtual double& double_value();
    virtual std::string& string_value();
    virtual JObject& object_value();
    virtual JArray& array_value();

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

    std::shared_ptr<JValue> ptr();

    bool isContained(const std::string& key);

    bool isNull() const;

    bool isArray() const;
    bool isNumber() const;
    bool isDouble() const;
    bool isString() const;
    bool isObject() const;
    bool isBool() const;

    int& num_value();
    bool& bool_value();
    double& double_value();
    std::string& string_value();
    JObject& object_value();
    JArray& array_value();

    std::string print() const;

    static Json parser(const std::string& s);

};


}

#endif