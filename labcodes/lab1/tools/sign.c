#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
/*
这段代码是一个 C 语言程序，其功能是创建一个512字节的启动扇区文件。程序执行以下步骤：

1. 检查命令行参数数量是否正确（需要恰好3个参数：程序名、输入文件名和输出文件名）。

```c
if (argc != 3) {
    fprintf(stderr, "Usage: <input filename> <output filename>\n");
    return -1;
}
```

2. 使用 `stat` 函数获取输入文件的状态信息，并检查是否成功。

```c
if (stat(argv[1], &st) != 0) {
    fprintf(stderr, "Error opening file '%s': %s\n", argv[1], strerror(errno));
    return -1;
}
```

3. 打印输入文件的大小，并检查文件大小是否超过510字节。

```c
printf("'%s' size: %lld bytes\n", argv[1], (long long)st.st_size);
if (st.st_size > 510) {
    fprintf(stderr, "%lld >> 510!!\n", (long long)st.st_size);
    return -1;
}
```

4. 定义一个512字节的缓冲区 `buf` 并使用 `memset` 函数将其初始化为0。

```c
char buf[512];
memset(buf, 0, sizeof(buf));
```

5. 以二进制读取模式打开输入文件，并使用 `fread` 函数读取文件内容到缓冲区 `buf`。

```c
FILE *ifp = fopen(argv[1], "rb");
int size = fread(buf, 1, st.st_size, ifp);
if (size != st.st_size) {
    fprintf(stderr, "read '%s' error, size is %d.\n", argv[1], size);
    return -1;
}
```

6. 关闭输入文件。

```c
fclose(ifp);
```

7. 在缓冲区的最后两个字节处设置启动扇区的结束标志（0x55AA）。

```c
buf[510] = 0x55;
buf[511] = 0xAA;
```

8. 以二进制写入模式打开输出文件，并使用 `fwrite` 函数将缓冲区 `buf` 的内容写入输出文件。

```c
FILE *ofp = fopen(argv[2], "wb+");
size = fwrite(buf, 1, 512, ofp);
if (size != 512) {
    fprintf(stderr, "write '%s' error, size is %d.\n", argv[2], size);
    return -1;
}
```

9. 关闭输出文件。

```c
fclose(ofp);
```

10. 打印成功消息，表示已成功创建512字节的启动扇区文件。

```c
printf("build 512 bytes boot sector: '%s' success!\n", argv[2]);
```

11. 程序正常退出，返回0。

```c
return 0;
```

这个程序的主要目的是创建一个512字节的启动扇区文件，它首先检查输入文件的大小是否合适，然后将文件内容读取到缓冲区，最后在缓冲区的最后两个字节处添加启动扇区的结束标志（0x55AA），并将这个缓冲区的内容写入到输出文件中。

*/

int
main(int argc, char *argv[]) {
    struct stat st;
    if (argc != 3) {
        fprintf(stderr, "Usage: <input filename> <output filename>\n");
        return -1;
    }
    if (stat(argv[1], &st) != 0) {
        fprintf(stderr, "Error opening file '%s': %s\n", argv[1], strerror(errno));
        return -1;
    }
    printf("'%s' size: %lld bytes\n", argv[1], (long long)st.st_size);
    if (st.st_size > 510) {
        fprintf(stderr, "%lld >> 510!!\n", (long long)st.st_size);
        return -1;
    }
    char buf[512];
    memset(buf, 0, sizeof(buf));
    FILE *ifp = fopen(argv[1], "rb");
    int size = fread(buf, 1, st.st_size, ifp);
    if (size != st.st_size) {
        fprintf(stderr, "read '%s' error, size is %d.\n", argv[1], size);
        return -1;
    }
    fclose(ifp);
    buf[510] = 0x55;
    buf[511] = 0xAA;
    FILE *ofp = fopen(argv[2], "wb+");
    size = fwrite(buf, 1, 512, ofp);
    if (size != 512) {
        fprintf(stderr, "write '%s' error, size is %d.\n", argv[2], size);
        return -1;
    }
    fclose(ofp);
    printf("build 512 bytes boot sector: '%s' success!\n", argv[2]);
    return 0;
}

