
/**
*	功能：通用函数定义
*	作者：chenhui
*/


#ifndef FUNC_H
#define FUNC_H

#include <QTextCodec>
#include <QVariant>

//QT中显示中文，使用方法 G2U("中文")
#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )	
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )

#define HSCROLL_HEIGHT_RightPic(height) ui.scrollAreaWidgetContentsRight->setMinimumHeight(height)	//显示右边区域高度，用于显示出滚动条
#define HSCROLL_HEIGHT_LeftPic(height) ui.scrollAreaWidgetContentsLeft->setMinimumHeight(height)	//显示左边区域高度，用于显示出滚动条

#define HIDDLE_DIALOG_BUTTON setWindowFlags(Qt::ToolTip);	//隐藏对话框右上角的按钮，比如最小化，最大化、关闭按钮


void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName);	//配置文件中写入当前时间
void WriteConfigure(const QString& file, const QString& beginGroup, const QString& SetValueName, const QString& Param);	//配置文件中写入设置信息
void ReadConfigure(const QString& file, const QString& beginGroup, const QString& GetValueName, QVariant& Value);	////配置文件中读取信息

std::string Gen_Delta_Ascii_CR(const std::string& data);
std::string Delta_Ascii_CR(const std::string& data);	////配置文件中读取信息
std::string Delta_Ascii_CR(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info);
std::vector<short> Parse_Delta_Ascii(const std::string& data);	////配置文件中读取信息

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