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
    InitCurrent();//��ʼ����ǰλ��
    CountTable();//ͳ��IO�������������Ŀ
    while (command == 1) {
        cout << "������[0��1]�����ڵ�һ�������!!!" << endl;
        cin >> n;
        if (n > 0.5) {
            Scan();
        }
        else {
            Accept();
        }
        cout << "�����밴1,�˳��밴0" << endl;
        cin >> command;
    }
}

void Elevator_scheduling::Accept() {
    int flag = 1;
    while (flag == 1) {
        cout << "���룺������������š��ŵ��š������¼��" << endl;
        cin >> Table[tableNum].name >> Table[tableNum].cylinder >> Table[tableNum].magnetic >> Table[tableNum].record;
        tableNum++;
        cout << "�Ǽǳɹ��������Ǽ�����1����0" << endl;
        cin >> flag;
    }
}

int Elevator_scheduling::Scan() {
    int process;//ѡ��Ľ��̺�
    bool direction =1; //����0=out��1=in��Ĭ���Ʊ۷���Ϊ������
    print_io();
    if (tableNum == 0) {//�޵ȴ�������
        cout << "�޵ȴ������ߣ�����" << endl;
        return 0;
    }
    else{//�еȴ�������
        if (CylinderJudge() != -1) {//���뵱ǰ�������ͬ�ķ�����
            process = FindNearest();
        }
        else{//û���뵱ǰ�������ͬ�ķ�����
            if (direction == 1) {//�Ʊ۷���Ϊ����
                process = CylinderMax();
                if (process == -1) {//û�бȵ�ǰ����Ŵ�ķ�������
                    process = CylinderMin();
                    direction = 0;
                }
            }else {//�Ʊ۷���Ϊ����
                process = CylinderMin();
                if (process == -1) {//û�бȵ�ǰ�����С�ķ�������
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
    int differenceC = 200;//�����֮��
    int t = 8;//��С�����¼
    int k;//ѡ�������������
    int a;//ѡ�������������
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
    if (differenceC == 200) {//û�бȵ�ǰ�����С�ķ�������
        return -1;
    }
    else {
        return a;
    }
}

int Elevator_scheduling::CylinderMax() {
    int differenceC=200;//�����֮��
    int t = 8;//��С�����¼
    int k;//ѡ�������С�����
    int a;//ѡ�������������
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
    if (differenceC == 200) {//û�бȵ�ǰ����Ŵ�ķ�������
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
    cout << "------------------------ѡ�еĽ�����Ϣ------------------------" << endl;
    cout << "������:         �����:         �����¼��:         ����:" << endl;
    cout << setfill(' ') << setw(6) << current.name << setfill(' ') << setw(15) <<current.cylinder << setfill(' ') << setw(16) << current.record << setfill(' ') << setw(17) << directionStr << endl;
    cout << "--------------------------------------------------------------" << endl;
}

void Elevator_scheduling::print_io() {
    cout << "--------------------------����I/O��---------------------------" << endl;
    cout << "������:         �����:         �ŵ���:         �����¼��:" << endl;
    for (int i = 0; i < tableNum; i++) {
        cout << setfill(' ') << setw(6) << Table[i].name << setfill(' ') << setw(15) << Table[i].cylinder << setfill(' ') << setw(16) << Table[i].magnetic << setfill(' ') << setw(17) << Table[i].record << endl;
    }
    cout << "--------------------------------------------------------------" << endl;
}

int Elevator_scheduling::FindNearest() {
    int min = 8, k;//��С���룬��С����ı��
    int distance = 8;//��ǰ����
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
    current.name = "��ʼ����";
    current.cylinder = 0;
    current.magnetic = 0;
    current.record = 0;

    //�ٶ������Ѿ����ڵ�����
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