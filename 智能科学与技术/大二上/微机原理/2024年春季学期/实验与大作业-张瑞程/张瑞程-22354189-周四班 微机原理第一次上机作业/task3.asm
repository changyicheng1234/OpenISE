DATA SEGMENT
    X DB 16H,35H,18H,12H,11H,5FH,98H,06H,10H,05H,36H,99H,0E1H,0CDH,20H,0F1H
    Y DB 23H,40H,29H,2CH,02H,8EH,2FH,03H,4CH,06H,57H,9BH,63H,FDH,05H,AAH 
DATA ENDS

CODE SEGMENT
    ASSUME CS:CODE, DS:DATA

START:
    MOV AX, DATA
    MOV DS, AX
    ; 将 SI 寄存器加载到 X 数组的地址
    LEA SI, X
    ; 将 DI 寄存器加载到 Y 数组的地址
    LEA DI, Y
    ; 将 BX 寄存器加载为 2000H，这是存储结果的起始地址
    MOV BX, 2000H
    MOV CX, 16   

STORE:
    MOV AL, DS:[SI]
    ADD AL, DS:[DI]
    CMP AL, 40H
    ; 如果 AL 大于或等于 40H，跳转到 NEXT 标签（不存储 AL 的值）
    JAE NEXT
    ; 否则，将 AL 的值存储到 BX 指向的内存地址
    MOV DS:[BX], AL
    ; BX 寄存器递增，指向下一个存储位置
    INC BX
NEXT:
    ; SI 寄存器递增，指向 X 数组的下一个元素
    INC SI
    ; DI 寄存器递增，指向 Y 数组的下一个元素
    INC DI
    LOOP STORE

CODE ENDS
END START