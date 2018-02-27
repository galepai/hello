#ifndef FUNC_H
#define FUNC_H

#include <QTextCodec>
#include <QVariant>

#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )	//��ȷ��ʾ���ģ�ʹ�÷��� G2U("����")
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )

#define HSCROLL_HEIGHT_RightPic(height) ui.scrollAreaWidgetContentsRight->setMinimumHeight(height)
#define HSCROLL_HEIGHT_LeftPic(height) ui.scrollAreaWidgetContentsRight->setMinimumHeight(height)



void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName);
void WriteConfigure(const QString& file, const QString& beginGroup, const QString& SetValueName, const QString& Param);
void ReadConfigure(const QString& file, const QString& beginGroup, const QString& GetValueName, QVariant& Value);

#endif // FUNC_H