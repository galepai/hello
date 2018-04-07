
#include "Func.h"
#include <QTime>
#include <QSettings>
#include <vector>


void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName)
{
	QDateTime datetime;
	QSettings setting(file, QSettings::IniFormat);//���������ļ�
	//setting.setIniCodec(QTextCodec::codecForName("GB2312"));  //֧������ ������Ч
	setting.beginGroup(beginGroup);//beginGroup������endGroup ���Ӧ����config���Ǳ��
	setting.setValue(SetValueName, QVariant(datetime.currentDateTime().toString(Qt::ISODateWithMs)));
	setting.endGroup();
}

void WriteConfigure(const QString& file, const QString& beginGroup, const QString& SetValueName, const QString& Param)
{
	QSettings setting(file, QSettings::IniFormat);//���������ļ�
	//setting.setIniCodec(QTextCodec::codecForName("GB2312"));  //֧������ ������Ч
	setting.beginGroup(beginGroup);//beginGroup������endGroup ���Ӧ����config���Ǳ��
	setting.setValue(SetValueName, Param);
	setting.endGroup();
}

void ReadConfigure(const QString& file, const QString& beginGroup, const QString& GetValueName, QVariant& Value)
{
	QSettings setting(file, QSettings::IniFormat);//���������ļ�
	//setting.setIniCodec(QTextCodec::codecForName("GB2312"));  //֧������ ������Ч
	setting.beginGroup(beginGroup);//beginGroup������endGroup ���Ӧ����config���Ǳ��
	Value = setting.value(GetValueName);
	setting.endGroup();
}

//����̨��Ascii�ļ����뼰�����������ݣ�: + data + CR + \r\n���� ע�� (A-F)���봫���д
std::string Delta_Ascii_CR(const std::string& data)
{
	// "01020304FF05"  ~( 0X(01+02+03+04+EF+05) ) + 0x01;	�����ַ��øߵ��ֽڱ�ʾ1���ֽ�����ӣ�����EF����1���ֽڱ�ʾ��E��1110,F��1111������ֽ���11101111��
	unsigned char lr = 0x0;
	
	for (int index = 0; index < data.length() / 2; index++)
	{
		unsigned char high = data[index * 2];
		unsigned char low = data[index * 2 + 1];
		if (high<0x3a)
			high = (high - 0x30) << 4;
		else
			high = (high - 0x37) << 4;

		if (low<0x3a)
			low = low - 0x30;
		else
			low = low - 0x37;

		lr = high + low + lr;
	}
	lr = ~lr + 0x01;

	std::string cr = "ff";
	unsigned char high = lr >> 4;
	unsigned char low = lr & 0x0f;
	if (high < 0xa)		// С��10��Ascii���Ӧ��Hex
	{
		cr[0] = high + 0x30;	// ��Ӧ����ֵ(0-9)��Ascii�� �� ������Ӧ��Hex + 0x30(0��Ӧ��ASCII���Hex)�� ����: 2��Ӧ��ASCII �� 0x02+0x30��
	}
	else          // ������Ascii���Ӧ��Hex
	{
		cr[0] = high + 0x37;	// ��Ӧ����ĸ��A-F����Ascii�� = ����ĸ��Ӧ��Hex  + 0x37�� ����: d��Ӧ��ASCII �� 0xd+0x37��
	}

	if (low < 0xa)
	{
		cr[1] = low + 0x30;
	}
	else
	{
		cr[1] = low + 0x37;
	}

	std::string all_data(":");
	all_data += data + cr + "\r\n";

	return all_data;
}

std::string Delta_Ascii_CR(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& End_Address, const std::string& Other_Info)
{
	return Delta_Ascii_CR(Slave + Function_Code + Function_Code + Start_Address + End_Address + Other_Info);
}

//�������ܣ���δ����
std::vector<short> Parse_Delta_Ascii(const std::string& data)
{
	//data = ':00050520FF00D7\r\n';
	
	std::string temp;
	int len = data.length() / 2;
	std::vector<short> nums;
	if (data[0] == ':' && data[data.length() - 1] == '\n')
	{
		temp = data.substr(1, data.length() - 5);

		for (int index = 0; index < temp.length() / 2; index++)
		{
			unsigned char high = temp[index * 2];
			unsigned char low = temp[index * 2 + 1];
			if (high<0x3a)
				high = (high - 0x30) << 4;
			else
				high = (high - 0x37) << 4;

			if (low<0x3a)
				low = low - 0x30;
			else
				low = low - 0x37;

			nums.push_back(high + low);
		}
	}
	else
	{
		// ���ݲ��������������д�
	}
	
	return nums;
}
