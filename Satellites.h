#pragma once
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

class Satellites
{
public:
	Satellites(string path, string path2, string path3, tm jiesuan_date);
	~Satellites();
	void calData();
	static int ydcount(int year);					// 获取每一年的天数
	static int ydcount(int year, int month);		// 获取每一个月的天数
	long double getgpst(char c);					// 计算 GPS 周或 周内秒
	void wdata();						// 输出计算结果
	string gpsSeconds2Time(long long gpsSeconds);//将GPStime 转化为年月日

private:
	long double n;//n 平均角速度
	long double tk;//归化时间
	long double mk;//卫星平近点角
	long double ek; //偏近点角
	long double vk;//真近点角
	long double pa;//升交距角
	long double cu;//摄动改正项升交距角
	long double cr;//摄动改正项卫星向径
	long double ci;//摄动改正项卫星轨道倾角
	long double uk;//改正后升交距角
	long double rk;//改正后卫星向径
	long double ik;//改正后卫星轨道倾角
	long double xk;//平面直角坐标系中的坐标x
	long double yk;//平面直角坐标系中的坐标y
	long double dk;//升交点经度计算
	long double Xk;//地心坐标系x
	long double Yk;//地心坐标系y
	long double Zk;//地心坐标系z
	long double gpstime;	//gps时间

	ifstream inFile;
	ofstream oFile;

	string prn;	// 卫星号
	int year, month, day, hour, min, second;	// 年，月，日，时，分，秒
	long double af0;	// 卫星钟差
	long double af1;	// 卫星钟速
	long double af2;	// 卫星钟速变率

	long double IODE;	// 数据龄期
	long double Crs;	// 轨道半径正弦调和项改正的振幅
	long double a_poor;	// 平均角速度之差
	long double m0;		// 平近点角

	long double Cuc;	// 纬度幅角的余弦调和项改正的振幅
	long double e;		// 轨道偏心率
	long double Cus;	// 纬度幅角的余弦调和项改正的振幅
	long double sqrtA;	// 轨道长半径的平方根

	long double toe;	// 轨道参数的参考时间
	long double Cic;	// 轨道倾角的余弦调和项改正的振幅
	long double Ra0;	// 升交点赤经与本周起始时刻格林尼治恒星时之差
	long double Cis;	// 轨道倾角的正弦调和项改正的振幅

	long double i0;		// 轨道倾角
	long double Crc;	// 轨道半径的余弦调和项改正的振幅
	long double w;		// 近地点角距
	long double Ra;		// 升焦点赤经变化率

	long double i;		// 倾角变化率
	long double L2;		// L2上的码
	long double g_week;	// GPS 周数
	long double L2P;	// L2上的P码

	long double acc;	// 卫星精度
	long double state;	// 健康状态
	long double Tgd;	// 电离层时延迟差
	long double IDOC;	// 星钟的数据龄期
	long double s_time;	// 电文发送时刻
	long double f_val;	// 拟合区间
	void lglrchazhi(string path2, string path3, tm jiesuan_date);//对卫星坐标数据进行插值处理
	double lagrangeInterpolation(const vector<double>& x, const vector<double>& y, double xi);//拉格朗日插值算法
	vector<pair<int, int>> groupByWx_name(const vector<vector<string>>& data);//将得到的数据进行分组
	vector<long int> chazhi_gpstime(struct tm&, int jiange_miao = 900);//根据给出的日期和间隔时间生成插值gps周内秒，默认间隔时间为900秒（15分钟）

};
