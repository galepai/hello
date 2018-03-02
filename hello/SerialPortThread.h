/**
*	���ܣ��������װ��moveToThread(QThread*)�������ڶ��߳�
*	���ߣ�chenhui
*/


#ifndef SerialPortThread_H
#define SerialPortThread_H

#include <QObject>  
#include <QtSerialPort/QtSerialPort>

class SerialPort : public QObject 
{
	Q_OBJECT
public:
	SerialPort();
	~SerialPort();

	bool isOpen() const{ return m_serialPort->isOpen(); };
	QSerialPort* GetSerialPort() const{ return m_serialPort; };
	void setPortName(const QString &name);	//���ö˿ں�
	QString portName() const;	//��ȡ�˿ں�
	void setBaudRate(int baudRate);	//���ò�����
	int baudRate() const;	//��ȡ��������
	void setReciveLen(int ByteLen);	//����ÿ�ν������ݹ̶��ֽڳ���
	virtual bool open();	//�򿪶˿�
	virtual void close();
	virtual bool clear();
	int readData(char *buffer, int count, int timeout = 1000);
	int writeData(char *data, int size);

protected:
	QString m_portName;
	int m_baudRate;
	QSerialPort *m_serialPort;
	int m_reciveLen;

public slots:
	void recivedata();	//�������ݲۣ������з����ź� emitdata(QByteArray receiveData)

signals:
	void resultReady(bool bIsBad);
	void emitdata(QByteArray receiveData);
};

#endif