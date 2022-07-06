//
// Created by 86176 on 2022/5/23 0023.
//

#include "Elevator_scheduling.h"
#include <iostream>
#include<string>
#include<iomanip>
#include<cmath>
using namespace std;
Elevator_scheduling::Elevator_scheduling() {
    float n;
    int command=1;
    InitCurrent();//初始化当前位置
    CountTable();//统计IO请求表中请求数目
    while (command == 1) {
        cout << "输入在[0，1]区间内的一个随机数!!!" << endl;
        cin >> n;
        if (n > 0.5) {
            Scan();
        }
        else {
            Accept();
        }
        cout << "继续请按1,退出请按0" << endl;
        cin >> command;
    }
}

void Elevator_scheduling::Accept() {
    int flag = 1;
    while (flag == 1) {
        cout << "输入：进程名、柱面号、磁道号、物理记录号" << endl;
        cin >> Table[tableNum].name >> Table[tableNum].cylinder >> Table[tableNum].magnetic >> Table[tableNum].record;
        tableNum++;
        cout << "登记成功！继续登记请求按1否则按0" << endl;
        cin >> flag;
    }
}

int Elevator_scheduling::Scan() {
    int process;//选择的进程号
    bool direction =1; //方向0=out，1=in，默认移臂方向为向里移
    print_io();
    if (tableNum == 0) {//无等待访问者
        cout << "无等待访问者！！！" << endl;
        return 0;
    }
    else{//有等待访问者
        if (CylinderJudge() != -1) {//有与当前柱面号相同的访问者
            process = FindNearest();
        }
        else{//没有与当前柱面号相同的访问者
            if (direction == 1) {//移臂方向为向里
                process = CylinderMax();
                if (process == -1) {//没有比当前柱面号大的访问请求
                    process = CylinderMin();
                    direction = 0;
                }
            }else {//移臂方向为向外
                process = CylinderMin();
                if (process == -1) {//没有比当前柱面号小的访问请求
                    process = CylinderMax();
                    direction = 1;
                }
            }
        }
        current = Table[process];
        PrintProcess(direction);
        PopProcess(process);
    }
    return 1;
}

void Elevator_scheduling::PopProcess(int process) {
    for (int i = process; i < tableNum; i++) {
        Table[i] = Table[i + 1];
    }
    tableNum--;
}

int Elevator_scheduling::CylinderMin() {
    int differenceC = 200;//柱面号之差
    int t = 8;//最小物理记录
    int k;//选择出的最大柱面号
    int a;//选择出的最佳请求号
    for (int i = 0; i < tableNum; i++) {
        if (abs(Table[i].cylinder - current.cylinder) < differenceC && Table[i].cylinder < current.cylinder) {
            differenceC = abs(Table[i].cylinder - current.cylinder);
        }
        k = current.cylinder - differenceC;
    }
    for (int i = 0; i < tableNum; i++) {
        if (Table[i].cylinder == k && Table[i].record < t) {
            t = Table[i].record;
            a = i;
        }
    }
    if (differenceC == 200) {//没有比当前柱面号小的访问请求
        return -1;
    }
    else {
        return a;
    }
}

int Elevator_scheduling::CylinderMax() {
    int differenceC=200;//柱面号之差
    int t = 8;//最小物理记录
    int k;//选择出的最小柱面号
    int a;//选择出的最佳请求号
    for (int i = 0; i < tableNum; i++) {
        if (abs(Table[i].cylinder - current.cylinder) < differenceC && Table[i].cylinder > current.cylinder) {
            differenceC = Table[i].cylinder - current.cylinder;
        }
        k = current.cylinder + differenceC;
    }
    for (int i = 0; i < tableNum; i++) {
        if (Table[i].cylinder == k && Table[i].record < t) {
            t = Table[i].record;
            a = i;
        }
    }
    if (differenceC == 200) {//没有比当前柱面号大的访问请求
        return -1;
    }
    else {
        return a;
    }
}

int Elevator_scheduling::CylinderJudge() {
    for (int i = 0; i < tableNum; i++) {
        if (Table[i].cylinder == current.cylinder)
            return i;
    }
    return -1;
}

void Elevator_scheduling::PrintProcess(bool direction) {
    string directionStr;
    if (direction == 1) {
        directionStr = "up";
    }else {
        directionStr = "down";
    }
    cout << "------------------------选中的进程信息------------------------" << endl;
    cout << "进程名:         柱面号:         物理记录号:         方向:" << endl;
    cout << setfill(' ') << setw(6) << current.name << setfill(' ') << setw(15) <<current.cylinder << setfill(' ') << setw(16) << current.record << setfill(' ') << setw(17) << directionStr << endl;
    cout << "--------------------------------------------------------------" << endl;
}

void Elevator_scheduling::print_io() {
    cout << "--------------------------请求I/O表---------------------------" << endl;
    cout << "进程名:         柱面号:         磁道号:         物理记录号:" << endl;
    for (int i = 0; i < tableNum; i++) {
        cout << setfill(' ') << setw(6) << Table[i].name << setfill(' ') << setw(15) << Table[i].cylinder << setfill(' ') << setw(16) << Table[i].magnetic << setfill(' ') << setw(17) << Table[i].record << endl;
    }
    cout << "--------------------------------------------------------------" << endl;
}

int Elevator_scheduling::FindNearest() {
    int min = 8, k;//最小距离，最小距离的编号
    int distance = 8;//当前距离
    for (int i = 0; i < tableNum; i++) {
        if (Table[i].cylinder == current.cylinder) {
            distance = abs(Table[i].record - current.record);
            if (distance < min) {
                min = distance;
                k = i;
            }
        }
    }
    return k;
}

void Elevator_scheduling::CountTable() {
    int i=0;
    while (Table[i].name != ""){
        i++;
    }
    tableNum = i;
}

void Elevator_scheduling::InitCurrent() {
    current.name = "初始进程";
    current.cylinder = 0;
    current.magnetic = 0;
    current.record = 0;

    //假定表中已经存在的请求
    Table[0].name = "A";
    Table[0].cylinder = 67;
    Table[0].magnetic = 1;
    Table[0].record = 6;
    Table[1].name = "B";
    Table[1].cylinder = 31;
    Table[1].magnetic = 15;
    Table[1].record =  1;
    Table[2].name = "C";
    Table[2].cylinder = 16;
    Table[2].magnetic = 3;
    Table[2].record = 5;
    Table[3].name = "D";
    Table[3].cylinder = 88;
    Table[3].magnetic = 4;
    Table[3].record = 2;
    Table[4].name = "E";
    Table[4].cylinder = 127;
    Table[4].magnetic = 17;
    Table[4].record = 4;
}