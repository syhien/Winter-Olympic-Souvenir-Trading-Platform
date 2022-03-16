---
marp: true
---

# Winter Olympic Souvenir Trading Platform 设计文档

杨茂琛 191180164

---

# 需求概述

支持数据可持久化、用户多身份（买家、卖家）、面向对象思想设计的交易平台。

- 包含一个抽象数据处理过程、实现数据可持久化的数据库

- 包含单一用户双重身份的交易平台

- 包含对系统可进行维护管理的管理员

- 包含一个多项式计算器并暴露计算接口

---

# 开发规范

## 编码规范

### 命名

小驼峰式命名法 `auto order = Order();` `getOperationCode();`

类内私有成员命名以双下划线`__`开头 `string __password`

### 格式化

提交前使用Visual Studio格式化工具进行格式化

---

# 开发规范

## 版本控制

Github Private Repository

master分支用于开发，release用于发布较可靠版本

database和calculator分支专用于数据库和计算器开发

*没有dev分支*

---

# 模块设计

## `main()`

程序主流程，用户（不含管理员）通过控制台与`main()`交互

`main()`将用户（不含管理员）的操作转交给相关对象处理（数据库、计算器等）

`main.cpp`内将不同角色的相关操作抽象成了若干小方法：

1. `WelcomePage()`未以任何身份登录

2. `AdministatorPage()`管理员登录，进行管理员身份验证，验证通过调用`Administator::HomePage()`进入管理员面板

3. `InfoManagePage()`用户管理个人信息

4. `SellerPage()`和`BuyerPage()`卖家和买家的交互程序

5. ...

---

# 模块设计

## `Terminal.cpp`

`Terminal.cpp`中对一些频繁使用的控制台交互过程进行了抽象

### `int getOperationCode()`

函数会输出引导文字，并从标准输入流获取一个使用者输入的字符串，通过`try...catch`语句及`stoi()`尝试转换为整型数字并返回

通过抽象这样一个输入过程，在主程式中只需要一个调用即可处理各种合法、不合法情况

---

# 模块设计

## `Terminal.cpp`

`Terminal.cpp`中对一些频繁使用的控制台交互过程进行了抽象

### `string getCurrentTime()`

无参数

直接返回当地的当前时间，格式为程序内统一的格式`%Y-%m-%d`

---

# 数据结构

## `Administator`类

管理员类

### 私有成员

- 不可改动的名称 `const string __name = "admin"`

- 不可改动的密码 `const string __password = "123456"`

- 可操作的数据库 `Database __database`

- 对管理员的相关操作的过程抽象

```c++
void __handleCommodityPage();
void __handleOrderPage();
void __handleUserPage();
```

---

# 数据结构

## `Administator`类

管理员类

### 公开成员

- 构造函数

- 供`main.cpp`调用的登录验证方法 `bool login(string name, string password)`

- 抽象与控制台交互的过程的方法 `void HomePage()`

---

# 数据结构

## 商品、订单和用户

没有显式地定义它们（比如以结构体或类的形式）

原因是考虑到C++不太好实现像Python那样的索引器

*详见数据库设计*

---

# 数据结构

## `Database`类

数据库类

### 特点

- 完成本项目**所有的**文件I/O操作

- 暴露一个方法以供所有类型的命令使用

- 自带基础的权限检查

---

# 数据结构

## `Database`类

### 如何组织数据

- 对一个属性值，把它和属性一起组织成一对`pair<string, wstring>`比如`<"name", L"南大">`

- 对一个元组，它有很多属性值，将所有属性值顺序不严格地放置在一个向量中`vector< pair<,> >`

- 对于一张表，它有很多个元组，将所有元组顺序不严格地放置在一个更大的向量中`vector< vector< pair<,> > >`

- 对于一个数据库，它有很多张表，将每张表的表名和表一起组织成一个键值对放到一个字典中`map<string, vector<vector<pair<,> > > >`

---

# 数据结构

## `Database`类

### 私有成员

- 存储所有表的字典（按表名索引） `__table`

- 表名到所有属性的向量 `map<string, vector<string> > __columnOfTable`

- 存放表名和表所在路径（默认是同目录下同名.txt文件） `vector<pair<string, string> > __tableFiles` 

- 无权限检查的请求数据库的方法`__perform()`

- 处理某个单独命令的方法 `vector<vector<pair<string, wstring> > > __select(istringstream& stream)`，作为参数的流是传入的命令的剩余部分

