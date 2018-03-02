/**
*	功能：串口类封装，moveToThread(QThread*)，可用于多线程
*	作者：chenhui
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
	void setPortName(const QString &name);	//设置端口号
	QString portName() const;	//获取端口号
	void setBaudRate(int baudRate);	//设置波特率
	int baudRate() const;	//获取波特率真
	void setReciveLen(int ByteLen);	//设置每次接收数据固定字节长度
	virtual bool open();	//打开端口
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
	void recivedata();	//接收数据槽，代码中发射信号 emitdata(QByteArray receiveData)

signals:
	void resultReady(bool bIsBad);
	void emitdata(QByteArray receiveData);
};

#endif