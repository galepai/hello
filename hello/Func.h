
/**
*	功能：通用函数定义
*	作者：chenhui
*/


#ifndef FUNC_H
#define FUNC_H

#include <QTextCodec>
#include <QVariant>
#include <QFile>
#include <QTextStream>

#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )	//QT中显示中文，使用方法 G2U("中文")
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )

#define HSCROLL_HEIGHT_RightPic(height) ui.scrollAreaWidgetContentsRight->setMinimumHeight(height)	//显示右边区域高度，用于显示出滚动条
#define HSCROLL_HEIGHT_LeftPic(height) ui.scrollAreaWidgetContentsLeft->setMinimumHeight(height)	//显示左边区域高度，用于显示出滚动条
#define HSCROLL_HEIGHT_MiddlePic(height) ui.scrollAreaWidgetContentsMiddle->setMinimumHeight(height)	//显示中间区域高度，用于显示出滚动条

#define HIDDLE_DIALOG_BUTTON setWindowFlags(Qt::ToolTip);	//隐藏标题栏，比如最小化，最大化、关闭按钮


void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName);	//配置文件中写入当前时间 Use QSetting,slower than QFile 
void WriteConfigure(const QString& file, const QString& beginGroup, const QString& SetValueName, const QString& Param);	//配置文件中写入设置信息
void ReadConfigure(const QString& file, const QString& beginGroup, const QString& GetValueName, QVariant& Value);	//配置文件中读取信息


/* Example
for (int i = 1; i < 101; i++)
{
	QStringList writeinfo_list;
	writeinfo_list.append(QString("images/camera1/%1.bmp").arg(i, 4, 10, QChar('0')));
	writeinfo_list.append(QString("images/camera2/%1.bmp").arg(i, 4, 10, QChar('0')));
	writeinfo_list.append(QString("images/camera3/%1.bmp").arg(i, 4, 10, QChar('0')));
	writeinfo_list.append(QString("images/camera4/%1.bmp").arg(i, 4, 10, QChar('0')));
	QtWriteFile(QString("0423/%1.ini").arg(i, 4, 10, QChar('0')), writeinfo_list);
}
*/
void QtWriteFile(const QString& path_filename, const QStringList& writeinfo_list);

/* Example
QStringList readinfo_list;
QtReadFile(QString("0423/%1.ini").arg(15, 4, 10, QChar('0')), readinfo_list);
*/
void QtReadFile(const QString& path_filename,  QStringList& readinfo_list);

std::string Gen_Delta_Ascii_CR(const std::string& data);	//生成台达ASCII模式下的LRC
std::string Delta_Ascii_CR(const std::string& data);	//生成台达ASCII模式下的完整data,包括:+data+LRC+\r\n
std::string Delta_Ascii_CR(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info);	//生成台达ASCII模式下的完整data,包括:+data+LRC+\r\n
std::vector<bool> Parse_Delta_Ascii(const std::string& data);	//Delta接收信息，解析功能

bool CreaImagetDir();



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