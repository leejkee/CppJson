#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/timeb.h>
using namespace std;

#include "json/json.h"
using namespace yazi::json;
int main()
{
    try
    {
        // 文件读取流，将其写入string流转换为待解析的string
        ifstream fin("./test.json");
        stringstream ss;
        ss << fin.rdbuf();
        const string & data = ss.str();

        struct timeval tv;
        mingw_gettimeofday(&tv, NULL);
        int start_sec = tv.tv_sec;
        int start_usec = tv.tv_usec;
        const int max = 1;
        for (int i = 0; i < max; i++)
        {
            Json json;
            json.parse(data);
            std::cout << json.str() << std::endl;
            json.clear();
        }
        mingw_gettimeofday(&tv, NULL);
        int end_sec = tv.tv_sec;
        int end_usec = tv.tv_usec;
        double time_diff = (end_sec - start_sec) * 1000000 + (end_usec - start_usec);
        std::cout << time_diff / 1000 / max << "ms" << std::endl;
    }
    catch (std::exception & e)
    {
        std::cout << "catch exception: " << e.what() << std::endl;
    }
    // const string & str = "[\"a\", 1, 2, 3, true]";
    // Json v;
    // v.parse(str);
    // cout << v.str();
    return 0;
}