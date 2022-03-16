#include <stack>

#include "JsonCodec.h"
#include "JsonValue.h"
#include "utf.h"


namespace RainJson
{

static int _parser(Json& json, const std::string& data);

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

void escape(std::string& s)
{
    if(!s.empty())
    {
        bool utf16Transfer = false;
        for(auto n = s.begin(); n != s.end();)
        {
            if(*n == '\\')
            {
                n++;     
                switch(*n)
                {
                    case 'r': *n = '\r'; n--; n = s.erase(n); break;
                    case 'n': *n = '\n'; n--; n = s.erase(n);break;
                    case 'f': *n = '\f'; n--; n = s.erase(n);break;
                    case 't': *n = '\t'; n--; n = s.erase(n);break;
                    case 'b': *n = '\b'; n--; n = s.erase(n);break;
                    case 'u': utf16Transfer = true; break;
                    default: break;
                }
            }
            else
            {
                n++;
            }
        }

        if(utf16Transfer)
        {
            s = UtfCodec::utf16le_to_utf8(s.c_str(), s.length());
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
        for(const auto& n : const_cast<Json&>(json).object_value())
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
        for(const auto& n : const_cast<Json&>(json).array_value())
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
        encRet += std::to_string(const_cast<Json&>(json).double_value());
    }
    else if(json.isString())
    {
        encRet += "\"";
        std::string s = const_cast<Json&>(json).string_value();
        
        stringProcess(s);

        encRet += s;
        encRet += "\"";
    }
    else if(json.isNumber())
    {
        encRet += std::to_string(const_cast<Json&>(json).num_value());
    }
    else if(json.isBool())
    {
        encRet += const_cast<Json&>(json).bool_value() ? "true" : "false";
    }
    else
    {
        encRet += "null";
    }

    encRet += ",";

    return ret;
}

bool JCodec::encode(const Json& json, std::string& encRet)
{
    bool ret = true;

    encRet += "{";

    for(const auto& n : const_cast<Json&>(json).object_value())
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

static int findEnd(const std::string_view& data)
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

static int getArrayElementLen(const std::string& data, int i)
{
    int ret = 0;
    bool isInString = false;
    bool isObject = false;
    int curindex = 0;

    if(!data.empty())
    {
        int k = 0;
        for(k = 1; k < data.length(); k++)
        {
            if(data[k] == ']' && !isInString)
            {
                break;
            }
            else if(data[k] == '{')
            {
                isObject = true;
            }
            else if((data[k] == '}' && isObject))
            {
                isObject = false;
            }
            else if((data[k] == ',' && !isInString && !isObject))
            {
                ret++; // ','长度也加上

                if(curindex == i)
                {
                    break;
                }
                else
                {
                    ret = 0;
                    curindex++;
                    continue;
                }

                curindex++;
            }
            else if(data[k] == '\"')
            {
                isInString = !isInString;
            }

            ret++;
        }

        ret = ret == 0 ? 1 : ret;
    }

    return ret;
}

static bool isVaildFormat(const std::string& data)
{
    bool ret = true;

    int begin = skip(data, 0);

    if(data[begin] == '{' || data[begin] == '[')
    {
        int i = begin;
        std::stack<char> stack;

        stack.push(data[begin]);
        i++;

        while (stack.size() && (i < data.length()))
        {
            if((data[i] == '{') || (data[i] == '[') )
            {
                stack.push(data[i]);
            }
            else if((data[i] == '}') || (data[i] == ']'))
            {
                stack.pop();
            }

            i++;
        }
        
        ret = (stack.size() > 0) ? false : true;
    }
    else
    {
        ret = false;
    }

    return ret;
}

static bool isNumber(const std::string& s)
{
    bool ret = true;

    for(const auto& n : s)
    {
        if(n > '9' || n < '0')
        {
            ret = false;
            break;
        }
    }

    return ret;
}

static int parseObject(Json& json, const std::string& data)
{
    int ret = 0;

    if(json.empty())
    {
        json = JObject();
    }

    Json arrNode;
    if(json.isArray())
    {
        arrNode = JObject();
    }

    size_t ks = data.find_first_of("\"");
    size_t ke = data.find_first_of(":");
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

            if(json.isArray())
            {
                int len = _parser(arrNode, data.substr(sk));

                ret = len < 0 ? -1 : (ret + len);
            }
            else
            {
                int len = _parser(json, data.substr(sk));

                ret = len < 0 ? -1 : (ret + len);
            }

            if(ret < 0)
            {
                break;
            }
        }
    }

    if(!arrNode.empty())
    {
        json.array_value().push_back(arrNode);
    }

    return ret;
}

static int parseArray(Json& json, const std::string& data, int begin)
{
    int ret = 0;

    int b = skip(data, begin);

    if(!json.isArray())
    {
        json = JArray();
    }

    // std::string_view s(data.c_str() + b, data.length() - b);
    // int e = findEnd(data.substr(b));
    int e = findEnd(std::string_view(data.c_str() + b, data.length() - b));
    int len = 0; 
    int i = 0;
    int curlen = 0;

    while(len + 1 < e) // +1 加上 '[' 长度
    {
        int sk = skip(data, len);
        if(sk > len)
        {
            len += (sk - len);
        }

        int l = getArrayElementLen(data, i++);

        if((curlen = _parser(json, data.substr(b + 1 + len, l))) < 0)
        {
            break;
        }

        len += curlen;
    }

    ret = e-b; 

    return ret;   
}

static int parseString(Json& json, const std::string& data, int begin)
{
    int ret = 0;

    size_t ks = begin;//data.find_first_of("\"");
    // size_t ke = data.find_first_of(":", ks);

    int len = findEnd(std::string_view(data.c_str(), data.length())); 
    int b = skip(data, len+1);
    size_t ke = data.find_first_of(":", b);

    if(ks != std::string::npos && ke != std::string::npos && (len + 1 < ke))
    {
        Json subObj = JObject();

        b = skip(data, ke+1);

        len = findEnd(std::string_view(data.c_str() + b, data.length() - b)); 

        // int len = findEnd(data.substr((b = skip(data, b)))); 
            
        if(data[b] != '{' && data[b] != '[' )
        {
            len += 2;// len是字符串长度 +2是把双引号号加进去, 保留双引号 
        }

        // std::string tmp = data.substr(ks+1, data.find_first_of("\"", ks+1) - 1);

        ret = _parser(subObj, data.substr(b, len));

        if(ret >= 0)
        {
            json[std::move(data.substr(ks+1, data.find_first_of("\"", ks+1) - 1))] = subObj;

            ret = b + len;
        }
    }
    else if(ks != std::string::npos)
    {
        std::string s = data;

        s.erase(0, 1);

        if(s.find_last_of(',') != std::string::npos || 
            s.find_last_of('}') != std::string::npos || 
            s.find_last_of(']') != std::string::npos )
        {
            s.erase(s.length()-2, 2);
        }
        else
        {
            s.erase(s.length()-1, 1);
        }

        escape(s);

        if(json.isArray())
        {
            json.array_value().push_back(std::move(Json(s)));
            // dynamic_cast<JValueArray*>(json.ptr().get())->push_back(std::move(Json(s)));
        }
        else
        {
            json = Json(s);
        }

        ret = data.length();
    }    

    return ret;
}

static int parseEnd(Json& json, const std::string& data, int begin)
{
    int ret = begin;

    int e = skip(data, 1);

    if(data.find_first_of(',') != std::string::npos)
    {
        ret += 1;
    }

    ret += e;

    return ret;
}

static int parseNumber(Json& json, const std::string& data)
{
    int ret = 0;
    int e = 0;
    if((e = data.find_first_of(',')) != std::string::npos || 
        (e = data.find_first_of('}')) != std::string::npos)
    {
        std::string num = std::move(data.substr(0, e));

        if(isNumber(num))
        {
            trim(num);

            if(json.isArray())
            {
                json.array_value().push_back(std::move(Json(std::stoi(num))));
                // dynamic_cast<JValueArray*>(json.ptr().get())->push_back(Json(std::stoi(num)));
            }
            else
            {
                json = std::stoi(num);
            }
            ret = e + 1;
        }
        else
        {
            ret = -1;
        }

    }   

    return ret; 
}

static int _parser(Json& json, const std::string& data)
{
    int ret = 0;

    ret = skip(data, 0);

    if((data.compare(ret, 4, "null") == 0) || (data.compare(ret, 4, "NULL") == 0))
    {
        json = Json(nullptr);
        ret += data[ret + 4] == ',' ? 5 : 4;
    }
    else if(data.compare(ret, 4, "true") == 0)
    {
        json = Json(true);
        ret += data[ret + 4] == ',' ? 5 : 4;
    }
    else if(data.compare(ret, 5, "false") == 0)
    {
        json = Json(false);
        ret += data[ret + 5] == ',' ? 6 : 5;
    }
    else
    {
        if(data[ret] == '{')         
            ret = parseObject(json, data);
        else if(data[ret] == '\"')   
            ret = parseString(json, data, ret);
        else if(data[ret] == '[')    
            ret = parseArray(json, data, ret);
        else if((data[ret] == '}') || (data[ret] == ']'))   
            ret = parseEnd(json, data, ret);
        else if(data[ret] >= '0' && data[ret] <= '9')     
            ret = parseNumber(json, data);
        else
            ret = -1;
    }

    return ret;
}

bool JCodec::parser(const std::string& data, Json& json)
{
    bool ret = !data.empty() && isVaildFormat(data);

    if(ret)
    {
        json = JObject();
        int n =  _parser(json, data);
        if(n < 0)
        {
            ret = false;
        } 
    }

    return ret;
}


}