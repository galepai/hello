#include "CommThread.h"
#include <QtSerialBus\qmodbusrtuserialmaster.h>


//这个函数用来设置波特率,打开串口的时候用到
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

void CommThread::run()
{
	//QModbusRtuSerialMaster* modbusMaster = new QModbusRtuSerialMaster();
	//if (modbusMaster->state() != QModbusDevice::ConnectedState) 
	//{	//如果处在非连接状态，进行连接
	//	//进行串口连接
	//	modbusMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
	//		"com3");//获取串口端口,下面设置各种参数
	//	modbusMaster->setConnectionParameter(QModbusDevice::SerialParityParameter,
	//		QSerialPort::NoParity);//较验
	//	modbusMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
	//		QSerialPort::Baud9600);//波特率为19200
	//	modbusMaster->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
	//		QSerialPort::Data8);//数据位为8位
	//	modbusMaster->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
	//		QSerialPort::OneStop);//停止位为1位
	//	modbusMaster->setTimeout(1000);//连接超时1S
	//	modbusMaster->setNumberOfRetries(3);//连接失败重试三次连接
	//	if (!modbusMaster->connectDevice()) 
	//	{
	//		QString err = modbusMaster->errorString();
	//	}
	//}
	//else
	//{//处在连接状态进行断开连接的操作
	//	modbusMaster->disconnectDevice();
	//	return;
	//}

	//if (!modbusMaster)//如果设备没有被创建就返回
	//	return;
	//QString strDis;
	//QString str1;
	//qDebug() << str1;
	//QByteArray str2 = QByteArray::fromHex(str1.toLatin1().data());//按十六进制编码接入文本
	////qDebug()<<str2;
	//QString str3 = str2.toHex().data();//以十六进制显示

	//QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, 0, 10);//发送的数据信息（数据类型 ，起始地址，个数）
	////    QModbusDataUnit::RegisterType table = writeUnit.registerType();//得到寄存器类型传给table
	//for (uint i = 0; i < writeUnit.valueCount(); i++) {
	//	int j = 2 * i;
	//	QString st = str3.mid(j, 2);
	//	bool ok;
	//	int hex = st.toInt(&ok, 16);//将textedit中读取到的数据转换为16进制发送
	//	qDebug() << hex;
	//	writeUnit.setValue(i, hex);//设置发送数据
	//}
	//if (auto *reply = modbusMaster->sendWriteRequest(writeUnit, 1)) {// 1是server address   sendWriteRequest是向服务器写数据
	//	if (!reply->isFinished()) 
	//	{   //reply Returns true when the reply has finished or was aborted.
	//		connect(reply, &QModbusReply::finished, this, [this, reply]() 
	//		{
	//			if (reply->error() == QModbusDevice::ProtocolError) 
	//			{
	//				QString error = tr("Write response error: %1 (Mobus exception: 0x%2)").arg(reply->errorString())
	//					.arg(reply->rawResult().exceptionCode(), -1, 16);
	//			}
	//			else if (reply->error() != QModbusDevice::NoError) 
	//			{
	//				QString error = tr("Write response error: %1 (code: 0x%2)").
	//					arg(reply->errorString()).arg(reply->error(), -1, 16);
	//			}
	//			reply->deleteLater();
	//		});
	//	}
	//	else 
	//	{
	//		// broadcast replies return immediately
	//		reply->deleteLater();
	//	}
	//}
	//else {
	//	QString error = tr("Write error: ") + modbusMaster->errorString();
	//}

	////收数据
	//if (!modbusMaster)
	//	return;
	//QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, 0, 10);
	//if (auto *reply = modbusMaster->sendReadRequest(readUnit, 1))
	//{
	//	if (!reply->isFinished())
	//		connect(reply, SIGNAL(finished()), this, SLOT(readReady()));
	//	else
	//	{
	//		delete reply; // broadcast replies return immediately
	//	}
	//		
	//}
	//else
	//{
	//	QString error = tr("Read error: ") + modbusMaster->errorString();
	//}

////////////////////////////////////////////	
	qDebug() << "Worker Run Thread : " << QThread::currentThreadId();
	QByteArray buffer = "0000000000";
	int i = buffer.length();
	//while (!StopThread)
	//{
	//	if (!m_bIsPauseThread)
	//	{
	//		char buff[11] = "1234567890";
	//		writeData(buff, 10);
	//		m_serialPort->write(buffer);
	//		//readData(buff, 11);
	//		//msleep(20);
	//	}
	//	
	//}
	
	while (1)
	{
		recivedata();
		msleep(50);
	}


}

//window 下用"com1"   linux下用"/dev/ttyS0"
void CommThread::setPortName(const QString &name)
{
	m_portName = name;
}
//用来获取串口的名字
QString CommThread::portName() const
{
	return m_portName;
}
//设置波特率 9600  19200  38400
void CommThread::setBaudRate(int baudRate)
{
	m_baudRate = baudRate;
}
//用来打开串口，调用前，先设置串口名字和波特率

bool CommThread::open()
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
//用来关闭串口
void CommThread::close()
{
	if (m_serialPort->isOpen())
	{
		m_serialPort->close();
	}
}
//重启串口,清除数据
bool CommThread::clear()
{
	if (m_serialPort->isOpen())
	{
		m_serialPort->clear();
		this->close();
		return this->open();
	}
	return false;
}
//用来接收串口发来的数据
int CommThread::readData(char *buffer, int count, int timeout)
{
	int len = 0;
	forever
	{
		int n = m_serialPort->read(&buffer[len], count - len);
		if (n == -1)
		{
			return -1;
		}
		else if (n == 0 && !m_serialPort->waitForReadyRead(timeout))
		{
			return -2;
		}
		else
		{
			len += n;
			if (count == len)
				break;
		}
	}
	return len;
}
//发送数据到串口  比如发送协议 
int CommThread::writeData(char *data, int size)
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

void CommThread::recivedata()
{
	QByteArray requestData;
	char temp[8] = "";
	//char* temp = new char[8];
	memset(temp, 0, 8);
	//requestData = pCommThread->GetSerialPort()->readAll();
	//requestData = pCommThread->GetSerialPort()->read(8);
	readData(temp, 8);
	requestData.resize(8);

	for (int i = 0; i<8; i++) {
		requestData[i] = temp[i];
	}
	if (!requestData.isEmpty())
	{
		//pHello->statusBar()->showMessage(requestData.toHex());
		qDebug() << "recivedata Thread : " << QThread::currentThreadId();
		qDebug() << requestData.toHex();
	}
	requestData.clear();
}


//析构的时候  删除 数据
CommThread::~CommThread()
{
	delete m_serialPort;
}
