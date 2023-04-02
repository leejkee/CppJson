## 一个轻量级Json解析器
本仓库为复刻项目，非原创作者，[原仓库地址](https://github.com/sukai33/yazi-json)。  
本仓库为代码打上详细注释。
## json格式
- json_null
- json_bool
- json_int
- json_double
- json_string
- json_array
- json_object

每个json对象有一个json类型，构造函数根据json类型生成对应的初始json的value

## 文件说明
- **json/json.h** 定义json类
- **json/json.cpp** 定义json类的成员函数，json对象，json数组的运算符重载，json的API
- **json/parser.h** 定义parser类，m_str存放待解析字串，m_idx存放解析到的索引
- **json/parser.cpp** parser解析各类json数据，并返回一个json类型，调用json的输出api进行输出
- **main.cpp** 测试主函数，测试用的json为vscode中的tasks.json
- **test.json** 本项目vscode中的tasks.json