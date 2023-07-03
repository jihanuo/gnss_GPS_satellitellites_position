#pragma once
#include <iostream>
#include <regex>
#include <cctype>
#include "Satellites.h"
#include<iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include<string>
#include <ctime>

using namespace std;
#define U 3.986005e+14	// WGS-84 中地球引力常数
#define E 7.29211567e-5 // 地球自转速率


Satellites::Satellites(string path1, string path2, string path3, tm jiesuan_date)
{
	vector<string> prn;
	vector<vector<long double>> vt;								// 二维动态数组
	vector<long double> vi;										// 一维动态数组
	this->inFile.open(path1, ios::in);									// 打开文件
	this->oFile.open(path2, ios::out | ios::trunc);
	//读取文件数据

	if (this->inFile && this->oFile)													// 若文件打开成功则继续下面操作	
	{
		cout << "读取广播星历数据···" << endl;
		string s;												// 存放读取的每一行字符串
		smatch m;												// 存放正则匹配到的元素
		regex r("G\\d\\d.*|-?\\d.\\d{12}e[-+]\\d\\d");			// 正则匹配每个卫星单元
		regex r1("\\d{4}|\\d\\d|-?\\d\\.\\d{12}e[+-]\\d\\d");
		int i = 0;
		while (getline(this->inFile, s))								// 打开文件
		{

			if (++i == 13220) break;
			if (regex_search(s, m, r))							// 匹配条件为 True 进入
				if (regex_search(s, m, regex("G\\d\\d")))		// 首行则进入
				{
					//cout << s.substr(0, 3) ;
					prn.push_back(s.substr(0, 3));				// 将卫星 prn 推入
					vt.push_back(vi);							// 将之前的遍历作为一个推入 vt	
					vi.clear();									// 清空vi，为下一次遍历做准备
					for (sregex_iterator it(s.begin() + 3, s.end(), r1), end_it; it != end_it; ++it)	// sregex_iterator 正则迭代作用
						vi.push_back(stold(it->str()));			// 将匹配的每一个参数转为浮点型再推入vi
				}
				else
					for (sregex_iterator it(s.begin(), s.end(), r1),
						end_it; it != end_it; ++it) vi.push_back(stold(it->str()));	// 将匹配的每一个参数转为浮点型再推入vi

		}
		vt.push_back(vi);	// 将最后一次遍历的值推入 vt
		// 写入文档首行条目
		this->oFile << "prn" << "," << "gpstime" << "," << "n" << "," << "tk" << "," << "mk" << "," << "ek" << "," << "vk" << "," << "pa" << "," << "cu" << "," <<
			"cr" << "," << "ci" << "," << "uk" << "," << "rk" << "," << "ik" << "," << "xk" << "," << "yk" << "," << "dk" <<
			"," << "Xk" << "," << "Yk" << "," << "Zk" << endl;

	}
	else cout << "打开文件失败！请关闭 Excel 后重试！" << endl;	// 若文件打开失败则提示

	for (int i = 1; i < vt.size(); i++)	// 将数据存储在变量
	{
		//cout << i << endl;
		this->prn = prn[i - 1];
		this->year = vt[i][0];
		this->month = vt[i][1];
		this->day = vt[i][2];
		this->hour = vt[i][3];
		this->min = vt[i][4];
		this->second = vt[i][5];
		this->af0 = vt[i][6];
		this->af1 = vt[i][7];
		this->af2 = vt[i][8];
		/*
		cout << "shijain" << endl;
		cout<<this->year<<"  ";
		cout<<this->month<<" ";
		cout<<this->day <<" ";
		cout<<this->hour<<" ";
		cout<<this->min<<" ";
		cout<<this->second<<endl;
		*/
		this->IODE = vt[i][9];
		this->Crs = vt[i][10];
		this->a_poor = vt[i][11];
		this->m0 = vt[i][12];

		this->Cuc = vt[i][13];
		this->e = vt[i][14];
		this->Cus = vt[i][15];
		this->sqrtA = vt[i][16];

		this->toe = vt[i][17];
		this->Cic = vt[i][18];
		this->Ra0 = vt[i][19];
		this->Cis = vt[i][20];

		this->i0 = vt[i][21];
		this->Crc = vt[i][22];
		this->w = vt[i][23];
		this->Ra = vt[i][24];

		this->i = vt[i][25];
		this->L2 = vt[i][26];
		this->g_week = vt[i][27];
		this->L2P = vt[i][28];

		this->acc = vt[i][29];
		this->state = vt[i][30];
		this->Tgd = vt[i][31];
		this->IDOC = vt[i][32];

		//this->s_time = vt[i][33];
		//this->f_val = vt[i][34];

		this->calData();		// 计算数据
		this->wdata();			// 存储中间数据
	}
	cout << "正在计算数据，请稍等" << endl;
	this->lglrchazhi(path2, path3, jiesuan_date);

}

