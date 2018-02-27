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
	void SetPauseThread(bool IsPauseThread){ m_bIsPauseThread = IsPauseThread; };
	QSerialPort* GetSerialPort() const{ return m_serialPort; };

	bool GetPauseThreadStatu(){ return m_bIsPauseThread; };
	void setPortName(const QString &name);
	QString portName() const;
	void setBaudRate(int baudRate);
	void setReciveLen(int ByteLen);
	int baudRate() const;
	virtual bool open();
	virtual void close();
	virtual bool clear();
	int readData(char *buffer, int count, int timeout = 1000);
	int writeData(char *data, int size);
	int write(char ch);

protected:
	QString m_portName;
	int m_baudRate;
	QSerialPort *m_serialPort;
	bool m_bIsPauseThread;
	bool StopThread;
	int m_reciveLen;

public slots:
	void recivedata();

signals:
	void resultReady(bool bIsBad);
	void emitdata(QByteArray receiveData);
};

#endif