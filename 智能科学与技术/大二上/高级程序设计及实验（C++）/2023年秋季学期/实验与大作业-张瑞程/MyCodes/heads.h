#pragma once

///要放在头文件上面
//通过预编译去掉unicode字符集
#ifdef  UNICODE
#undef UNICODE
#endif //  UNICODE
//包含所有头文件，避免冲突
#include <easyx.h>
#include <iostream>
#include <string>
#include <vector> 
#include <graphics.h>
#include <time.h>
#include <conio.h>//接收键盘信息
#include <fstream>
#include <list>


