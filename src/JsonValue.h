#ifndef JSONVALUE_H
#define JSONVALUE_H

#include "Json.h"

namespace RainJson
{

template <JType t, typename T>
class Value : public JValue
{
protected:
    T m_value;

    Value(const T& val) : m_value(val){}

    JType type() const
    {
        return t;
    }

    std::string print() override
    {
        return "";
    }
};

class JValueBool : public Value<BOOL, int>
{

public:
    JValueBool(bool v) : Value(v){}

    bool bool_value() const;
}; 

class JValueInt : public Value<INT, int>
{

public:
    JValueInt(int v) : Value(v){}

    int num_value() const;  
}; 

class JValueDouble : public Value<DOUBLE, double>
{

public:
    JValueDouble(double val) : Value(val){}

    double double_value() const;
};

class JValueString : public Value<STRING, std::string>
{

public:
    JValueString(const std::string& val) : Value(val){}

    std::string string_value() const;
};

class JValueArray : public Value<ARRAY, JArray>
{

public:
    JValueArray(const JArray& val) : Value(val){}

    JArray array_value() const;

    Json& operator[] (uint32_t i);   
};

class JValueObject : public Value<OBJECT, JObject>
{

public:
    JValueObject(const JObject& val) : Value(val){}

    std::string print(); 

    JObject object_value() const;

    Json& operator[] (const std::string& key);
};

}

#endif