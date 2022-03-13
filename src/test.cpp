
#include <iostream>
#include <string>
#include "Json.h"

#include "libcJSON.h"

using namespace std;

int main()
{
    std::string s("s");
    RainJson::JArray arr{1, 2, 3, 4, 
                    RainJson::JObject{{"aa", 123}, {"bb","\ftt"}},
                    6,
                    };

    RainJson::JObject sub{{"s1", 112}, {"s2", 4.5f}};

    RainJson::Json j = RainJson::JObject{
                        {"js", "哈哈"},
                        {"t1", 1}, {"t2", "\t2"},
                        {"t3", arr}
                        };
    // cout<< j["t1"].num_value();
    // cout<< j["t2"].string_value();

    // RainJson::JArray e = j["t3"].array_value();

    // cout<< j.print()<< endl;

    // cJSON* root = cJSON_CreateObject();
    // cJSON_AddStringToObject(root, "kkk", "哈哈");

    // cout<< cJSON_Print(root);

    std::string ss("{\"a\":123,\"b\":{\"c\"\r\r:345,\"d\":\"ddd\"},\"e\":\r\r\"eeee\",\"k\":\r\r[\r\r8890,\"kkkk\",{\"cc\":112,\"oo\":\"ppp\"}\r\r]\n\f}");
    // std::string ss("{\"a\":123,\"e\":\"eeee\",\"k\":[0,\"ss\"]}");

    RainJson::Json jj =  RainJson::Json::parser(ss);

    if(jj.isNull())
    {
        cout<< "invaild json";
        return 0;
    }

    // RainJson::Json jo = jj["b"];

    RainJson::JArray jarr = jj["k"].array_value();

    for(auto& n : jarr)
    {
        if(n.isNumber())
        {
            cout<< n.num_value()<< endl;
        }
        else if(n.isString())
        {
            cout<< n.string_value()<< endl;
        }
        else if(n.isObject())
        {
            cout<< "cc: "<< n["cc"].num_value()<< endl;
            cout<< "oo: "<< n["oo"].string_value()<< endl;
        }
    }

    cout<< jj["a"].num_value()<< endl;
    cout<< jj["e"].string_value()<< endl;

    // cout<< jo["c"].num_value()<< endl;
    // cout<< jo["d"].string_value()<< endl;

    // cout<< jj.isContained("d")<< endl;
    // cout<< jj["e"].string_value()<< endl;

}