- 保存所有表到文件的方法 `void __save()`

---

# 数据结构

## `Database`类

### 公有成员

#### 构造函数`Database(vector<pair<string, string> > inputFiles)`

构造函数，`inputFiles`指示应该从哪个文件初始化表。`inputFiles`的默认值为同目录的与表同名的`.txt`文件

`inputFiles = { {"commodity","commodity.txt"},{"order","order.txt"},{"user","user.txt"},{"recharge","recharge.txt"} }`


---

# 数据结构

## `Database`类

### 公有成员

#### 析构函数`~Database()`

析构函数做的唯一的事情就是在消亡前最后一次显式地把数据库内容保存到文件：

```c++
Database::~Database()
{
	__save();
}
```

---

# 数据结构

## `Database`类

### 公有成员

#### 操作方法`perform()`

`perform()`的完整函数签名为：`vector<vector<pair<string, wstring>>> Database::perform(string command, string sender, string mode)`

`command`为指令，`sender`为请求的发起者（管理员或用户），`mode`为请求发起者的角色（买家或卖家或用户）

`perform()`本身不进行数据请求，它会将指令转发给`__perform()`并将`__perform()`返回的结果根据指令的发起者、发起者的角色进行过滤（例如卖家查看历史订单时只能看到自己为卖家的订单）

---

# 数据结构

## `Database`类

### 私有成员

#### 操作方法`__perform()`

`__perform()`的完整签名如下：
`vector<vector<pair<string, wstring> > > __perform(string command)`

接受一个字符串（命令），将字符串作为流`stringstream`尝试解析。如发现错误，立即抛出错误停止处理。如命令正确，则将请求的结果返回。

如果命令正确并且该命令对数据库产生了改动，`__perform()`会立即调用`__save()`将改动写回文件


---

# 数据结构

## `Calculator`类

### 公有成员

#### 计算方法 `string perform(string input)`

接受一个可能包含错误的字符串作为算式，尝试解析并计算。如发现错误会立即抛出错误并中断计算，返回具体的错误信息。如算式正确，返回至多精确到小数点后一位的计算结果的字符串形式

- 如果将返回值字符串看作数字，那么只会有整数和含1位小数的浮点数

- 不会对计算过程、计算结果进行保存

---

# 数据结构

## `Calculator`类

### 私有成员

#### `vector<item> __divide(string input)`

该方法会首先被`perform()`调用。方法将`string`形式的算式进行解析，并将算式内的元素分解成具体的操作数或操作符放入向量并返回

```c++
struct item
{
	bool isOperand; // 是否是操作数
	double operand; // 操作数，若操作符则为0
	char _operator; // 操作符，若操作数则为' '
};
```

---

# 数据结构

## `Calculator`类

### 私有成员

#### `vector<item> __divide(string input)`

该方法会进行一些初步的错误检查，抛出以下错误给`perform()`处理：

- 操作数格式不对，如`1.1.1` `2.` `.4`

- 不允许的字符，如`&` `%`

---

# 数据结构

## `Calculator`类

### 私有成员

#### `void __check(vector<item>& mid)`

该方法主要用于检查算式。它会检出以下错误并抛出给`perform()`：

- 两个连续的操作数，如`12` `3`

- 缺少必要的操作符，如`3(2+3)`

- 缺少必要的操作数，如`+2`

- 意义不明的格式，如`+*`

- 小括号无法成对匹配，如`1+(2*(-5)`

---

# 数据结构

## `Calculator`类

### 私有成员

#### `vector<item> __midToBack(vector<item>& mid)`

该方法顾名思义，将中缀表达式转为后缀

用到的是操作符栈内栈外优先级不同的方法：

```c++
map<char, int> stackPriority = { {'#',0},{'(',1},{'*',5},{'/',5},{'+',3},{'-',3},{')',6} };
map<char, int> comePriority = { {'#',0},{'(',6},{'*',4},{'/',4},{'+',2},{'-',2},{')',1} };
```

---

# 数据结构

## `Calculator`类

### 私有成员

#### `double __calculate(vector<item>& back)`

该方法最后被`perform()`调用，用于计算一个后缀表达式

可能会抛出以下错误给`perform()`：

- 除`0`错误

- 操作数过多（操作符不足）或操作数不足（操作符过多）
此错误理论上应该在`__check()`中就被检出，但是如果碰到了还是会抛出

