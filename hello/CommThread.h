#ifndef COMM_THREAD_H
#define COMM_THREAD_H

#include <QThread>
#include <QDebug>
#include <QtSerialPort/QtSerialPort>
#include "hello.h"

class CommThread : public QThread
{
	Q_OBJECT

public:
	explicit CommThread(QObject *parent = 0)
		: QThread(parent)
	{
		qDebug() << "Worker Thread : " << QThread::currentThreadId();

		m_serialPort = new QSerialPort();
		m_baudRate = 9600;
		m_portName = "com1";
		StopThread = false;
		m_bIsPauseThread = false;
	}
	~CommThread();


	bool isOpen() const{ return m_serialPort->isOpen(); };
	void SetPauseThread(bool IsPauseThread){ m_bIsPauseThread = IsPauseThread; };
	QSerialPort* GetSerialPort() const{ return m_serialPort; };

	bool GetPauseThreadStatu(){ return m_bIsPauseThread; };
	void setPortName(const QString &name);
	QString portName() const;
	void setBaudRate(int baudRate);
	int baudRate() const;
	virtual bool open();
	virtual void close();
	virtual bool clear();
	int readData(char *buffer, int count, int timeout = 1000);
	int writeData(char *data, int size);
	int write(char ch);

	hello* pHello;

protected:
	QString m_portName;
	int m_baudRate;
	QSerialPort *m_serialPort;
	bool m_bIsPauseThread;
	bool StopThread;
	

public slots:
	void recivedata();

protected:
	virtual void run() Q_DECL_OVERRIDE;

signals:
	void resultReady(bool bIsBad);


};

#endif