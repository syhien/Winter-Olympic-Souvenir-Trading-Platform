# 冬奥纪念品交易平台 用户手册

开发者：杨茂琛 191180164

## 引言

本用户手册的目的旨在明确一些平台使用规范，服务于普通买家、卖家及管理员。

冬奥纪念品交易平台（以下简称“本平台”）南京大学计算机科学与技术系选修课高级程序设计的课程项目，旨在实践C++面向对象开发。

## 软件概述

本平台提供一个纪念品交易平台，用户可以在本平台上进行纪念品交易，并可以查看自己的交易记录。

本平台支持多用户登录，每个用户可以自行创建账户，并可以在本平台上进行纪念品交易。本平台支持管理员账户，可以对用户进行管理，并可以查看用户的交易记录。

本平台仅提供英语环境。

## 运行环境

本平台支持跨平台编译运行，支持Windows、Linux、MacOS等多种操作系统。

其中使用了部分预览的C++20特性，需要G++10(`std=c++20`)或MSVC2019(`c++:/latest`)编译器。

## 使用说明

### 安装和初始化

本平台无需安装，直接运行可执行文件即可。

本平台可从文件初始化，同时支持无文件全新初始化。

### 输入和输出

仅支持英文输入，输出为英文。

## 运行说明

### 初始化文件

| 文件                   | 文件名（带*为必需） | 描述               |
| ---------------------- | ------------------- | ------------------ |
| user                   | user.txt            | 存储用户信息       |
| order                  | order.txt           | 存储订单信息       |
| commodity              | commodity.txt       | 存储商品信息       |
| recharge**(Feature)**  | recharge.txt        | 存储用户充值信息   |
| blacklist**(Feature)** | blacklist.txt       | 存储用户黑名单信息 |

初始化文件的名称不可更改，大小写敏感。

初始化文件的存放路径为和可执行文件同目录，不可更改，目录不应该包含特殊字符。

### 保存文件

本平台会根据运行清空更新初始化文件中的内容，同时产生以下日志文件

| 文件    | 文件名      | 描述               |
| ------- | ----------- | ------------------ |
| command | command.txt | 存储数据库操作记录 |

日志文件的存放路径和可执行文件同目录

## 常规过程

### 用户注册

```
                      1.Log in                     2.Sign up                        3.Exit     4.Log in as Administrator
Input operation code:2
Input username:guide
Input password:123456789
Input phone number:17776690339
Input address:nju
Check informations:

username:guide
password:123456789
phone number:17776690339
address:nju

Input 1 to sign up or input others to quit
Input operation code:1
Sign up successfully
```

进入系统后选择操作2并输入新用户信息，输入信息完毕后会将信息输出确认，确认注册则新建用户并返回上层。

支持的用户注册的输入如下：

| 字段名       | 字段描述 | 允许的字段值                                         |
| ------------ | -------- | ---------------------------------------------------- |
| username     | 用户名   | 10字符内的英文字母、数字**（本平台提供的额外支持）** |
| password     | 密码     | 20字符内的小写字母、数字                             |
| phone number | 联系方式 | 20字符内的数字                                       |
| address      | 地址     | 40字符内的英文字母、数字**（本平台提供的额外支持）** |

### 用户登录

```
                      1.Log in                     2.Sign up                        3.Exit     4.Log in as Administrator
Input operation code:1
Input username:guide
Input password:12345678
Name or password is wrong.
Try again?
Input 1 to retry or input others to quit
Input operation code:1
Input username:guide
Input password:123456789
Log in successfully
```

进入系统后选择操作1并在引导下输入账号密码

**Feature: **本平台提供了基本的隐私保护，账号或密码其中一项不匹配时返回模糊的错误，防止恶意使用者爬取用户名

### 管理员登录

```
                      1.Log in                     2.Sign up                        3.Exit     4.Log in as Administrator
Input operation code:4
Input administrator name:admin
Input administrator password:123456
Log in successfully!
```

本平台内置一个管理员身份，管理员身份信息如下：

