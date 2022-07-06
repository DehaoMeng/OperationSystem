//
// Created by 86176 on 2022/5/11 0011.
//

#include "virtual_memory.cpp"
#include <iostream>
#include <string>
using namespace std;
// 输入指令操作
int input_operation(Operation &operators){
	cout << "请依次输入操作名、页号、字块号" << endl;
	cin >> operators.operate >> operators.pagenumber >> operators.unit_number;
    return 0;
}
// 测试函数
int main(){
    virtual_memory virtualMemory;
    virtualMemory.show();
    Operation operators;
    while(1){
	    input_operation(operators);
	    virtualMemory.Find_absolute_address(operators);
	    virtualMemory.show();
	}
    return 0;
}