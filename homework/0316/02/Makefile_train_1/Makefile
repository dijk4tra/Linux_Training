# 编译器(注意，Makefile的注释要用 #!)
CC = gcc

# 编译选项
# -Wall: 开启所有警告，帮助排查潜在的代码风险
# -g: 生成调试信息，方便使用 gdb 进行断点调试
CFLAGS = -Wall -g

# 最终可执行程序名
TARGET = main

# 源文件列表
SRCS = main.c add.c subtract.c

# 由源文件自动替换生成目标文件列表
# 例如：main.c add.c subtract.c -> main.o add.o subtract.o
OBJS = $(SRCS:.c=.o)

# 默认目标
# 直接输入 'make' 时执行的第一个目标。它依赖于 $(TARGET)
# 直接执行 make 时，默认生成 main
all: $(TARGET)

# 链接规则：把所有 .o 文件链接成最终可执行程序
# $(TARGET): 目标名称
# $(OBJS):   依赖列表
# $@: 自动化变量，代表当前规则的目标文件名（即 main）
# $^: 自动化变量，代表当前规则的所有依赖文件（即所有 .o 文件）
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 头文件列表 (Header Files)
# 明确依赖关系，当 .h 文件内容改变时，关联的 .c 会重新编译
HEADERS = calc.h
main.o: main.c $(HEADERS)
add.o: add.c $(HEADERS)
subtract.o: subtract.c $(HEADERS)

# 通用编译规则：把 .c 编译成 .o
# $< 表示第一个依赖文件
# $@ 表示目标文件名
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# clean：删除编译过程中产生的文件
clean:
	rm -f $(OBJS) $(TARGET)

# rebuild：先清理，再重新编译
rebuild: clean all

# 声明伪目标，避免与同名文件冲突
.PHONY: all clean rebuild
