
#include <iostream>
#include <string>
#include "Json.h"

#include "libcJSON.h"
#include "utf.h"

using namespace std;

void json_test()
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

    std::string ss("{\n\"null\":\"\\u0068\\u0065\\u006c\\u006c\\u006f\",\"a\":123,\"b\":{\"c\":345,\"d\":\"ddd\"},\"e\":\"eeee\",\"k\":[8890,\"kkkk\",{\"cc\":112,\"oo\":\"pp\\np\"}]}");
    // std::string ss("{\"a\":123,\"e\":\"eeee\",\"k\":[0,\"ss\"]}");
    // std::string ss();
    RainJson::Json jj;

    cout<< "jj: "<< jj.empty()<< endl;

    bool ret = RainJson::Json::parser(ss, jj);
    if(ret)
    {
        cout<< "jj: "<< jj.empty()<< endl;

        if(jj.empty())
        {
            cout<< "invaild json";
            return;
        }

        cout<< jj["null"].isNull()<< endl;
        cout<< jj["null"].isBool()<< endl;

        if(jj["null"].isBool())
        {
            cout<< "null: "<< jj["null"].bool_value()<< endl;
        }

        if(jj["null"].isString())
        {
            cout<< "null: "<<jj["null"].string_value()<< endl;
        }

        cout<< "a: "<<jj["a"].num_value()<< endl;

        RainJson::Json jo = jj["b"];
        cout<< "c: "<<jo["c"].num_value()<< endl;
        cout<< "d: "<<jo["d"].string_value()<< endl;


        cout<< "e: "<<jj["e"].string_value()<< endl;

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
    }
    else
    {
        cout<< "parse json failed"<< endl;
    }

    // cout<< jo["c"].num_value()<< endl;
    // cout<< jo["d"].string_value()<< endl;

    // cout<< jj.isContained("d")<< endl;
    // cout<< jj["e"].string_value()<< endl;    
}


void utf_test()
{
    const char* s = "\\u0068\\u0065\\u006c\\u006c\\u006f";

    cout<< UtfCodec::utf16le_to_utf8(s, 31);
}

int main()
{
    json_test();

}