#include "DeltaThread.h"
#include "Func.h"
#include <QMessageBox>

QSerialPort* Delta_Thread::m_SerialPort = nullptr;
//std::string Thread22::str = "";
std::atomic<std::string> Delta_Thread::m_write_string = "";
std::atomic<bool> Delta_Thread::m_bIsStop = false;
std::vector<std::string> Delta_Thread::m_Vector_String;
bool Delta_Thread::m_bIsOneMode = false;

Delta_Thread::Delta_Thread(QObject *parent)
	: QThread(parent)
{
	qDebug() << "Delta_Thread construct func(),  Thread : " << QThread::currentThreadId();
}

void Delta_Thread::InitSerialPortInfo(const char* PortName, int BaudRate, QSerialPort::Parity parity, QSerialPort::DataBits databits)
{
	setPortName(PortName);
	setBaudRate(BaudRate);
	setParity(parity);
	setDataBits(databits);
}

void Delta_Thread::setWriteInfo(const char* wirteInfo)
{
	m_write_string = Delta_Ascii_CR(wirteInfo);

}

void Delta_Thread::setWriteInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& End_Address, const std::string& Other_Info)
{
	m_write_string = Delta_Ascii_CR(Slave + Function_Code + Start_Address + End_Address + Other_Info);

}

void Delta_Thread::setVectorInfo(const std::string& Slave, const std::string& Function_Code, const std::string& Start_Address, const std::string& End_Address, const std::string& Other_Info)
{
	m_Vector_String.push_back(Delta_Ascii_CR(Slave + Function_Code + Start_Address + End_Address + Other_Info));
}

void Delta_Thread::setPortName(const char* PortName)
{
	m_PortName = PortName;
}

void Delta_Thread::setBaudRate(int BaudRate)
{
	m_BaudRate = BaudRate;
}

void Delta_Thread::setParity(QSerialPort::Parity parity)
{
	m_parity = parity;
}

void Delta_Thread::setDataBits(QSerialPort::DataBits databits)
{
	m_databits = databits;
}

void Delta_Thread::CreateSerialPort()
{
	m_SerialPort = new QSerialPort;
	m_SerialPort->setBaudRate(m_BaudRate);
	m_SerialPort->setPortName(m_PortName.c_str());
	m_SerialPort->setParity(m_parity);
	m_SerialPort->setDataBits(m_databits);
	m_SerialPort->setStopBits(QSerialPort::OneStop);
	m_SerialPort->setFlowControl(QSerialPort::NoFlowControl);
	
}

void Delta_Thread::run()
{
	//std::string test = ":00050520FF00D7\r\n";

	qDebug() << "Delta_Thread Run Thread : " << QThread::currentThreadId();

	//setWriteInfo("00050520FF00");

	Delta_Thread::setVectorInfo("00", "05", "0501", "", "FF00");

	CreateSerialPort();

	if (!m_SerialPort->open(QSerialPort::ReadWrite))
	{
		return;
	}
	
	while (!m_bIsStop)
	{
		if (!m_Vector_String.empty())
		{
			m_write_string = m_Vector_String[m_Vector_String.size()-1];
			m_Vector_String.pop_back();
		}
		writeData(m_write_string._My_val.c_str(), m_write_string._My_val.length());
	
		m_SerialPort->waitForReadyRead(1000);
		receiveData();
		m_SerialPort->clear();
		if (m_bIsOneMode)
			break;
	}

	delete m_SerialPort;
	m_SerialPort = nullptr;


}

int Delta_Thread::writeData(const char *data, int size)
{
	int len = 0;
	forever
	{
		int n = m_SerialPort->write(&data[len], size - len);
		if (n == -1)
		{
			return -1;
		}
		else
		{
			len += n;
			if (size == len)
				break;
		}
	}
	//qDebug() << "Thread22 writeFunc Thread : " << QThread::currentThreadId();
	return len;

}

void Delta_Thread::receiveData()
{

	bool is_continue_recive = true;
	//int t = 0;

	while (is_continue_recive)
	{
		m_SerialPort->waitForReadyRead(50);
		requestData = m_SerialPort->readAll();
		//t++;
		//if (!m_total_data.isEmpty() && m_total_data[m_total_data.length() - 1] == '\n')
		if (requestData.isEmpty())
		{
			is_continue_recive = false;
			m_SerialPort->clear();
			emit emitdata(m_total_data);
			qDebug() << "recivedata : " << m_total_data;
			//qDebug() << "p1 : " << (void*)m_total_data.constData();
			m_total_data.clear();
		}
		else
		{
			m_total_data.append(requestData);
		}

	}
}

void Delta_Thread::setOneMode(bool status)
{
	m_bIsOneMode = status;
}

void Delta_Thread::StopRun(bool status)
{
	m_bIsStop = status;
}

QSerialPort* Delta_Thread::GetSerialPort()
{
	return m_SerialPort;
}

Delta_Thread::~Delta_Thread()
{

	if (isRunning())
		wait();

	m_bIsStop = false;
	m_Vector_String.clear();
	m_bIsOneMode = false;

}