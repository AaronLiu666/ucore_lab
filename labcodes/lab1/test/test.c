#include <stdio.h>

int global_variable = 10;  // 定义一个全局变量，属于数据

void print_message() {
    printf("Hello, World!\n");
}

int main() {
    int local_variable = 5;  // 定义一个局部变量，也属于数据
    print_message();
    global_variable += local_variable;
    printf("The value of global_variable is now: %d\n", global_variable);
    return 0;
}