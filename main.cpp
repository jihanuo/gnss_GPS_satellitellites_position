#pragma once
#include <iostream>
#include "Satellites.h"
using namespace std;
#include <filesystem>//需使用c++17及更新标准

int main()
{
	//传入卫星广播星历和解算时间参数
	//string path1 = "..//资源文件//220225data.rnx";	 
	FILE* fp;
	std::string Data_relativePath = "\\data\\220225.rnx";//源数据
	std::string currentPath = std::filesystem::current_path().string();

	// 使用 C++17 的文件系统库拼接路径
	std::string path1 = std::filesystem::path(currentPath).string() + Data_relativePath;// GPS 卫星星历在 PC 上的路径


	string path2 = std::filesystem::path(currentPath).string() + "\\data\\data.csv";		// 提取的数据
	string path3 = std::filesystem::path(currentPath).string() + "\\data\\final_results.csv";//最终解算出的每隔15分钟数据
	int year = 2022;//在此更改要解算的年月日，为格林尼治标准时间
	int month = 2;
	int day = 25;
	int shiqucha = 8;//这是本地时间减去格里尼治标准时间小时差，不同时区的用户需修改此值，此值为北京时间




	//以下代码无需修改
	tm jiesuan_date = { 0 };//定义解算日期,传入此处为北京时间，因此要加上8，转换为以utc时间整天 
	jiesuan_date.tm_year = year - 1900;
	jiesuan_date.tm_mon = month - 1;
	jiesuan_date.tm_mday = day;
	jiesuan_date.tm_hour = shiqucha;
	jiesuan_date.tm_min = 0;
	jiesuan_date.tm_sec = 0;
	Satellites s(path1, path2, path3, jiesuan_date);	// 实例化类，并传入地址
	s.calData();						// 调用计算卫星位置的函数
	system("pause");
	return 0;
}