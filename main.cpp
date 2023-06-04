#pragma once
#include <iostream>
#include "Satellites.h"
using namespace std;

int main()
{
	string path1 = "E:\\迅雷下载\\SCH200CAN_R_20231460000_01D_MN.rnx\\SCH200CAN_R_20231460000_01D_MN.rnx";	 // GPS 卫星星历在 PC 上的路径
	string path2 = "E:\\迅雷下载\\brdc0560.22n\\results.csv";		// GPS 位置计算结果存储的路径
	string path3 = "E:\\迅雷下载\\brdc0560.22n\\final_results.csv";
	tm jiesuan_date = {};//定义解算日期
	jiesuan_date.tm_year = 2022 - 1900;
	jiesuan_date.tm_mon = 1;
	jiesuan_date.tm_mday = 25;
	jiesuan_date.tm_hour = 0;
	jiesuan_date.tm_min = 0;
	jiesuan_date.tm_sec = 0;

	Satellites s(path1, path2, path3, jiesuan_date);	// 实例化类，并传入地址
	s.calData();						// 调用计算卫星位置的函数
	system("pause");
	return 0;
}