//
// Created by 86176 on 2022/5/11 0011.
//

#include "virtual_memory.cpp"
#include <iostream>
#include <string>
using namespace std;
// ����ָ�����
int input_operation(Operation &operators){
	cout << "�����������������ҳ�š��ֿ��" << endl;
	cin >> operators.operate >> operators.pagenumber >> operators.unit_number;
    return 0;
}
// ���Ժ���
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