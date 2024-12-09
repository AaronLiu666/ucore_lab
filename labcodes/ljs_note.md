# Note

基于GNU make ()

## Makefile

__make的基本目的是自动的构建项目的目标文件，makefile是告诉make工具如何编译和link程序__

### makefile的基本内容

~~~makefile
#### 执行makefile的规则：prerequisites中如果有一个以上的文件比target文件要新的话，recipe所定义的命令就会被执行。
ifeq
...(条件判断)
endif

CC = gcc 
... (变量赋值)

target ...(目标，可以是文件或者中间产物/变量) : prerequisites ...（依赖的文件或者变量）
    recipe （执行的任意shell命令）
    (比如)
    echo "hello makefile!"
~~~

### 一个简单例子展示依赖关系

~~~makefile
edit : main.o kbd.o command.o display.o \
        insert.o search.o files.o utils.o
    cc -o edit main.o kbd.o command.o display.o \
        insert.o search.o files.o utils.o

main.o : main.c defs.h
    cc -c main.c
kbd.o : kbd.c defs.h command.h
    cc -c kbd.c
command.o : command.c defs.h command.h
    cc -c command.c
display.o : display.c defs.h buffer.h
    cc -c display.c
insert.o : insert.c defs.h buffer.h
    cc -c insert.c
search.o : search.c defs.h buffer.h
    cc -c search.c
files.o : files.c defs.h buffer.h command.h
    cc -c files.c
utils.o : utils.c defs.h
    cc -c utils.c
clean :
    rm edit main.o kbd.o command.o display.o \
        insert.o search.o files.o utils.o
# clean 不是一个文件，它只不过是一个动作名字，有点像C语言中的label一样，其冒号后什么也没有，
# 那么，make就不会自动去找它的依赖性，也就不会自动执行其后所定义的命令。要执行其后的命令，就要在
# make命令后明显得指出这个label的名字。这样的方法非常有用，我们可以在一个makefile中定义不用的
# 编译或是和编译无关的命令，比如程序的打包，程序的备份，等等。

# 由于不会自动在make时执行clean：后的指令，所以如果要执行需要手动指出
make clean
# 相似的，也可以单独对任何一个在makefile中声明了的target进行make
make utils.o
# make会查找utils.o的target并检查，如果依赖的文件比target的时间戳更新则会执行这个target下的命令，即
cc -c utils.c
~~~

### make是如何工作的

直接执行`make`指令时，会找到当前目录下的名字叫`Makefile`，如果找到则把`Makefile`中的第一个target作为最终的目标文件，如果这个target不存在或者时间戳比依赖文件老则会执行后面的代码来生成。如果依赖文件也不存在则会找到目标是这个依赖文件的规则进而生成依赖文件，以此类推。

### 简洁写法

~~~shell
# 用一个变量objecets来保存这些依赖
objects = main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o

# 用$(objects)引用变量获取它的的值，类似宏
edit : $(objects)
    cc -o edit $(objects)

# 这样写可以表示某些目标的依赖，多目标的共享依赖
$(objects) : defs.h
kbd.o command.o files.o : command.h
display.o insert.o search.o files.o : buffer.h

# 特殊指示符，明确声明clean是伪目标，
# 这样可以避免当目录下真的有一个文件叫clean时make去编译那个文件
.PHONY : clean
clean :
    -rm edit $(objects) # —可以忽略错误强制执行rm，防止某些文件无法删除时中断rm操作
~~~

### make查找的文件名

make命令会在当前目录下按顺序寻找文件名为 GNUmakefile 、 makefile 和 Makefile 的文件。在这三个文件名中，最好使用 Makefile 这个文件名，因为这个文件名在排序上靠近其它比较重要的文件，比如 README。最好不要用 GNUmakefile，因为这个文件名只能由GNU make ，其它版本的 make 无法识别，但是基本上来说，大多数的 make 都支持 makefile 和 Makefile 这两种默认文件名。

或者也可以自定义，比如：“Make.Solaris”，如果要指定特定的Makefile，使用make的 -f 或 --file 参数，如： make -f Make.Solaris。如果你使用多条 -f 或 --file 参数可以指定多个makefile。

### 通配符

用于代表不确定的字符或字符串，以实现对多个文件或对象的批量操作或模式匹配。

规则、变量、命令中都可以使用通配符

#### 常见通配符

* 星号（*）

代表零个或多个任意字符。例如，在文件系统中，*.txt 表示所有以 .txt 结尾的文件，无论文件名的前面部分是什么字符。如果当前目录下有 file1.txt、file2.txt、abc.txt 等文件，使用*.txt 就可以一次性指代所有这些文件。在命令行中，ls *.c 命令可以列出当前目录下所有扩展名为 .c 的文件。

* 问号（?）

代表单个任意字符。例如，file?.txt 可以匹配 file1.txt、file2.txt 等文件名，但不能匹配 file11.txt，因为 ? 只能代表一个字符。

* 方括号（[]）

用于指定一个字符集合，匹配其中的任意一个字符。例如，[abc].txt 可以匹配 a.txt、b.txt 和 c.txt。还可以使用范围表示法，如 [a-z].txt 表示匹配所有以小写字母开头并以 .txt 结尾的文件。
其他通配符

* 大括号（{}）

常用于生成一系列有规律的字符串或文件名组合。例如，file{1,2,3}.txt 等同于 file1.txt、file2.txt 和 file3.txt。它也可以用于嵌套，如 {a,b}{1,2} 会生成 a1、a2、b1、b2 等组合。

* 波浪号（~）

在某些系统中，~ 通常用于表示当前用户的主目录 。例如，cd ~ 会切换到当前用户的主目录。

## make的工作方式

1. 读入所有的Makefile。

1. 读入被include的其它Makefile。

1. 初始化文件中的变量。

1. 推导隐式规则，并分析所有规则。

1. 为所有的目标文件创建依赖关系链。

1. 根据依赖关系，决定哪些目标要重新生成。

1. 执行生成命令。

## make时的文件搜索

如果没有指定目录，make只会在当前目录下直接查找（不递归）搜索，可以使用特殊变量 `VPATH`来声明目录搜索的顺序例如

~~~makefile
VPATH = src:../headers
~~~

表示搜索目录是：当前目录、src、:/headers

另一个特殊变量`vpath`，可以按照模式来搜索，例如

~~~makefile
vpath <pattern> <directories>

vpath %.h ../headers
~~~

指定`.h`文件的搜索目录是当前、../headers

多条指令会按照`vpath`指令出现的先后顺序来确定搜索路径的优先级

## 命令

### 命令执行

~~~makefile
exec:
    cd /home/hchen
    pwd
~~~

这样不cd不起作用，因为每一行的命令是在独立的shell里执行，可以用分号连接来实现顺序执行

~~~makefile
exec:
    cd /home/hchen; pwd
~~~
