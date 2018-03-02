
/**
*	���ܣ�ͨ�ú�������
*	���ߣ�chenhui
*/


#ifndef FUNC_H
#define FUNC_H

#include <QTextCodec>
#include <QVariant>


#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )	//QT����ʾ���ģ�ʹ�÷��� G2U("����")
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )

#define HSCROLL_HEIGHT_RightPic(height) ui.scrollAreaWidgetContentsRight->setMinimumHeight(height)	//��ʾ�ұ�����߶ȣ�������ʾ��������
#define HSCROLL_HEIGHT_LeftPic(height) ui.scrollAreaWidgetContentsLeft->setMinimumHeight(height)	//��ʾ�������߶ȣ�������ʾ��������

#define HIDDLE_DIALOG_BUTTON setWindowFlags(Qt::ToolTip);	//���ضԻ������Ͻǵİ�ť��������С������󻯡��رհ�ť


void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName);	//�����ļ���д�뵱ǰʱ��
void WriteConfigure(const QString& file, const QString& beginGroup, const QString& SetValueName, const QString& Param);	//�����ļ���д��������Ϣ
void ReadConfigure(const QString& file, const QString& beginGroup, const QString& GetValueName, QVariant& Value);	////�����ļ��ж�ȡ��Ϣ


enum LocationView
{
	LeftView,
	MiddleView,
	RightView,
};

#endif // FUNC_H