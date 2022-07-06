//
// Created by 86176 on 2022/4/14 0014.
//
#include <iostream>
#include "Operating_System.cpp"
using namespace std;

int main(){
    Operating_System process;
    for (int  i = 0; i < 5;i++) {
        process.Add_Process();
    }
    process.Running();
    return 0;
}