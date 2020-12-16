项目文件说明
====

编译源码
----

在源码目录下输入
```
g++ *.cpp -o turing
```
即可生成可执行程序turing。

turing命令使用
----

```
usage: turing [-v|--verbose] [-h|--help] <tm> <input>
```

对拍测试
----

提供test.py对拍测试程序，自行阅读程序，可以方便地进行对拍测试。  
对拍测试的流程是将图灵机的输出打印到tmp文件，然后test.py读入tmp文件，和它自己得到的结果进行比较。  
使用如下命令即可进行测试：  
```
python test.py
```