| 字段名                 | 字段描述   | 固定的字段值 |
| ---------------------- | ---------- | ------------ |
| administrator name     | 管理员名称 | admin        |
| administrator password | 123456     | 123456       |

管理员支持商品、订单、用户三大类操作。

```
         1.Commodity operation             2.Order operation              3.User operation                     4.Log out
Input operation code:1
       1.Check all commodities        2.Search for commodity        3.Uncontinue commodity                      4.Return
Input operation code:4
         1.Commodity operation             2.Order operation              3.User operation                     4.Log out
Input operation code:2
            1.Check all orders            2.Search for order                      3.Return
Input operation code:3
         1.Commodity operation             2.Order operation              3.User operation                     4.Log out
Input operation code:3
             1.Check all users                    2.Ban user                      3.Return
```

管理员能够实现以下操作：

| 操作 | 操作对象         | 操作描述                               |
| ---- | ---------------- | -------------------------------------- |
| 查看 | 商品、订单、用户 | 管理员查看所有的商品、订单、用户信息   |
| 下架 | 商品             | 管理员下架指定的在售商品，该操作不可逆 |
| 封禁 | 用户             | 管理员封禁指定的用户，该操作不可逆     |

### 用户维护个人信息

用户登录后，可对自身的信息进行维护（包括账户充值）。

```
Log in successfully
                  1.I am Buyer                 2.I am Seller              3.Manage profile            4.Manage blacklist                      5.Return
Input operation code:3
               1.Check profile                2.Edit profile                    3.Recharge                      4.Return
Input operation code:
```

可查看并维护的字段如下：

| 字段名                | 字段描述 | 允许的新字段值                                       |
| --------------------- | -------- | ---------------------------------------------------- |
| username              | 用户名   | 10字符内的英文字母、数字**（本平台提供的额外支持）** |
| password**(Feature)** | 密码     | 20字符内的小写字母、数字                             |
| phone number          | 联系方式 | 20字符内的数字                                       |
| address               | 地址     | 40字符内的英文字母、数字**（本平台提供的额外支持）** |
| balance               | 余额     | 通过`Recharge`进行充值后自动计算更新                 |

### 用户维护黑名单(Feature)

> 讨厌一个人不需要太多的理由

用户登录后，可对自己的黑名单进行维护。用户在购买商品时不会看到自己黑名单中用户发布的商品，用户发布的商品不允许自己黑名单中用户购买。

```
                  1.I am Buyer                 2.I am Seller              3.Manage profile            4.Manage blacklist                      5.Return
Input operation code:4
             1.Check blacklist               2.Add blacklist                      3.Return
Input operation code:1
                  bannedUserID                          date
                          U001                    2022-03-30
                          U003                    2022-03-31
             1.Check blacklist               2.Add blacklist                      3.Return
Input operation code:
```

用户添加黑名单无需任何权限，该操作是不可逆的。

用户添加黑名单需要提供以下信息：

| 字段（带*为必需项） | 字段描述           | 允许的字段值                           |
| ------------------- | ------------------ | -------------------------------------- |
| userID*             | 拉黑的用户的用户ID | 用户ID所指的用户存在且不是当前用户本人 |

### 卖家交易

用户登录后选择我是卖家即可进入卖家的操作界面。

```
                  1.I am Buyer                 2.I am Seller              3.Manage profile            4.Manage blacklist                      5.Return
Input operation code:2
               1.New commodity             2.Check commodity              3.Edit commodity       4.Discontinue commodity                 5.Check order                      6.Return
Input operation code:
```

卖家支持进行以下操作：

| 操作名                 | 操作描述                 | 操作所需信息                                                 |
| ---------------------- | ------------------------ | ------------------------------------------------------------ |
| New commodity          | 上架商品                 | 商品名称、商品单价、商品数量、商品描述                       |
| Check commodity        | 查看自己已上架商品       |                                                              |
| Edit commodity         | 修改自己已上架商品的信息 | 商品ID、商品新字段的值（商品名称、商品单价、商品数量、商品描述） |
| Disconntinue commodity | 下架商品                 | 商品ID                                                       |
| Check order            | 查看自己卖出的订单       |                                                              |

