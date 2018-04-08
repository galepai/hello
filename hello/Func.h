
/**
*	���ܣ�ͨ�ú�������
*	���ߣ�chenhui
*/


#ifndef FUNC_H
#define FUNC_H

#include <QTextCodec>
#include <QVariant>

//QT����ʾ���ģ�ʹ�÷��� G2U("����")
#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )	
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )

#define HSCROLL_HEIGHT_RightPic(height) ui.scrollAreaWidgetContentsRight->setMinimumHeight(height)	//��ʾ�ұ�����߶ȣ�������ʾ��������
#define HSCROLL_HEIGHT_LeftPic(height) ui.scrollAreaWidgetContentsLeft->setMinimumHeight(height)	//��ʾ�������߶ȣ�������ʾ��������

#define HIDDLE_DIALOG_BUTTON setWindowFlags(Qt::ToolTip);	//���ضԻ������Ͻǵİ�ť��������С������󻯡��رհ�ť


void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName);	//�����ļ���д�뵱ǰʱ��
void WriteConfigure(const QString& file, const QString& beginGroup, const QString& SetValueName, const QString& Param);	//�����ļ���д��������Ϣ
void ReadConfigure(const QString& file, const QString& beginGroup, const QString& GetValueName, QVariant& Value);	////�����ļ��ж�ȡ��Ϣ

std::string Gen_Delta_Ascii_CR(const std::string& data);
std::string Delta_Ascii_CR(const std::string& data);	////�����ļ��ж�ȡ��Ϣ
std::string Delta_Ascii_CR(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info);
std::vector<short> Parse_Delta_Ascii(const std::string& data);	////�����ļ��ж�ȡ��Ϣ

enum LocationView
{
	LeftView,
	MiddleView,
	RightView,
};



struct Delta_Ascii_Info
{
	short Slave;
	short Function_Code;
	short Start_Address_High;
	short Start_Address_Low;
	short End_Address_High;
	short End_Address_Low;
	short Other_Info;
};
#endif // FUNC_H