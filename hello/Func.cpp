
#include "Func.h"
#include <QTime>
#include <QSettings>
#include <vector>


void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName)
{
	QDateTime datetime;
	QSettings setting(file, QSettings::IniFormat);//生成配置文件
	//setting.setIniCodec(QTextCodec::codecForName("GB2312"));  //支持中文 测试无效
	setting.beginGroup(beginGroup);//beginGroup与下面endGroup 相对应，“config”是标记
	setting.setValue(SetValueName, QVariant(datetime.currentDateTime().toString(Qt::ISODateWithMs)));
	setting.endGroup();
}

void WriteConfigure(const QString& file, const QString& beginGroup, const QString& SetValueName, const QString& Param)
{
	QSettings setting(file, QSettings::IniFormat);//生成配置文件
	//setting.setIniCodec(QTextCodec::codecForName("GB2312"));  //支持中文 测试无效
	setting.beginGroup(beginGroup);//beginGroup与下面endGroup 相对应，“config”是标记
	setting.setValue(SetValueName, Param);
	setting.endGroup();
}

void ReadConfigure(const QString& file, const QString& beginGroup, const QString& GetValueName, QVariant& Value)
{
	QSettings setting(file, QSettings::IniFormat);//生成配置文件
	//setting.setIniCodec(QTextCodec::codecForName("GB2312"));  //支持中文 测试无效
	setting.beginGroup(beginGroup);//beginGroup与下面endGroup 相对应，“config”是标记
	Value = setting.value(GetValueName);
	setting.endGroup();
}

//生成台达Ascii的检验码及生成完整数据（: + data + CR + \r\n）； 注意 (A-F)必须传入大写
std::string Delta_Ascii_CR(const std::string& data)
{
	// "01020304FF05"  ~( 0X(01+02+03+04+EF+05) ) + 0x01;	两两字符用高低字节表示1个字节再相加，比如EF，用1个字节表示，E是1110,F是1111，组成字节是11101111。
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
	if (high < 0xa)		// 小于10的Ascii码对应的Hex
	{
		cr[0] = high + 0x30;	// 对应的数值(0-9)的Ascii码 ＝ 该数对应的Hex + 0x30(0对应的ASCII码的Hex)； 例如: 2对应的ASCII ＝ 0x02+0x30；
	}
	else          // 其他的Ascii码对应的Hex
	{
		cr[0] = high + 0x37;	// 对应的字母（A-F）的Ascii码 = 该字母对应的Hex  + 0x37； 例如: d对应的ASCII ＝ 0xd+0x37；
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

//解析功能，还未完善
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
		// 数据不完整或者数据有错
	}
	
	return nums;
}