// 析构函数，关闭文件
Satellites::~Satellites()
{
	this->inFile.close();		// 关闭文件
	this->oFile.close();				// 关闭文件
}

// 计算数据函数
void Satellites::calData()
{
	this->n = sqrt(U) / pow(this->sqrtA, 3) + this->a_poor; //	计算平均角速度 n
	//cout << "gpst  " << this->getgpst('s');
	//cout << "toe " << this->toe;
	this->gpstime = this->getgpst('s');
	this->tk = this->getgpst('s') - this->toe;	//	计算归化时间 tk
	//cout << "tkshijain  " << this->tk<<endl;
	this->mk = this->m0 + (this->n * this->tk);				// 观测时间 mk
	this->ek = this->mk;									// 计算偏近角
	long double temp = 0;
	while (fabs(this->ek - temp) > 0.10e-12)
	{
		temp = this->ek;
		this->ek = this->mk + (this->e * sin(temp));
	}

	//(1)  轨道平面坐标系下卫星坐标
	this->vk = atan((sqrt(1 - pow(this->e, 2)) * sin(this->ek) / (cos(this->ek) - this->e)));			// 计算真近角 vk
	this->pa = this->vk + this->w;																		// 计算升交距角
	this->cu = (this->Cuc * cos(2 * this->pa)) + (this->Cus * sin(2 * this->pa));						// 计算摄动改正项
	this->cr = (this->Crc * cos(2 * this->pa)) + (this->Crs * sin(2 * this->pa));
	this->ci = (this->Cic * cos(2 * this->pa)) + (this->Cis * sin(2 * this->pa));
	this->uk = this->pa + this->cu;																		// 计算经过摄动改正的参数
	this->rk = pow(this->sqrtA, 2) * (1 - this->e * cos(this->ek)) + this->cr;
	this->ik = this->i0 + this->ci + (this->i * this->tk);
	this->xk = this->rk * cos(this->uk);																// 轨道平面直角坐标系中的坐标
	this->yk = this->rk * sin(this->uk);

	//(2)轨道平面坐标系转换地心地固系坐标系 

	this->dk = this->Ra0 + ((this->Ra - E) * this->tk) - (this->Ra * this->toe);//公式修改								// 升交点经度计算
	this->Xk = this->xk * cos(this->dk) - (this->yk * cos(this->ik) * sin(this->dk));					// 计算在地心固定坐标系中的直角坐标
	this->Yk = this->xk * sin(this->dk) + (this->yk * cos(this->ik) * sin(this->dk));
	this->Zk = this->yk * sin(this->ik);
}


// 将数据写入文件
void Satellites::wdata()
{
	//cout << this->prn << endl;
	this->oFile << this->prn << "," << this->gpstime << "," << this->n << "," << this->tk << "," << this->mk << "," << this->ek << ","
		<< this->vk << "," << this->pa << "," << this->cu << "," << this->cr <<
		"," << this->ci << "," << this->uk << "," << this->rk << "," << this->ik
		<< "," << this->xk << "," << this->yk << "," << this->dk << "," << this->Xk <<
		"," << this->Yk << "," << this->Zk << endl;
}


