//
// Created by 86176 on 2022/4/21 0021.
//
#include <string>

#ifndef PROJECT_PV_H
#define PROJECT_PV_H
using namespace std;

int P(int s);       // P原语
int V(int s);       // V原语
void Put(char c);   // 生产者生产的产品放入商店
char Get();         // 消费者从商店获取产品
char produce();     // 生产者生产产品
void consume(char c);// 消费者消费产品
void GOTO(int L);     // 执行PC寄存器的指令
void NOP();           // 空指令
void init();          // 初始化程序
void schedule();      // 处理器调度程序
void pcDoing();       // 处理器指令调度程序
void showPCB(int i);  // 输出PCB

#endif //PROJECT_PV_H
