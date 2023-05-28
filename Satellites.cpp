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



// c = 'w' �򷵻� gps �ܣ� c = 's' �򷵻� gpa �����룬Ĭ�Ϸ���������

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


//���������ÿ��15���ӵ����Բ�ֵ
void Satellites::lglrchazhi(string path2,string path3)
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
		newRow.push_back(data[i][0]); // ��һ��
		newRow.push_back(data[i][1]); // �ڶ���
		newRow.push_back(data[i][17]); // ��18��
		newRow.push_back(data[i][18]); // ��19��
		newRow.push_back(data[i][19]); // ��20��
		Weixing_jiesuan.push_back(newRow);
	}

	// �����������Բ�ֵ
	double x, y;
	vector<vector<string>> WX_cz;     //

	for (int i = 2; i < Weixing_jiesuan.size(); i++) {
		string name1 = Weixing_jiesuan[i - 1][0], name2 = Weixing_jiesuan[i][0];
		//cout << "weixingming1 " << name1 << "weixingming2 " << name2 << endl;
		if (name1==name2)
		{//ͬһ�����ǲŽ��в�ֵ����
			double x1 = stod(Weixing_jiesuan[i - 1][1]), x2 = stod(Weixing_jiesuan[i][1]);//���Բ�ֵ���
			double X1 = stod(Weixing_jiesuan[i - 1][2]), X2 = stod(Weixing_jiesuan[i][2]);//��ü������ֵ
			double Y1 = stod(Weixing_jiesuan[i - 1][3]), Y2 = stod(Weixing_jiesuan[i][3]);
			double Z1 = stod(Weixing_jiesuan[i - 1][4]), Z2 = stod(Weixing_jiesuan[i][4]);
			for (double j = x1; j < x2; j += 900) { // ÿ��900����в�ֵ
				double k = (j - x1) / (x2 - x1);
				double X = X1 * (1 - k) + X2 * k;   //��ֵ����
				double Y = Y1 * (1 - k) + Y2 * k;
				double Z = Z1 * (1 - k) + Z2 * k;
				//cout << name1 << " shijian" << j << " zuobiao  " << X << " " << Y << " " << endl;

				//����GPS��ת�������մ洢�����ӿɶ���

				vector<string> newRow2;
				newRow2.push_back(name1);
				cout << "time " << gpsSeconds2Time(j)<<endl;
				newRow2.push_back(gpsSeconds2Time(j));
				newRow2.push_back(to_string(j));
				newRow2.push_back(to_string(X));
				newRow2.push_back(to_string(Y));
				newRow2.push_back(to_string(Z));
				WX_cz.push_back(newRow2);
				i++;

			}
		}

	}
		//����������ļ�
		ofstream out(path3);
		out << "prn" << "," <<"time"<<"," << "gpstime" << "," << "X" << "," << "Y" << "," << "Z" << endl;
		for (auto row : WX_cz) {
			for (auto cell : row) {
				out << cell << ",";
			}
			out << endl;
		}
		out.close();

		cout << "�ѽ������Ľ��������"<<path3<<" �ļ��С�" << endl;

		//for (const auto& row : WX_cz) {
		//	for (const auto& col : row) {
		//		cout << col << " ";
		//	}
		//	cout << endl;
		//}

		//

}
