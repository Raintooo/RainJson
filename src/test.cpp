
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

    std::string ss("\n{\"a\":123,\"b\":{\"c\":345,\"d\":\"ddd\"},\"e\":\"eeee\"\n}");

    RainJson::Json jj =  RainJson::Json::parser(ss);

    RainJson::Json jo = jj["b"];

    cout<< jj["a"].num_value()<< endl;

    cout<< jo["c"].num_value()<< endl;
    cout<< jo["d"].string_value()<< endl;

    cout<< jj.isContained("d")<< endl;
    cout<< jj["e"].string_value()<< endl;

}