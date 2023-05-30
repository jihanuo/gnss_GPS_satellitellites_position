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
#define U 3.986005e+14	// WGS-84 �е�����������
#define E 7.29211567e-5 // ������ת����


Satellites::Satellites(string path1, string path2, string path3)
{
	vector<string> prn;
	vector<vector<long double>> vt;								// ��ά��̬����
	vector<long double> vi;										// һά��̬����
	this->inFile.open(path1, ios::in);									// ���ļ�
	this->oFile.open(path2, ios::out | ios::trunc);
	//��ȡ�ļ�����

	if (this->inFile && this->oFile)													// ���ļ��򿪳ɹ�������������	
	{	
		cout << "��ʼ��ȡ����" << endl;
		string s;												// ��Ŷ�ȡ��ÿһ���ַ���
		smatch m;												// �������ƥ�䵽��Ԫ��
		regex r("G\\d\\d.*|-?\\d.\\d{12}e[-+]\\d\\d");			// ����ƥ��ÿ�����ǵ�Ԫ
		regex r1("\\d{4}|\\d\\d|-?\\d\\.\\d{12}e[+-]\\d\\d");
		int i = 0;
		while (getline(this->inFile, s))								// ���ļ�
		{

			if (++i == 13220) break;
			if (regex_search(s, m, r))							// ƥ������Ϊ True ����
				if (regex_search(s, m, regex("G\\d\\d")))		// ���������
				{
					//cout << s.substr(0, 3) ;
					prn.push_back(s.substr(0, 3));				// ������ prn ����
					vt.push_back(vi);							// ��֮ǰ�ı�����Ϊһ������ vt	
					vi.clear();									// ���vi��Ϊ��һ�α�����׼��
					for (sregex_iterator it(s.begin() + 3, s.end(), r1), end_it; it != end_it; ++it)	// sregex_iterator �����������
						vi.push_back(stold(it->str()));			// ��ƥ���ÿһ������תΪ������������vi
				}
				else
					for (sregex_iterator it(s.begin(), s.end(), r1),
						end_it; it != end_it; ++it) vi.push_back(stold(it->str()));	// ��ƥ���ÿһ������תΪ������������vi
						
		}
		vt.push_back(vi);	// �����һ�α�����ֵ���� vt
		// д���ĵ�������Ŀ
		this->oFile << "prn"<<","<<"gpstime"<< ","<<"n" << "," << "tk" << "," << "mk" << "," << "ek" << "," << "vk" << "," << "pa" << "," << "cu" << "," <<
			"cr" << "," << "ci" << "," << "uk" << "," << "rk" << "," << "ik" << "," << "xk" << "," << "yk" << "," << "dk" <<
			"," << "Xk" << "," << "Yk" << "," << "Zk" << endl;

	}
	else cout << "���ļ�ʧ�ܣ���ر� Excel �����ԣ�" << endl;	// ���ļ���ʧ������ʾ

	for (int i = 1; i < vt.size(); i++)	// �����ݴ洢�ڱ���
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

		this->calData();		// ��������
		this->wdata();			// �洢�м�����
	}
		this->lglrchazhi(path2,path3);

}

// �����������ر��ļ�
Satellites::~Satellites()
{
	this->inFile.close();		// �ر��ļ�
	this->oFile.close();				// �ر��ļ�
}

