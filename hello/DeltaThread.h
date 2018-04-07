#ifndef DELTA_THREAD_H
#define DELTA_THREAD_H

#include <QThread>
#include <QDebug>
#include <QtSerialPort/QtSerialPort>
#include <atomic>

class Delta_Thread : public QThread
{
	Q_OBJECT

public:
	explicit Delta_Thread(QObject *parent = 0);
	~Delta_Thread();
	
	int writeData(const char *data, int size);
	void receiveData();
	void setPortName(const char* PortName);
	void setBaudRate(int BaudRate);
	void setParity(QSerialPort::Parity parity);
	void setDataBits(QSerialPort::DataBits databits);

	void InitSerialPortInfo(const char* PortName, int BaudRate, QSerialPort::Parity parity, QSerialPort::DataBits databits);
	static void setWriteInfo(const char* wirteInfo);
	static void setWriteInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& End_Address, const std::string& Other_Info);
	static void setVectorInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& End_Address, const std::string& Other_Info);
	static void setOneMode(bool status = false);
	static void StopRun(bool status = false);
	static QSerialPort* GetSerialPort();

protected:
	virtual void run() Q_DECL_OVERRIDE;

	void CreateSerialPort();
	QByteArray requestData;
	QByteArray m_total_data;
	std::string m_PortName;
	int m_BaudRate;
	QSerialPort::Parity m_parity;
	QSerialPort::DataBits m_databits;
	
	static QSerialPort* m_SerialPort;
	//static std::string str;
	static std::atomic<std::string> m_write_string;
	static std::atomic<bool> m_bIsStop;
	static std::vector<std::string> m_Vector_String;
	static bool m_bIsOneMode;


signals:
	void emitdata(QByteArray receiveData);
};

#endif