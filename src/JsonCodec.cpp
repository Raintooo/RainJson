#include <stack>

#include "JsonCodec.h"


namespace RainJson
{

void stringProcess(std::string& s)
{
    size_t i = 0;

    for(auto& n : s)
    {
        switch(n)
        {
            case '\t': s.replace(i++, 1, "\\"); s.insert(i++, "t"); break;
            case '\f': s.replace(i++, 1, "\\"); s.insert(i++, "f"); break;
            case '\r': s.replace(i++, 1, "\\"); s.insert(i++, "r"); break;
            case '\n': s.replace(i++, 1, "\\"); s.insert(i++, "n"); break;
            case '\b': s.replace(i++, 1, "\\"); s.insert(i++, "b"); break;
            // case '\u': break;
            default:
                i++;
        }
    }
}

static bool _encode(const std::string &key, const Json& json, std::string& encRet)
{
    bool ret = true;

    if(!key.empty())
    {
        encRet += "\"" + key + "\":";
    }
    
    if(json.isObject())
    {
        encRet += "{";
        for(const auto& n : json.object_value())
        {
            _encode(n.first, n.second, encRet);
        }

        if(encRet[encRet.length() - 1] == ',')
        {
            encRet.erase(encRet.length() - 1);
        }

        encRet += "}";
    }
    else if(json.isArray())
    {
        encRet += "[";
        for(const auto& n : json.array_value())
        {
            _encode("", n, encRet);
        }

        if(encRet[encRet.length() - 1] == ',')
        {
            encRet.erase(encRet.length() - 1);
        }

        encRet += "]";
    }
    else if(json.isDouble())
    {
        encRet += std::to_string(json.double_value());
    }
    else if(json.isString())
    {
        encRet += "\"";
        std::string s = json.string_value();
        
        stringProcess(s);

        encRet += s;
        encRet += "\"";
    }
    else if(json.isNumber())
    {
        encRet += std::to_string(json.num_value());
    }
    else if(json.isBool())
    {
        encRet += json.bool_value() ? "true" : "false";
    }

    encRet += ",";

    return ret;
}

bool JCodec::encode(const Json& json, std::string& encRet)
{
    bool ret = true;

    encRet += "{";

    for(const auto& n : json.object_value())
    {
        _encode(n.first, n.second, encRet);
    }

    if(encRet[encRet.length() - 1] == ',')
    {
        encRet.erase(encRet.length() - 1);
    }

    encRet += "}";

    return ret;
}

static int findEnd(const std::string& data)
{
    int ret = 0;

    if(!data.empty())
    {
        if(data[0] == '{' || data[0] == '[')
        {
            std::stack<char> s;

            s.push(data[0]);
            ret++;

            while(s.size())
            {
                char t = data[ret];
                if((data[ret] == '{') || (data[ret] == '[') )
                {
                    s.push(data[ret]);
                }
                else if((data[ret] == '}') || (data[ret] == ']'))
                {
                    s.pop();
                }

                ret++;
            }

            if((data.length() > ret) && (data[ret] == ','))
            {
                ret++;
            }
        }
        else if(data[0] == '\"')
        {
            int i = 0;
            for(i = 1; (i < data.length()) && (data[i] != '\"'); i++)
            {
                ret++;
            }

            if(data[i+1] == ',')
            {
                ret++;
            }
        }
        else
        {
            for(int i = 1; (i < data.length()) && (data[i] != ','); i++)
            {
                ret++;
            }
        }
    }
    
    return ret;
}


static void trim(std::string& s)
{
    for(auto n = s.begin(); n != s.end();)
    {
        if(*n == '\r' || *n == '\n' || *n == '\f' || *n == '\b' || *n == '\t')
        {
            n = s.erase(n);
        }
        else
        {
            n++;
        }
    }
}

static int skip(const std::string& s, int b)
{
    int ret = b;

    for(int i = b; i < s.length(); i++)
    {
        if(s[i] == '\r' || s[i] == '\n' || s[i] == '\f' || s[i] == '\b' || s[i] == '\t')
        {
            ret++;
        }
        else
        {
            break;
        }
    }

    return ret;
}

static int _parser(Json& json, const std::string& data)
{
    int ret = 0;

    ret = skip(data, 0);

    if(data[ret] == '{')
    {
        if(json.isNull())
        {
            json = JObject();
        }

        size_t ks = data.find_first_of("\"");
        size_t ke = data.find_first_of(":");
        ret = 0;
        if(ks != std::string::npos && ke != std::string::npos)
        {
            int s = data.find_first_of("{");
            ret += skip(data, s) + 1;

            while(ret < data.length())
            {
                int sk = skip(data, ret);
                if(sk > ret)
                {
                    ret += (sk - ret);
                }
                ret += _parser(json, data.substr(sk));
            }
        }
    }
    else if(data[ret] == '\"')
    {
        size_t ks = data.find_first_of("\"");
        size_t ke = data.find_first_of(":", ks);

        if(ks != std::string::npos && ke != std::string::npos)
        {
            Json subObj;
            int b = skip(data, ke) + 1;//data.find_first_of("\"", ke);

            int len = findEnd(data.substr(b)); 
             
            if(data[b] != '{' && data[b] != '[' )
            {
                len += 2;// len是字符串长度 +2是把双引号号加进去, 保留双引号 
            }

            //  std::string tmp = data.substr(ks+1, data.find_first_of("\"", ks+1) - 1);

            ret = _parser(subObj, data.substr(b, len));

            json[data.substr(ks+1, data.find_first_of("\"", ks+1) - 1)] = subObj;

            ret = b + len;
        }
        else if(ks != std::string::npos)
        {
            std::string s = data;

            s.erase(0, 1);
            s.erase(s.length()-1, 1);

            if(s.find_last_of(',') != std::string::npos)
            {
                s.erase(s.length()-1, 1);
            }

            json = Json(s);
        }
    }
    else if(data[ret] == '[')
    {

    }
    else if(data[ret] == '}')
    {
        int e = skip(data, 1);

        if(data.find_first_of(',') != std::string::npos)
        {
            ret += 1;
        }

        ret += e;
    }
    else
    {
        int e = 0;
        if((e = data.find_first_of(',')) != std::string::npos || 
            (e = data.find_first_of('}')) != std::string::npos)
        {
            std::string num = data.substr(0, e);

            trim(num);

            json = std::stoi(num);
            ret = e + 1;
        }
    }

    return ret;
}

Json JCodec::parser(const std::string& data)
{
    Json ret = JObject();

    if(!data.empty())
    {
        _parser(ret, data); 
    }

    return ret;
}


}