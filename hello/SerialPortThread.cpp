#include "SerialPortThread.h"
#include <QtSerialBus\qmodbusrtuserialmaster.h>
#include <Windows.h>  //����ʹ��Sleep����

//��������������ò�����,�򿪴��ڵ�ʱ���õ�
static QSerialPort::BaudRate getBaudRate(int baudRate)
{
	switch (baudRate)
	{
	case 1200:
		return QSerialPort::Baud1200;
	case 2400:
		return QSerialPort::Baud2400;
	case 4800:
		return QSerialPort::Baud4800;
	case 9600:
		return QSerialPort::Baud9600;
	case 19200:
		return QSerialPort::Baud19200;
	case 38400:
		return QSerialPort::Baud38400;
	case 57600:
		return QSerialPort::Baud57600;
	case 115200:
		return QSerialPort::Baud115200;
	default:
		return QSerialPort::UnknownBaud;
	}
}
SerialPort::SerialPort()
{
	qDebug() << "Worker Thread : " << QThread::currentThreadId();

	m_serialPort = new QSerialPort();
	m_baudRate = 9600;
	m_portName = "com1";
	StopThread = false;
	m_bIsPauseThread = false;
	m_reciveLen = 8;
	
}

//window ����"com1"   linux����"/dev/ttyS0"
void SerialPort::setPortName(const QString &name)
{
	m_portName = name;
}
//������ȡ���ڵ�����
QString SerialPort::portName() const
{
	return m_portName;
}
//���ò����� 9600  19200  38400
void SerialPort::setBaudRate(int baudRate)
{
	m_baudRate = baudRate;
}

//���ò����� 9600  19200  38400
void SerialPort::setReciveLen(int Len)
{
	m_reciveLen = Len;
}

//�����򿪴��ڣ�����ǰ�������ô������ֺͲ�����
bool SerialPort::open()
{
	if (m_serialPort->isOpen())
	{
		return true;
	}
	m_serialPort->setPortName(m_portName);
	m_serialPort->setBaudRate(getBaudRate(m_baudRate));
	m_serialPort->setParity(QSerialPort::NoParity);
	m_serialPort->setDataBits(QSerialPort::Data8);
	m_serialPort->setStopBits(QSerialPort::OneStop);
	m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
	m_serialPort->setReadBufferSize(1024);
	return m_serialPort->open(QSerialPort::ReadWrite);
}
//�����رմ���
void SerialPort::close()
{
	if (m_serialPort->isOpen())
	{
		m_serialPort->close();
	}
}
//��������,�������
bool SerialPort::clear()
{
	if (m_serialPort->isOpen())
	{
		m_serialPort->clear();
		this->close();
		return this->open();
	}
	return false;
}
////�������մ��ڷ���������
//int SerialPort::readData(char *buffer, int count, int timeout)
//{
//	int len = 0;
//	forever
//	{
//		int n = m_serialPort->read(&buffer[len], count - len);
//
//		if (n == -1)
//		{
//			return -1;
//		}
//		else if (n == 0 && !m_serialPort->waitForReadyRead(timeout))
//		{
//			return -2;
//		}
//		else
//		{
//			len += n;
//			if (count == len)
//				break;
//		}
//	}
//	return len;
//}
//�������ݵ�����  ���緢��Э�� 
int SerialPort::writeData(char *data, int size)
{
	int len = 0;
	forever
	{
		int n = m_serialPort->write(&data[len], size - len);
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
	return len;
}

//void SerialPort::recivedata()
//{
//	QByteArray requestData;
// 	char temp[8] = "";
//	//char* temp = new char[8];
//	memset(temp, 0, 8);
//	int q = readData(temp, 8);
//	requestData.resize(8);
//	
//	for (int i = 0; i<8; i++)
//	{
//		requestData[i] = temp[i];
//	}
//
//	if (!requestData.isEmpty())
//	{
//		emit emitdata(requestData);
//		qDebug() << "recivedata Thread : " << QThread::currentThreadId();
//		qDebug() << "recivedata : " << requestData.toHex();
//	}
//	requestData.clear();
//	Sleep(20);	
//
//}

void SerialPort::recivedata()
{
	QByteArray requestData;
	if (m_serialPort->bytesAvailable() == m_reciveLen)
	{
		requestData = m_serialPort->readAll();
	}

	if (!requestData.isEmpty())
	{
		emit emitdata(requestData);
		qDebug() << "children Thread : " << QThread::currentThreadId()
				<< "    recivedata : " << requestData.toHex();
	}
	//requestData.clear();
//	Sleep(20);

}

//������ʱ��  ɾ�� ����
SerialPort::~SerialPort()
{
	delete m_serialPort;
}
