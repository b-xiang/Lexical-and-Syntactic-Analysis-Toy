词法和语法分析的练习程序

词法分析部分通过配置文件中的正则表达式， 去构建自动机， 依次来进行词法分析。
配置文件的规则及样例请见bfn.data
被分析的源代码存于code.data
词法分析后的结果存放于lex.data
词法分析中遇到错误时， 向lex.data中写入错误信息， 并立即退出词法分析。

语法分析部分用LALR(1)对词法分析的结果进行分析。
词法分析首先根据bfn.data中的bfn范式， 构建出LALR(1)自动机， 进而构造出状态转移表。
样例中C-的状态转移表请见项目中的xls文件。
语法分析中遇到错误时， 会立即报错， 并退出程序。
如果无错误， 则返回 success。