上架商品、修改商品信息时的商品字段的值遵循以下规定：

| 字段名      | 字段描述 | 允许的字段值                                                 |
| ----------- | -------- | ------------------------------------------------------------ |
| name        | 商品名称 | 不含空格的20字符内的英文字母、数字**（本平台提供的额外支持）** |
| price       | 商品单价 | 大于0的至多精确到小数点后1位的数字                           |
| number      | 商品数量 | 正整数                                                       |
| description | 商品描述 | 200字符内的英文字符                                          |

**Feature: **卖家可通过维护自己的黑名单来阻止某些买家购买商品。

卖家可以浏览卖出订单，其中的字段及其含义如下：

| 字段        | 字段描述                   | 字段值样式      |
| ----------- | -------------------------- | --------------- |
| orderID     | 描述本次交易的订单的唯一ID | Txxx            |
| commodityID | 订单交易的商品的ID         | Mxxx            |
| unitPrice   | 交易商品的商品单价         | 含1位小数的正数 |
| number      | 买家购买的商品数量         | 正整数          |
| date        | 交易日期                   | yyyy-mm-dd      |
| sellerID    | 卖家（即用户自己）ID       | Uxxx            |
| buyerID     | 买家ID                     | Uxxx            |

### 买家交易

用户登录后选择我是买家即可进入买家界面。

```
                  1.I am Buyer                 2.I am Seller              3.Manage profile            4.Manage blacklist                      5.Return
Input operation code:1
             1.View commoditys               2.Buy commodity                      3.Search                 4.View orders   5.View details of commodity                      6.Return
```

买家支持进行以下操作：

| 操作名                    | 操作描述                     | 操作所需信息     |
| ------------------------- | ---------------------------- | ---------------- |
| View commodity            | 查看在售的其他用户上架的商品 |                  |
| Buy commodity             | 购买商品                     | 支持购买的商品ID |
| Search                    | 全字段搜索**(Feature)**      | 搜索关键字       |
| View orders               | 查看历史购买订单             |                  |
| View details of commodity | 查看商品库中某商品的商品描述 | 商品ID           |

买家购买商品时需满足以下条件：

- 商品在售
- 商品的卖家不是自己
- 买家与卖家之间不存在任何单向或双向的黑名单关系
- 商品的可购买数量大于等于购买数量
- 买家的余额足够支付订单

买家搜索商品时支持搜索以下字段：

| 字段名        | 字段描述                    |
| ------------- | --------------------------- |
| commodityID   | 商品ID**(Feature)**         |
| commodityName | 商品名称                    |
| price         | 商品单价**(Feature)**       |
| number        | 商品可购买数量**(Feature)** |
| description   | 商品描述**(Feature)**       |
| sellerID      | 卖家的用户ID**(Feature)**   |
| addedDate     | 上架时间**(Feature)**       |

买家可以浏览购买订单，其中的字段及其含义如下：

| 字段        | 字段描述                   | 字段值样式      |
| ----------- | -------------------------- | --------------- |
| orderID     | 描述本次交易的订单的唯一ID | Txxx            |
| commodityID | 订单交易的商品的ID         | Mxxx            |
| unitPrice   | 交易商品的商品单价         | 含1位小数的正数 |
| number      | 买家购买的商品数量         | 正整数          |
| date        | 交易日期                   | yyyy-mm-dd      |
| sellerID    | 卖家ID                     | Uxxx            |
| buyerID     | 买家（即用户自己）ID       | Uxxx            |

## 程序文件一览表

| 文件                   | 文件名（带*为必需） | 描述               |
| ---------------------- | ------------------- | ------------------ |
| 可执行文件             | 任意                | 本平台             |
| user                   | user.txt            | 存储用户信息       |
| order                  | order.txt           | 存储订单信息       |
| commodity              | commodity.txt       | 存储商品信息       |
| recharge**(Feature)**  | recharge.txt        | 存储用户充值信息   |
| blacklist**(Feature)** | blacklist.txt       | 存储用户黑名单信息 |
| command                | command.txt         | 存储数据库操作记录 |
