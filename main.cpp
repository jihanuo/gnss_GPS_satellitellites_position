#pragma once
#include <iostream>
#include "Satellites.h"
using namespace std;
#include <filesystem>//需使用c++17及更新标准

int main()
{
	//string path1 = "..//资源文件//220225data.rnx";	 
	FILE* fp;
	std::string Data_relativePath = "\\data\\SCH200CAN_R_20231460000_01D_MN.rnx";
	std::string currentPath = std::filesystem::current_path().string();

	// 使用 C++17 的文件系统库拼接路径
	std::string path1 = std::filesystem::path(currentPath).string() + Data_relativePath;// GPS 卫星星历在 PC 上的路径

	//cout << path1 << endl;

	string path2 = std::filesystem::path(currentPath).string() + "\\data\\results.csv";		// GPS 位置计算结果存储的路径
	string path3 = std::filesystem::path(currentPath).string() + "\\data\\final_results.csv";
	tm jiesuan_date = {};//定义解算日期
	jiesuan_date.tm_year = 2022 - 1900;
	jiesuan_date.tm_mon = 2;
	jiesuan_date.tm_mday = 25;
	jiesuan_date.tm_hour = 0;
	jiesuan_date.tm_min = 0;
	jiesuan_date.tm_sec = 0;

	Satellites s(path1, path2, path3, jiesuan_date);	// 实例化类，并传入地址
	s.calData();						// 调用计算卫星位置的函数
	system("pause");
	return 0;
}