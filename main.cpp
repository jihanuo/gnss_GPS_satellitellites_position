#pragma once
#include <iostream>
#include "Satellites.h"
using namespace std;

int main()
{
	string path1 = "E:\\迅雷下载\\SCH200CAN_R_20231460000_01D_MN.rnx\\SCH200CAN_R_20231460000_01D_MN.rnx";	 // GPS 卫星星历在 PC 上的路径
	string path2 = "E:\\迅雷下载\\brdc0560.22n\\results.csv";		// GPS 位置计算结果存储的路径
	string path3 = "E:\\迅雷下载\\brdc0560.22n\\final_results.csv";
	Satellites s(path1, path2,path3);	// 实例化类，并传入地址
	s.calData();						// 调用计算卫星位置的函数
	system("pause");
	return 0;
}