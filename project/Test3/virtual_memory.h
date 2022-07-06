//
// Created by 86176 on 2022/5/11 0011.
//

#ifndef PV_CPP_VIRTUAL_MEMORY_H
#define PV_CPP_VIRTUAL_MEMORY_H
#include <iostream>
#include <iomanip>
using namespace std;
typedef struct Virtual{     // 页表结构
    int pagenum;            // 页号
    int sign;               // 标志位，表示是否存入主存中
    int main_memory;        // 在主存的位置
    int where_in_disk;      // 在磁盘中的位置
    int modify_bit;     // 修改位默认为0 如果发生存操作则变为1
}Virtual;
typedef struct Operation{   // 指令
    string operate;         // 指令操作
    int pagenumber;         // 操作页号
    int unit_number;        // 操作字块号
}Operation;
const int MAXSIZE = 7;      // 页表长度为 7
const int MAXSIEZ_MAIN = 4;
class virtual_memory {      // 分页式虚拟存储系统
private:
    Virtual s[MAXSIZE];     // 分页式虚拟存储系统页表
    int P[MAXSIEZ_MAIN];
    int k;
public:
    virtual_memory();       // 初始化页表操作
    int Find_absolute_address(Operation operators);     // 地址转换
    int show();             // 展示页表
    int FIFO(int pagenum);             // FIFO调度算法解决缺页中断
};


#endif //PV_CPP_VIRTUAL_MEMORY_H
