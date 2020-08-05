# SM2密码方案C++实现

## 环境

使用MIRACL库来完成大数运算，通过C++运算符重载以直接通过一般的运算表达式完成大数运算。
在Linux环境下可以使用`install_miracl.sh`来安装，将会在项目文件夹下创建`miracl`目录，并将源代码和静态链接库安装到`/usr/include/miracl`和`/usr/lib`。

## 运行与调试

通过`CMakeLisst.txt`组织项目代码，可以通过CLion加载该文件来调试运行对应的测试。

`test`下包含Catch2测试用例，负责对应模块的单元测试。
