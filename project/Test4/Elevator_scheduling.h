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
    typedef struct request {//����I/O��
        string name;//������
        int cylinder;//�����
        int magnetic;//�ŵ���
        int record;//�����¼��
    }Request;
    int tableNum = 0; // ��ǰ�̺�
    Request Table[100]; // ����һ��100��С������I/O��
    Request current;    // ��ǰ����
    void InitCurrent(); // ��ʼ������
    int FindNearest(); // ������С����ı��
    void print_io();    // ��ӡIO��
    void PrintProcess(bool direction);// �������
    int CylinderJudge();    // �ж��Ƿ����뵱ǰ�������ͬ�ķ�����
    int CylinderMax();      // ���ұȵ�ǰ����Ŵ�ķ�������
    int CylinderMin();      // ���ұȵ�ǰ�����С�ķ�������
    void PopProcess(int process); // ��������
    int Scan();             // �Ʊ�
    void Accept();          // �Ǽǽ���
    void CountTable();      // ��¼�̺�
public:
    Elevator_scheduling();
    ~Elevator_scheduling(){}
};


#endif //VIRTUAL_MEMORYDEMO_CPP_ELEVATOR_SCHEDULING_H
