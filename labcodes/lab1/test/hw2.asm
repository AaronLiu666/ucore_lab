; 定义代码段
section.text
    global _start

_start:
    ; 设置段寄存器，将代码段寄存器cs的值赋给其他段寄存器
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00  ; 设置栈指针

    ; 使用INT 0x10的0x0E号功能逐个字符输出字符串
    mov ah, 0x0E  ; 功能号，0x0E表示在TTY模式下写字符到屏幕

    mov bx, 0x0000  ; 设置页号和颜色属性（这里页号为0，颜色属性暂不详细设置复杂值）

    mov al, 'H'  ; 要输出的第一个字符
    int 0x10  ; 调用BIOS中断输出字符

    mov al, 'e'
    int 0x10

    mov al, 'l'
    int 0x10

    mov al, 'l'
    int 0x10

    mov al, 'o'
    int 0x10

    mov al, ','
    int 0x10

    mov al, ' '
    int 0x10

    mov al, 'W'
    int 0x10

    mov al, 'o'
    int 0x10

    mov al, 'r'
    int 0x10

    mov al, 'l'
    int 0x10

    mov al, 'd'
    int 0x10

    mov al, '!'
    int 0x10

    ; 程序暂停，使用INT 0x16的0x00号功能等待键盘输入（这里只是简单让程序暂停，不处理输入内容）
    mov ah, 0x00
    int 0x16
    jmp $  ; 死循环，保持程序暂停状态

    ; times 510-($-$$) db 0  ; 用0填充剩余空间直到510字节
    ; db 0x55,0xAA  ; 添加主引导记录的结束标志，符合引导扇区格式要求