// 计算每一年的天数
int Satellites::ydcount(int year)
{
	int count = 0;
	if (year == 1980)	count = 360;													// 若是 1980 年的话，按照算法 360 天
	else if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))	count = 366;	// 若是闰年，则 1 年有 366 天
	else	count = 365;																// 若平闰年，则 1 年有 365 天
	return count;
}

// 计算一年中每个月的天数
int Satellites::ydcount(int year, int month)
{
	int count = 0;																			// 存储每个月的天数
	if (year == 1980 && month == 1) count = 25;												// 根据算法，1980 年 1 月算作 25 天
	else if (month == 4 || month == 6 || month == 9 || month == 11) count = 30;				// 四六九冬 30 整
	else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) count = 31;
	else
	{
		if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))	count = 29;			// 若是闰年，则 2 月有 29 天
		else count = 28;
	}
	return count;

}

// c = 'w' 则返回 gps 周， c = 's' 则返回 gpa 周内秒，默认返回周内秒
long double Satellites::getgpst(char c = 's')
{
	if (c != 'w' && c != 's') return -1;											// 若传递的参数不正确，则返回-1
	int days = this->day;
	for (int i = 1980; i < this->year; i++)	days += ydcount(i);						// 计算年天数
	for (int i = 1; i < this->month; i++)	days += ydcount(this->year, i);			// 计算月天数
	return c == 'w' ? days / 7 : (days % 7) * 86400 + (this->hour * 3600) + (this->min * 60) + this->second;
}



string Satellites::gpsSeconds2Time(long long gpsSeconds)
{
	//cout << "getw " << this->getgpst('s')<<endl;
	//cout << gpsSeconds << " gpssceond" << endl;
	time_t utcTime = gpsSeconds + this->getgpst('w') * 604800 + 315964800; // 转化为utc
	struct tm utcTm = { 0 };
	gmtime_s(&utcTm, &utcTime); // 将UTC时间转化为可读时间

	char buffer[20];
	sprintf_s(buffer, "%04d-%02d-%02d %02d:%02d:%02d", utcTm.tm_year + 1900, utcTm.tm_mon + 1, utcTm.tm_mday,
		utcTm.tm_hour, utcTm.tm_min, utcTm.tm_sec);

	string timeStr(buffer);
	//cout << buffer << endl;
	return timeStr;
}

vector<long int> Satellites::chazhi_gpstime(struct tm& jiesuan_date, int seconds) {
	vector<long int> gpstime;//
	time_t utc_time;
	utc_time = mktime(&jiesuan_date);//此处使用的时间结构为地方时
	//cout << "utc " << utc_time << endl;

	for (int i = 0; i < (86400 / seconds); i++) {
		gpstime.push_back((utc_time - 315964800) % 604800);

		//cout << (utc_time - 315964800) % 604800 << "  ";
		utc_time += seconds;
	}
	for (const auto& col : gpstime) {
		//cout << col << " ";
	}	return gpstime;
}


// 以逗号为分隔符，将一行数据解析为一个vector<string>
vector<string> parseLine(const string& line) {
	vector<string> result;
	stringstream ss(line);
	string cell;
	while (getline(ss, cell, ',')) {
		result.push_back(cell);
	}
	return result;
}

// 读取CSV文件，返回一个二维vector<string>
vector<vector<string>> readCSV(const string& filename) {
	vector<vector<string>> result;
	ifstream file(filename);
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			result.push_back(parseLine(line));
		}
		file.close();
	}
	return result;
}

vector<pair<int, int>> Satellites::groupByWx_name(const vector<vector<string>>& data) {
	vector<vector<string>> sortedData = data;
	sort(sortedData.begin(), sortedData.end(), [](const vector<string>& a, const vector<string>& b) { return a[0] < b[0]; });

	string currentKey = "";
	int start = 0, end = 0;
	vector<pair<int, int>> result;

	for (int i = 0; i < sortedData.size(); ++i) {
		if (sortedData[i][0] != currentKey) {
			if (!currentKey.empty()) {
				result.push_back(make_pair(start, end - 1));
			}
			currentKey = sortedData[i][0];
			start = i;
		}
		end = i + 1;
	}

	if (!currentKey.empty()) {
		result.push_back(make_pair(start, end - 1));
	}

	return result;
}




