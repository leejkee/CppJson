#pragma once
#include <string>
using namespace std;
#include "json.h"
namespace yazi{
namespace json{

class Parser
{
public:
    Parser();
    ~Parser();
    void load(const string & str);
    /*解析Json数据函数*/
    Json parse();

private: 
    /*跳过空白符、换行字符等非解析字符*/
    void skip_white_space();
    /*索引跳到下一个token,在处理json字段过程中由解析函数处理json字段的索引前进*/
    char get_next_token();

    /*解析json各种格式的函数，提供给parse函数调用，返回值为一个Json对象*/
    Json parse_null(); // 检测"null", 返回一个空json对象
    Json parse_bool(); // 检测true和false, 返回一个bool的Json对象
    Json parse_number(); // 注意0，负数，小数的处理，小数点前后分开处理
    string parse_string(); // 注意转义字符的处理
    Json parse_array();
    Json parse_object();
    
    // 判断x是否在low到up之间
    bool in_range(int x, int low, int up)
    {
        return (x >= low && x <= up);
    }

private:
    // 存放待解析的字串
    string m_str;
    // 解析到的位置
    int m_idx;
};
} // namespace json
} // namespace yazi

