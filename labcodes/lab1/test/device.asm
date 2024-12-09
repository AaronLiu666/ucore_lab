section.text
    global _start

_start:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    ; 调用INT 0x11获取设备配置信息
    mov ah, 0x11
    int 0x11

    ; 设备配置信息返回在AX寄存器等，提取并打印相关信息
    ; 假设我们先打印是否有软盘驱动器，第7位表示软盘驱动器的存在
    test al, 0x80
    jz no_floppy
    mov ah, 0x0E
    mov al, 'Y'
    int 0x10
    jmp check_next
no_floppy:
    mov ah, 0x0E
    mov al, 'N'
    int 0x10
check_next:
    ; 这里可以继续提取其他设备信息并打印，如硬盘数量等
    ; （硬盘数量信息在其他位，具体位置根据BIOS规范）

    ; 程序暂停，等待键盘输入（可选）
    mov ah, 0x00
    int 0x16
    jmp $