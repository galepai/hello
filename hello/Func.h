
/**
*	功能：通用函数定义
*	作者：chenhui
*/


#ifndef FUNC_H
#define FUNC_H

#include <QTextCodec>
#include <QVariant>


#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )	//QT中显示中文，使用方法 G2U("中文")
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )

#define HSCROLL_HEIGHT_RightPic(height) ui.scrollAreaWidgetContentsRight->setMinimumHeight(height)	//显示右边区域高度，用于显示出滚动条
#define HSCROLL_HEIGHT_LeftPic(height) ui.scrollAreaWidgetContentsLeft->setMinimumHeight(height)	//显示左边区域高度，用于显示出滚动条

#define HIDDLE_DIALOG_BUTTON setWindowFlags(Qt::ToolTip);	//隐藏对话框右上角的按钮，比如最小化，最大化、关闭按钮


void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName);	//配置文件中写入当前时间
void WriteConfigure(const QString& file, const QString& beginGroup, const QString& SetValueName, const QString& Param);	//配置文件中写入设置信息
void ReadConfigure(const QString& file, const QString& beginGroup, const QString& GetValueName, QVariant& Value);	////配置文件中读取信息


enum LocationView
{
	LeftView,
	MiddleView,
	RightView,
};

#endif // FUNC_H