
/**
*	���ܣ�ͨ�ú�������
*	���ߣ�chenhui
*/


#ifndef FUNC_H
#define FUNC_H

#include <QTextCodec>
#include <QVariant>
#include <QFile>
#include <QTextStream>

#define G2U(s) ( QTextCodec::codecForName("GBK")->toUnicode(s) )	//QT����ʾ���ģ�ʹ�÷��� G2U("����")
#define U2G(s) ( QTextCodec::codecForName("GBK")->fromUnicode(s) )

#define HSCROLL_HEIGHT_RightPic(height) ui.scrollAreaWidgetContentsRight->setMinimumHeight(height)	//��ʾ�ұ�����߶ȣ�������ʾ��������
#define HSCROLL_HEIGHT_LeftPic(height) ui.scrollAreaWidgetContentsLeft->setMinimumHeight(height)	//��ʾ�������߶ȣ�������ʾ��������
#define HSCROLL_HEIGHT_MiddlePic(height) ui.scrollAreaWidgetContentsMiddle->setMinimumHeight(height)	//��ʾ�м�����߶ȣ�������ʾ��������

#define HIDDLE_DIALOG_BUTTON setWindowFlags(Qt::ToolTip);	//���ر�������������С������󻯡��رհ�ť


void WriteCurrenDateTime(const QString& file, const QString& beginGroup, const QString& SetValueName);	//�����ļ���д�뵱ǰʱ�� Use QSetting,slower than QFile 
void WriteConfigure(const QString& file, const QString& beginGroup, const QString& SetValueName, const QString& Param);	//�����ļ���д��������Ϣ
void ReadConfigure(const QString& file, const QString& beginGroup, const QString& GetValueName, QVariant& Value);	//�����ļ��ж�ȡ��Ϣ


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

std::string Gen_Delta_Ascii_CR(const std::string& data);	//����̨��ASCIIģʽ�µ�LRC
std::string Delta_Ascii_CR(const std::string& data);	//����̨��ASCIIģʽ�µ�����data,����:+data+LRC+\r\n
std::string Delta_Ascii_CR(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& Other_Info);	//����̨��ASCIIģʽ�µ�����data,����:+data+LRC+\r\n
std::vector<bool> Parse_Delta_Ascii(const std::string& data);	//Delta������Ϣ����������

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