double Satellites::lagrangeInterpolation(const vector<double>& x, const vector<double>& y, double xi) {
	if (x.size() != y.size()) {
		cout << "x 和 y 的长度不相等！" << endl;
		return NAN;
	}

	int n = x.size();
	double yi = 0.0;

	for (int i = 0; i < n; ++i) {
		double li = 1.0;

		for (int j = 0; j < n; ++j) {
			if (i != j) {
				li *= (xi - x[j]) / (x[i] - x[j]);
			}
		}

		yi += y[i] * li;
	}

	return yi;
};

//对坐标进行每隔15分钟的线性插值
void Satellites::lglrchazhi(string path2, string path3, tm jiesuan_date)
{
	vector<vector<string>> data = readCSV(path2);//读入已知数据   data为所有数据
	//for (auto row : data) {
	//	for (auto cell : row) {
	//		cout << cell << " ";
	//	}
	//	cout << endl;
	//}
	vector<vector<string>> Weixing_jiesuan;     //筛选出要算的数据
	for (int i = 1; i < data.size(); i++) { // 从第二行开始遍历   
		vector<string> newRow;
		newRow.push_back(data[i][0]); // prn
		newRow.push_back(data[i][1]); // gps周内秒
		newRow.push_back(data[i][17]); // x
		newRow.push_back(data[i][18]); // y
		newRow.push_back(data[i][19]); // z
		Weixing_jiesuan.push_back(newRow);
	}


	// 拉格朗日线性插值
	vector<vector<string>> WX_cz;     //插值后数据
	time_t utc_time;
	utc_time = mktime(&jiesuan_date);
	//cout << "utc " << utc_time << endl;

	vector<long int> chazhi_time = this->chazhi_gpstime(jiesuan_date, 900);//获得解算区间


	vector<pair<int, int>> quyu = groupByWx_name(Weixing_jiesuan);
	for (int i = 0; i < quyu.size(); i++) {
		vector<double> X;
		vector<double> Yx;
		vector<double> Yy;
		vector<double> Yz;

		for (int j = 0; j < quyu[i].second - quyu[i].first; j++) {
			X.push_back(stod(Weixing_jiesuan[quyu[i].first + j][1]));
			Yx.push_back(stod(Weixing_jiesuan[quyu[i].first + j][2]));
			Yy.push_back(stod(Weixing_jiesuan[quyu[i].first + j][3]));
			Yz.push_back(stod(Weixing_jiesuan[quyu[i].first + j][4]));
		}
		for (int k = 0; k < chazhi_time.size(); k++) {
			vector<string>suanzhi;
			suanzhi.push_back(Weixing_jiesuan[quyu[i].first][0]);//卫星名称
			suanzhi.push_back(gpsSeconds2Time(chazhi_time[k]));
			suanzhi.push_back(to_string(chazhi_time[k]));//gpsshijian
			suanzhi.push_back(to_string(lagrangeInterpolation(X, Yx, chazhi_time[k])));//三个坐标
			suanzhi.push_back(to_string(lagrangeInterpolation(X, Yy, chazhi_time[k])));
			suanzhi.push_back(to_string(lagrangeInterpolation(X, Yz, chazhi_time[k])));
			WX_cz.push_back(suanzhi);
		}



		ofstream out(path3);
		out << "prn" << "," << "time" << "," << "gpstime" << "," << "X" << "," << "Y" << "," << "Z" << endl;
		for (auto row : WX_cz) {
			for (auto cell : row) {
				out << cell << ",";
			}
			out << endl;
		}
		out.close();


		//for (const auto& row : WX_cz) {
		//	for (const auto& col : row) {
		//		cout << col << " ";
		//	}
		//	cout << endl;
		//}
	}
	cout << "已将排序后的结果保存在" << path3 << " 文件中。" << endl;

}
