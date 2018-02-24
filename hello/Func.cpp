
#include "Func.h"
#include <QTime>
#include <QSettings>

void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName)
{
	QDateTime datetime;
	QSettings setting(file, QSettings::IniFormat);//���������ļ�
	//setting.setIniCodec(QTextCodec::codecForName("GB2312"));  //֧������ ������Ч
	setting.beginGroup(beginGroup);//beginGroup������endGroup ���Ӧ����config���Ǳ��
	setting.setValue(SetValueName, QVariant(datetime.currentDateTime().toString(Qt::ISODateWithMs)));
	setting.endGroup();
}