// �������ݺ���
void Satellites::calData()
{
	this->n = sqrt(U) / pow(this->sqrtA, 3) + this->a_poor; //	����ƽ�����ٶ� n
	//cout << "gpst  " << this->getgpst('s');
	//cout << "toe " << this->toe;
	this->gpstime = this->getgpst('s');
	this->tk = this->getgpst('s') - this->toe;	//	����黯ʱ�� tk
	//cout << "tkshijain  " << this->tk<<endl;
	this->mk = this->m0 + (this->n * this->tk);				// �۲�ʱ�� mk
	this->ek = this->mk;									// ����ƫ����
	long double temp = 0;
	while (fabs(this->ek - temp) > 0.10e-12)
	{
		temp = this->ek;
		this->ek = this->mk + (this->e * sin(temp));
	}

//(1)  ���ƽ������ϵ����������
	this->vk = atan((sqrt(1 - pow(this->e, 2)) * sin(this->ek) / (cos(this->ek) - this->e)));			// ��������� vk
	this->pa = this->vk + this->w;																		// �����������
	this->cu = (this->Cuc * cos(2 * this->pa)) + (this->Cus * sin(2 * this->pa));						// �����㶯������
	this->cr = (this->Crc * cos(2 * this->pa)) + (this->Crs * sin(2 * this->pa));
	this->ci = (this->Cic * cos(2 * this->pa)) + (this->Cis * sin(2 * this->pa));
	this->uk = this->pa + this->cu;																		// ���㾭���㶯�����Ĳ���
	this->rk = pow(this->sqrtA, 2) * (1 - this->e * cos(this->ek)) + this->cr;
	this->ik = this->i0 + this->ci + (this->i * this->tk);
	this->xk = this->rk * cos(this->uk);																// ���ƽ��ֱ������ϵ�е�����
	this->yk = this->rk * sin(this->uk);

//(2)���ƽ������ϵת�����ĵع�ϵ����ϵ 

	this->dk = this->Ra0 + ((this->Ra - E) * this->tk) - (this->Ra * this->toe);//��ʽ�޸�								// �����㾭�ȼ���
	this->Xk = this->xk * cos(this->dk) - (this->yk * cos(this->ik) * sin(this->dk));					// �����ڵ��Ĺ̶�����ϵ�е�ֱ������
	this->Yk = this->xk * sin(this->dk) + (this->yk * cos(this->ik) * sin(this->dk));
	this->Zk = this->yk * sin(this->ik);
}


// ������д���ļ�
void Satellites::wdata()
{
	//cout << this->prn << endl;
	this->oFile<< this->prn<< "," << this->gpstime << "," << this->n << "," << this->tk << "," << this->mk << "," << this->ek << ","
		<< this->vk << "," << this->pa << "," << this->cu << "," << this->cr <<
		"," << this->ci << "," << this->uk << "," << this->rk << "," << this->ik
		<< "," << this->xk << "," << this->yk << "," << this->dk << "," << this->Xk <<
		"," << this->Yk << "," << this->Zk << endl;
}


// ����ÿһ�������
int Satellites::ydcount(int year)
{
	int count = 0;
	if (year == 1980)	count = 360;													// ���� 1980 ��Ļ��������㷨 360 ��
	else if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))	count = 366;	// �������꣬�� 1 ���� 366 ��
	else	count = 365;																// ��ƽ���꣬�� 1 ���� 365 ��
	return count;
}

// ����һ����ÿ���µ�����
int Satellites::ydcount(int year, int month)
{
	int count = 0;																			// �洢ÿ���µ�����
	if (year == 1980 && month == 1) count = 25;												// �����㷨��1980 �� 1 ������ 25 ��
	else if (month == 4 || month == 6 || month == 9 || month == 11) count = 30;				// �����Ŷ� 30 ��
	else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) count = 31;
	else
	{
		if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))	count = 29;			// �������꣬�� 2 ���� 29 ��
		else count = 28;
	}
	return count;

}

// c = 'w' �򷵻� gps �ܣ� c = 's' �򷵻� gpa �����룬Ĭ�Ϸ���������
long double Satellites::getgpst(char c = 's')
{
	if (c != 'w' && c != 's') return -1;											// �����ݵĲ�������ȷ���򷵻�-1
	int days = this->day;
	for (int i = 1980; i < this->year; i++)	days += ydcount(i);						// ����������
	for (int i = 1; i < this->month; i++)	days += ydcount(this->year, i);			// ����������
	return c == 'w' ? days / 7 : (days % 7) * 86400 + (this->hour * 3600) + (this->min * 60) + this->second;
}





