DATA SEGMENT
    ; 定义一个名为 CHAR 的数组，它有 26 个重复的字节，初始值都为 0
    CHAR DB 26 DUP(0)
DATA ENDS

CODE SEGMENT
	ASSUME CS:CODE, DS:DATA

START:
    ; 将 DI 寄存器加载到 CHAR 数组的地址
    LEA DI, CHAR
    ; 将 AL 寄存器加载为字符 'A' 的 ASCII 码
    MOV AL, 'A'
    ; 将 CX 寄存器加载为 26，CX 用作循环计数器
    MOV CX, 26
STORE:
    ; 将 AL 寄存器的值存储到 DI 指向的内存地址，即 CHAR 数组中
    MOV [DI], AL
    ; 将 DI 寄存器增加 1，指向下一个字符位置
    INC DI
    ; 将 AL 寄存器增加 1，ASCII 码递增，即字符递增
    INC AL
    LOOP STORE
CODE ENDS
END START