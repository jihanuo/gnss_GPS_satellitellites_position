#pragma once
#include <iostream>
#include "Satellites.h"
using namespace std;

int main()
{
	string path1 = "E:\\Ѹ������\\SCH200CAN_R_20231460000_01D_MN.rnx\\SCH200CAN_R_20231460000_01D_MN.rnx";	 // GPS ���������� PC �ϵ�·��
	string path2 = "E:\\Ѹ������\\brdc0560.22n\\results.csv";		// GPS λ�ü������洢��·��
	string path3 = "E:\\Ѹ������\\brdc0560.22n\\final_results.csv";
	Satellites s(path1, path2,path3);	// ʵ�����࣬�������ַ
	s.calData();						// ���ü�������λ�õĺ���
	system("pause");
	return 0;
}