string Satellites:: gpsSeconds2Time(long long gpsSeconds)
{
	//cout << "getw " << this->getgpst('s')<<endl;
	time_t utcTime = gpsSeconds+this->getgpst('w')* 604800+ 315964800; // ת��Ϊutc
	struct tm utcTm = { 0 };
	gmtime_s(&utcTm, &utcTime); // ��UTCʱ��ת��Ϊ�ɶ�ʱ��

	char buffer[20];
	sprintf_s(buffer, "%04d-%02d-%02d %02d:%02d:%02d", utcTm.tm_year + 1900, utcTm.tm_mon + 1, utcTm.tm_mday,
		utcTm.tm_hour, utcTm.tm_min, utcTm.tm_sec);

	string timeStr(buffer);
	return timeStr;
}

vector<long int> Satellites::chazhi_gpstime(struct tm& jiesuan_date,int seconds) {
	vector<long int> gpstime;//
	time_t utc_time;
	utc_time = mktime(&jiesuan_date);
	cout << "utc " << utc_time << endl;

	for (int i = 0; i < (86400 / seconds); i++) {
		gpstime.push_back((utc_time - 315964800) % 604800);
		utc_time += seconds;
	}
	for (const auto& col : gpstime) {
   		cout << col << " ";
		}	return gpstime;
}


// �Զ���Ϊ�ָ�������һ�����ݽ���Ϊһ��vector<string>
vector<string> parseLine(const string& line) {
	vector<string> result;
	stringstream ss(line);
	string cell;
	while (getline(ss, cell, ',')) {
		result.push_back(cell);
	}
	return result;
}

// ��ȡCSV�ļ�������һ����άvector<string>
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

vector<pair<int, int>> Satellites:: groupByWx_name(const vector<vector<string>>& data) {
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




double Satellites:: lagrangeInterpolation(const vector<double>& x, const vector<double>& y, double xi) {
	if (x.size() != y.size()) {
		cout << "x �� y �ĳ��Ȳ���ȣ�" << endl;
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

//���������ÿ��15���ӵ����Բ�ֵ
void Satellites::lglrchazhi(string path2, string path3)
{
	vector<vector<string>> data = readCSV(path2);//������֪����   dataΪ��������
	//for (auto row : data) {
	//	for (auto cell : row) {
	//		cout << cell << " ";
	//	}
	//	cout << endl;
	//}
	vector<vector<string>> Weixing_jiesuan;     //ɸѡ��Ҫ�������
	for (int i = 1; i < data.size(); i++) { // �ӵڶ��п�ʼ����   
		vector<string> newRow;
		newRow.push_back(data[i][0]); // prn
		newRow.push_back(data[i][1]); // gps������
		newRow.push_back(data[i][17]); // x
		newRow.push_back(data[i][18]); // y
		newRow.push_back(data[i][19]); // z
		Weixing_jiesuan.push_back(newRow);
	}

	// �����������Բ�ֵ
	vector<vector<string>> WX_cz;     //��ֵ������

	tm jiesuan_date = {};//�����������
	jiesuan_date.tm_year = 2022-1900;
	jiesuan_date.tm_mon = 1;
	jiesuan_date.tm_mday = 25;
	jiesuan_date.tm_hour = 0;
	jiesuan_date.tm_min = 0;
	jiesuan_date.tm_sec = 0;
	time_t utc_time;
	utc_time = mktime(&jiesuan_date);
	cout << "utc " << utc_time << endl;

	vector<long int> chazhi_time = this->chazhi_gpstime(jiesuan_date,900);//��ý�������


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
			suanzhi.push_back(Weixing_jiesuan[quyu[i].first][0]);//��������
			suanzhi.push_back(gpsSeconds2Time(chazhi_time[k]));
			suanzhi.push_back(to_string(chazhi_time[k]));//gpsshijian
			suanzhi.push_back(to_string(lagrangeInterpolation(X, Yx, chazhi_time[k])));//��������
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
		cout << "�ѽ������Ľ��������" << path3 << " �ļ��С�" << endl;

	}
