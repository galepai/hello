#ifndef FUNC_H
#define FUNC_H

#include <QTextCodec>

#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )	//正确显示中文，使用方法 G2U("中文")
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )

#define HSCROLL_HEIGHT_RightPic(height) ui.scrollAreaWidgetContentsRight->setMinimumHeight(height)
#define HSCROLL_HEIGHT_LeftPic(height) ui.scrollAreaWidgetContentsRight->setMinimumHeight(height)



void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName);

#endif // FUNC_H