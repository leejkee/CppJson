#include "parser.h"
#include <stdexcept>
#include <cstdlib>
using namespace yazi::json;
Parser::Parser() : m_str(""), m_idx(0)
{
}

void Parser::load(const string & str)
{
    m_str = str;
    m_idx = 0;
}

Parser::~Parser()
{
}

void Parser::skip_white_space()
{
    /*跳过换行，回车，水平制表符*/
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
    {
        m_idx++;
    }
}

char Parser::get_next_token()
{
    skip_white_space();
    if (m_idx == m_str.size())
    {
        throw std::logic_error("unexpected end of input");
    }
    return m_str[m_idx++];
}

Json Parser::parse()
{
    char ch = get_next_token();
    switch (ch)
    {
        case 'n':
            m_idx--;
            return parse_null();
        case 't':
        case 'f':
            m_idx--;
            return parse_bool();
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            m_idx--;
            return parse_number();
        case '"':
            return Json(parse_string());
        case '[':
            return parse_array();
        case '{':
            return parse_object();
        default:
            break;
    }
    throw std::logic_error("unexpected character in parse json");
}

Json Parser::parse_null()
{
    if (m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;
        return Json();
    }
    throw std::logic_error("parse null error");
}

Json Parser::parse_bool()
{
    if (m_str.compare(m_idx, 4, "true") == 0)
    {
        m_idx += 4;
        return Json(true);
    }
    if (m_str.compare(m_idx, 5, "false") == 0)
    {
        m_idx += 5;
        return Json(false);
    }
    throw std::logic_error("parse bool error");
}

Json Parser::parse_number()
{
    size_t pos = m_idx;

    if (m_str[m_idx] == '-')
        m_idx++;

    // integer part
    // 处理小数点前面的
    if (m_str[m_idx] == '0')
    {
        m_idx++;
    }
    else if (in_range(m_str[m_idx], '1', '9'))
    {
        m_idx++;
        while (in_range(m_str[m_idx], '0', '9'))
        {
            m_idx++;
        }
    }
    else
    {
        throw std::logic_error("invalid character in number");
    }

    if (m_str[m_idx] != '.')
    {
        return Json(std::atoi(m_str.c_str() + pos));
    }

    // decimal part
    // 检测到了小数点，开始处理小数部分
    m_idx++;
    if (!in_range(m_str[m_idx], '0', '9'))
    {
        throw std::logic_error("at least one digit required in fractional part");
    }
    while (in_range(m_str[m_idx], '0', '9'))
    {
        m_idx++;
    }
    return Json(std::atof(m_str.c_str() + pos));
}

string Parser::parse_string()
{
    int pos = m_idx;
    while (true)
    {
        if (m_idx == m_str.size())
        {
            throw std::logic_error("unexpected end of input in string");
        }

        char ch = m_str[m_idx++];
        if (ch == '"')
        {
            break;
        }

        // The usual case: non-escaped characters
        // 转义字符，不需要处理，跳过即可，\u是占4个字符
        if (ch == '\\')
        {
            ch = m_str[m_idx++];
            switch (ch)
            {
                case 'b':
                case 't':
                case 'n':
                case 'f':
                case 'r':
                case '"':
                case '\\':
                    break;
                case 'u':
                    m_idx += 4;
                    break;
                default:
                    break;
            }
        }
    }
    // 检测到第一个"进入该函数，检测到第二个"退出解析，返回解析到的字串
    return m_str.substr(pos, m_idx - pos - 1); // m_str[2] = "a", substr返回是是从pos到m_idx-pos-1的左闭右开的子串, 即 'a'
}

Json Parser::parse_array()
{
    Json arr(Json::json_array);
    char ch = get_next_token();
    if (ch == ']')
    {
        return arr;
    }
    m_idx--;
    while (true)
    {
        arr.append(parse());// 返回的是一个Json对象，将解析到的Json对象添加到arr中
        ch = get_next_token();
        if (ch == ']')
        {
            break;
        }
        if (ch != ',') // 如果一个对象解析完了下一个token不是逗号，那么抛出异常，这个Json格式是错误的
        {
            throw std::logic_error("expected ',' in array");
        }
    }
    return arr;
}

Json Parser::parse_object()
{
    Json obj(Json::json_object);
    char ch = get_next_token();
    if (ch == '}')
    {
        return obj;
    }
    m_idx--;
    while (true)
    {
        ch = get_next_token();
        if (ch != '"')// obj中的string必须以"开头
        {
            throw std::logic_error("expected '\"' in object");
        }
        string key = parse_string();// 解析obj map中的string
        ch = get_next_token();
        if (ch != ':')// map中的string和Json中间以冒号相隔
        {
            throw std::logic_error("expected ':' in object");
        }
        obj[key] = parse();// 解析到map.second的Json对应到obj中
        ch = get_next_token();
        if (ch == '}')
        {
            break;
        }
        if (ch != ',')
        {
            throw std::logic_error("expected ',' in object");
        }
    }
    return obj;
}