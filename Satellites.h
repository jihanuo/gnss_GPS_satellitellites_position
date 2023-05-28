#pragma once
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

class Satellites
{
public:
	Satellites(string path, string path2, string path3);
	~Satellites();
	void calData();
	static int ydcount(int year);					// ��ȡÿһ�������
	static int ydcount(int year, int month);		// ��ȡÿһ���µ�����
	long double getgpst(char c);					// ���� GPS �ܻ� ������
	void wdata();						// ���������
	void lglrchazhi(string path2, string path3);
	long double gpsTime(int year, int month, int day, int hour, int minute, int second);//���õ���ʱ��ת��Ϊ	GPS��
	string gpsSeconds2Time(long long gpsSeconds);//��GPStime ת��Ϊ������

private:
	long double n;//n ƽ�����ٶ�
	long double tk;//�黯ʱ��
	long double mk;//����ƽ�����
	long double ek; //ƫ�����
	long double vk;//������
	long double pa;//�������
	long double cu;//�㶯�������������
	long double cr;//�㶯������������
	long double ci;//�㶯���������ǹ�����
	long double uk;//�������������
	long double rk;//������������
	long double ik;//���������ǹ�����
    long double xk;//ƽ��ֱ������ϵ�е�����x
	long double yk;//ƽ��ֱ������ϵ�е�����y
	long double dk;//�����㾭�ȼ���
	long double Xk;//��������ϵx
	long double Yk;//��������ϵy
	long double Zk;//��������ϵz
	long double gpstime;	//gpsʱ��

	ifstream inFile;
	ofstream oFile;

	string prn;	// ���Ǻ�
	int year, month, day, hour, min, second;	// �꣬�£��գ�ʱ���֣���
	long double af0;	// �����Ӳ�
	long double af1;	// ��������
	long double af2;	// �������ٱ���

	long double IODE;	// ��������
	long double Crs;	// ����뾶���ҵ�������������
	long double a_poor;	// ƽ�����ٶ�֮��
	long double m0;		// ƽ�����

	long double Cuc;	// γ�ȷ��ǵ����ҵ�������������
	long double e;		// ���ƫ����
	long double Cus;	// γ�ȷ��ǵ����ҵ�������������
	long double sqrtA;	// ������뾶��ƽ����

	long double toe;	// ��������Ĳο�ʱ��
	long double Cic;	// �����ǵ����ҵ�������������
	long double Ra0;	// ������ྭ�뱾����ʼʱ�̸������κ���ʱ֮��
	long double Cis;	// �����ǵ����ҵ�������������

	long double i0;		// ������
	long double Crc;	// ����뾶�����ҵ�������������
	long double w;		// ���ص�Ǿ�
	long double Ra;		// ������ྭ�仯��

	long double i;		// ��Ǳ仯��
	long double L2;		// L2�ϵ���
	long double g_week;	// GPS ����
	long double L2P;	// L2�ϵ�P��

	long double acc;	// ���Ǿ���
	long double state;	// ����״̬
	long double Tgd;	// �����ʱ�ӳٲ�
	long double IDOC;	// ���ӵ���������
	long double s_time;	// ���ķ���ʱ��
	long double f_val;	// �������
};
