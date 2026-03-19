# 自动获取当前目录下所有 .c 文件
SRCS := $(wildcard *.c)

# 将所有 .c 文件名转换为不带后缀的可执行文件名
OUTS := $(patsubst %.c,%,$(SRCS))

# 编译器
CC := gcc

# 编译选项
CFLAGS := -Wall -g

# 声明伪目标
.PHONY: all clean rebuild

# 默认目标：生成所有可执行程序
all: $(OUTS)

# 模式规则：每个 .c 文件生成同名可执行程序
%: %.c
	$(CC) $(CFLAGS) $< -o $@

# 清理生成的可执行程序
clean:
	$(RM) $(OUTS)

# 重建
rebuild: clean all
