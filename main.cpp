#pragma once
#include <iostream>
#include "Satellites.h"
using namespace std;

int main()
{
	string path1 = "E:\\Ѹ������\\SCH200CAN_R_20231460000_01D_MN.rnx\\SCH200CAN_R_20231460000_01D_MN.rnx";	 // GPS ���������� PC �ϵ�·��
	string path2 = "E:\\Ѹ������\\brdc0560.22n\\results.csv";		// GPS λ�ü������洢��·��
	string path3 = "E:\\Ѹ������\\brdc0560.22n\\final_results.csv";
	tm jiesuan_date = {};//�����������
	jiesuan_date.tm_year = 2022 - 1900;
	jiesuan_date.tm_mon = 1;
	jiesuan_date.tm_mday = 25;
	jiesuan_date.tm_hour = 0;
	jiesuan_date.tm_min = 0;
	jiesuan_date.tm_sec = 0;

	Satellites s(path1, path2, path3, jiesuan_date);	// ʵ�����࣬�������ַ
	s.calData();						// ���ü�������λ�õĺ���
	system("pause");
	return 0;
}