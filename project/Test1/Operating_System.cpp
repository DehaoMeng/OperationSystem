//
// Created by 86176 on 2022/4/14 0014.
//

#include "Operating_System.h"
#include <iostream>
#include <climits>
#include <string>
using namespace std;
Operating_System::Operating_System() {
    // 初始默认构造函数
    process = new Process;  // 为头结点分配空间
    process->next = nullptr; // 将后继节点指向空指针
}

bool Operating_System::Add_Process() {
    // 创建一个新进程  并将其加入到系统内部
    Process *s = process;
    auto *p = new Process;
    string name;
    int times;
    int number;
    cout << "Please input Process_name" << endl;
    cin >> name;
    cout << "Please input Required_times" << endl;
    cin >> times;
    cout << "Please input Priority_number" << endl;
    cin >> number;
    p->Process_name = name;
    p->Required_runtime = times;
    p->Priority_number = number;
    p->next = s->next;
    s->next = p;
    cout << "OK" << endl;
    return true;
}
// 每次运行设置每个进程的等待时间
bool Operating_System::Set_hours() {
    // 计算每次等待时间
    auto p = process->next; // 游标指针 设置此指针指向的进程的等待时间
    while (p != nullptr){
        auto s = process->next; // 与游标指针比较的其他进程的指针
        int hours = 0; // 初始等待时间为0
        while (s != nullptr){
            if (s == p){
                s = s->next;
                continue;
            }
            // 循环遍历进程链表，求出 p指针所需的等待时间
            if (s->Priority_number >= p->Priority_number && s->Status == 'R') {
                for (int i = 0; i <= s->Priority_number - p->Priority_number; i++) {
                    hours += s->Required_runtime - i;
                }
            }
            s = s->next;
        }
        p->Operation_hours = hours;
        if (p->Status == 'E'){
            p->Operation_hours = 0;
        }
        p = p->next;
    }
    return true;
}
// 将进程从队列中”删除“
bool Operating_System::Delete_Process() {
    if(process->next == nullptr) return false;
    auto pre = process;
    auto cur = process->next;
    while(cur != nullptr) {
        if (cur->Required_runtime == 0) {
            // 将所需时间为0 的进程的状态设置为 E 。
            cur->Status = 'E';
        }
        cur = cur->next;
        pre = pre->next;
    }
    return true;
}
// 运行进程
bool Operating_System::Running() {
    Set_hours(); // 遍历设置队列等待时间
    auto p= process->next;
    auto s = new Process;
    s->Priority_number = INT_MIN;
    // 寻找本次所需执行的最高优先数的指针
    while (p != nullptr){
        if(p->Status == 'E'){
            p = p->next;
            continue;
        }
        if(p->Priority_number >= s->Priority_number){
            s = p;
        }
        p = p->next;
    }
    // 所有的进程都执行完毕后 结束递归调用
    if (s->Required_runtime == 0) {
        cout << "The current processor is idle ! " << endl;
        Print();
        return false;
    }
    // 每次执行进程  将其时间和优先数各减一
    s->Priority_number--;
    s->Required_runtime--;
    cout << "The currently running process is named :" << s->Process_name << endl;
    cout << " The running time of the currently running process is :" << s->Required_runtime << endl;
    cout << " The priority number of the currently running process is :" << s->Priority_number << endl;
    // 输出所有进程的状态信息
    Print();
    // 检查并结束 不在需要执行的进程
    Delete_Process();
    // 调用自身 继续执行
    Running();
    return true;
}
// 输出所有进程状态
void Operating_System::Print() {
    auto p = process;
    while (p->next){
        cout << "Process name : " << p->next->Process_name << " Waiting time :" << p->next->Operation_hours << endl;
        cout << "Process Status : " << p->next->Status << endl;
        p = p->next;
    }
}