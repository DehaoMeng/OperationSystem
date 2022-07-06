//
// Created by 86176 on 2022/4/14 0014.
//

#ifndef PROJECT_OPERATING_SYSTEM_H
#define PROJECT_OPERATING_SYSTEM_H
#include<string>
using namespace std;
class Operating_System {
private:
    // 创建进程模块
    typedef struct Process{
        string Process_name;
        int Operation_hours=0;
        int Required_runtime;
        int Priority_number;
        char Status='R';
        struct Process *next;
    }Process;
    Process *process;
    double time=0;
    bool Set_hours();     // 为每个进程计算设置等待时间
    void Print(); // 打印所有进程
    bool Delete_Process();// 要求运行时间为0 状态位标为“结束”，并且从表中删除该进程。
public:
    Operating_System();   // 构造函数
    ~Operating_System(){} // 析构函数
    bool Add_Process();   // 创建新进程将其加入到操作系统中
    bool Running();       // 运行当前优先数最大的的一个进程，即第一个进程
};

#endif //PROJECT_OPERATING_SYSTEM_H
