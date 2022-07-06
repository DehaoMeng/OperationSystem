//
// Created by 86176 on 2022/5/11 0011.
//

#include "virtual_memory.h"
// 初始化页表操作
virtual_memory::virtual_memory() {
    int i = 0;
    k = 0;
    for(;i<MAXSIZE;i++) {
        int status, main_memory, where_in_disk,pagenum;
        cout << "请输入页号:" << endl;
        cin >> s[i].pagenum;
        cout << "请输入标志位:" << endl;
        cin >> s[i].sign;
        if (s[i].sign == 1) {
            cout << "请输入页框号:" << endl;
            cin >> s[i].main_memory;
            P[k] = s[i].pagenum;
            k = (k + 1) % MAXSIEZ_MAIN;
        } else {
            s[i].main_memory = 0;
        }
        cout << "请输入在磁盘上的位置:" << endl;
        cin >> s[i].where_in_disk;
        s[i].modify_bit = 0;
    }
}
// 地址转换
int virtual_memory::Find_absolute_address(Operation operators) {
    int i, absolute_address;
    if(operators.pagenumber >= MAXSIZE || operators.unit_number > 128){
    	cout << "页号或页框号输入错误，请重新输入" << endl;
    	return -1;
	}
    for (i = 0; i < MAXSIZE; ++i) {
        if (s[i].pagenum == operators.pagenumber) break;
    }
    if("存" == operators.operate){
    	s[i].modify_bit = 1;
	}
    absolute_address = s[i].main_memory * 128 + operators.unit_number;
    if (s[i].sign == 1) {
        cout << "绝对地址为: " << s[i].main_memory << " * 128 + "<< operators.unit_number << " = ";
        cout << absolute_address << endl;
    } else {
        FIFO(s[i].pagenum);
        cout << "* " << operators.pagenumber << endl;
    }
    return absolute_address;
}
// 展示页表
int virtual_memory::show() {
    cout << "页号\t" << "标志位\t" << "页框号\t" << "在磁盘上的位置\t" << "修改位\t" << endl;
    for (int i = 0; i < MAXSIZE; ++i) {
        cout << setiosflags(ios::left) << setw(8) << s[i].pagenum << setw(9) << s[i].sign << setw(8) << s[i].main_memory << setw(15) << s[i].where_in_disk << setw(8) << s[i].modify_bit
             << endl;
    }
    return 1;
}
// FIFO调度算法解决缺页中断
int virtual_memory::FIFO(int pagenum) {
    for (int i = 0; i < MAXSIZE; ++i) {
        if (s[i].pagenum == pagenum) {
            s[i].sign = 1;
            for (int j = 0; j < MAXSIEZ_MAIN; ++j) {
                if(P[k] == s[j].pagenum){
                    s[i].main_memory = s[j].main_memory;
                    s[j].main_memory = 0;
                    s[j].sign = 0;
                    if (s[j].modify_bit == 1){
                        s[j].modify_bit = 0;
                    }
                    cout << "调出的页面为: " << s[j].pagenum <<endl;
                    cout << "调入的页面为: " << s[i].pagenum << endl;
                    break;
                }
            }
            break;
        }
    }
    P[k] = pagenum;
    k = (k+1)%MAXSIZE;
    return 1;
}
