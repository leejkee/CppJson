# pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;

namespace yazi{
namespace json{
class Json
{
public:
    
    /* json数据格式可能的类型 */
    /*空类型*/
    /*bool类型*/
    /*int类型*/
    /*double类型*/
    /*string类型*/
    /*array类型*/
    /*Json类型*/
    enum Type
    {
        json_null = 0,
        json_bool,
        json_int,
        json_double,
        json_string,
        json_array,
        json_object
    };

    /* 构造函数 */
    /* bool值 */
    /* int值 */
    /* double值 */
    /* c字串值 */
    /* string值 */
    /* Json类型 */
    /* 其他Json值 */
    Json();
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char* value);
    Json(const string & value);
    Json(Type type);
    Json(const Json & other);


    /* 运算符重载 */
    /* 类型转换运算符，转换为基本类型 */
    operator bool() const;
    operator int() const;
    operator double() const;
    operator string() const;
    /* 下标运算符 map：Obj[first] = second  Json: Obj[string] = Json */
    Json & operator [] (const char * key);
    Json & operator [] (const string & key);
    Json & operator [] (int index);
    /* 向m_array的末尾添加新Json */
    void append(const Json & other);
    /* 拷贝运算符 */
    void operator = (const Json & other);
    /* 两个Json是否相等 */
    bool operator == (const Json & other);
    bool operator != (const Json & other);

    /* API */
    /* 拷贝 */
    void copy(const Json & other);
    /* 清理内存，拷贝前先清理内存 */
    void clear();
    /* 将json的各种类型转换为string的stream便于输出 */
    string str() const;
    /* 迭代器 */
    typedef std::vector<Json>::iterator iterator;
    iterator begin(){ return (m_value.m_array)->begin(); }
    iterator end(){ return (m_value.m_array)->end(); }
    /* 类型判断 */
    bool isNull() const { return m_type == json_null; }
    bool isBool() const { return m_type == json_bool; }
    bool isInt() const { return m_type == json_int; }
    bool isDouble() const { return m_type == json_double; };
    bool isString() const { return m_type == json_string; };
    bool isArray() const { return m_type == json_array; };
    bool isObject() const { return m_type == json_object; };
    /* 显式类型转换 */
    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    string asString() const;
    /* 存在判断 */
    bool has(int index);
    bool has(const char * key);
    bool has(const string key);
    /* 删除值函数 */
    void remove(int index);
    void remove(const char * key);
    void remove(const string key);
    /* 解析Json，并将解析结果转为一个json对象填充至当前对象 */
    void parse(const string & str);

private:

    // Value用来存储数据 
    union Value
    {
        bool m_bool;
        int m_int;
        double m_double;
        std::string* m_string;
        std::vector<Json>* m_array;
        std::map<string, Json>* m_object; // {"str": "hello"}
    };

    Type m_type;
    Value m_value;
};

} // namespace json
} // namespace yazi
