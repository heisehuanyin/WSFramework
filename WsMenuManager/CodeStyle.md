# 代码风格定制手册
## 目的
1. 生成风格统一、简明明了、易于理解的代码

## 简要定义
### 类名称
采用Camel-Case命名，添加必要注释
> ClassName

### 类方法
1. public方法采用Camel-Case命名法，首个单词小写
> ClassName::publicMethod()

2. privete和protected方法采用连词命名法
> ClassName::_privete_method()

3. 方法形参采用单一小写单词或者首字母小写驼峰命名法
> ClassName::_privete_method(int number)

### 类变量成员
采用连词命名法多个单词连接
> ClassName::variable_name


### 内存管理
1. 内存实例生成方同时负责析构
