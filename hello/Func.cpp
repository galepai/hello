
#include "Func.h"
#include <QTime>
#include <QSettings>


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