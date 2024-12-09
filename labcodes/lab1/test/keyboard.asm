; 定义代码段，起始地址为0x7c00，符合主引导记录加载位置要求
section MBR vstart=0x7c00
    global _start

_start:
    ; 初始化段寄存器，将代码段寄存器cs的值赋给其他段寄存器
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov sp, 0x7c00

    ; 清屏操作，利用INT 0x10的0x06号功能
    mov ax, 0x0600
    mov bx, 0x0700
    mov cx, 0
    mov dx, 0x184f
    int 0x10

    ; 循环读取和输出键盘输入
    read_loop:
        ; 使用INT 0x16的0x00号功能读取键盘输入，等待按键按下
        mov ah, 0x00
        int 0x16

        ; 判断是否按下了回车键（ASCII码为0x0D），如果是则结束循环
        cmp al, 0x0D
        je end_loop

        ; 使用INT 0x10的0x0E号功能输出字符到屏幕（命令行）
        mov ah, 0x0E
        int 0x10

        ; 继续下一次循环读取键盘输入
        jmp read_loop

    end_loop:
        ; 程序结束，这里可以添加更多清理或返回相关操作（简单起见，暂时使用死循环暂停程序）
        jmp $

    ; 用0填充剩余空间直到510字节
    times 510-($-$$) db 0
    ; 添加主引导记录的结束标志，符合引导扇区格式要求
    db 0x55, 0xaa