//
// Created by 86176 on 2022/5/23 0023.
//

#ifndef VIRTUAL_MEMORYDEMO_CPP_ELEVATOR_SCHEDULING_H
#define VIRTUAL_MEMORYDEMO_CPP_ELEVATOR_SCHEDULING_H

#include <iostream>
#include<string>
using namespace std;
class Elevator_scheduling {
private:
    typedef struct request {//请求I/O表
        string name;//进程名
        int cylinder;//柱面号
        int magnetic;//磁道号
        int record;//物理记录号
    }Request;
    int tableNum = 0; // 当前盘号
    Request Table[100]; // 创建一张100大小的请求I/O表
    Request current;    // 当前进程
    void InitCurrent(); // 初始化进程
    int FindNearest(); // 查找最小距离的编号
    void print_io();    // 打印IO表
    void PrintProcess(bool direction);// 输出进程
    int CylinderJudge();    // 判断是否有与当前柱面号相同的访问者
    int CylinderMax();      // 查找比当前柱面号大的访问请求
    int CylinderMin();      // 查找比当前柱面号小的访问请求
    void PopProcess(int process); // 弹出进程
    int Scan();             // 移臂
    void Accept();          // 登记进程
    void CountTable();      // 记录盘号
public:
    Elevator_scheduling();
    ~Elevator_scheduling(){}
};


#endif //VIRTUAL_MEMORYDEMO_CPP_ELEVATOR_SCHEDULING_H
