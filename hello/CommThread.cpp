#include "CommThread.h"
#include <QtSerialBus\qmodbusrtuserialmaster.h>


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

void CommThread::run()
{
	//QModbusRtuSerialMaster* modbusMaster = new QModbusRtuSerialMaster();
	//if (modbusMaster->state() != QModbusDevice::ConnectedState) 
	//{	//������ڷ�����״̬����������
	//	//���д�������
	//	modbusMaster->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
	//		"com3");//��ȡ���ڶ˿�,�������ø��ֲ���
	//	modbusMaster->setConnectionParameter(QModbusDevice::SerialParityParameter,
	//		QSerialPort::NoParity);//����
	//	modbusMaster->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
	//		QSerialPort::Baud9600);//������Ϊ19200
	//	modbusMaster->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
	//		QSerialPort::Data8);//����λΪ8λ
	//	modbusMaster->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
	//		QSerialPort::OneStop);//ֹͣλΪ1λ
	//	modbusMaster->setTimeout(1000);//���ӳ�ʱ1S
	//	modbusMaster->setNumberOfRetries(3);//����ʧ��������������
	//	if (!modbusMaster->connectDevice()) 
	//	{
	//		QString err = modbusMaster->errorString();
	//	}
	//}
	//else
	//{//��������״̬���жϿ����ӵĲ���
	//	modbusMaster->disconnectDevice();
	//	return;
	//}

	//if (!modbusMaster)//����豸û�б������ͷ���
	//	return;
	//QString strDis;
	//QString str1;
	//qDebug() << str1;
	//QByteArray str2 = QByteArray::fromHex(str1.toLatin1().data());//��ʮ�����Ʊ�������ı�
	////qDebug()<<str2;
	//QString str3 = str2.toHex().data();//��ʮ��������ʾ

	//QModbusDataUnit writeUnit(QModbusDataUnit::HoldingRegisters, 0, 10);//���͵�������Ϣ���������� ����ʼ��ַ��������
	////    QModbusDataUnit::RegisterType table = writeUnit.registerType();//�õ��Ĵ������ʹ���table
	//for (uint i = 0; i < writeUnit.valueCount(); i++) {
	//	int j = 2 * i;
	//	QString st = str3.mid(j, 2);
	//	bool ok;
	//	int hex = st.toInt(&ok, 16);//��textedit�ж�ȡ��������ת��Ϊ16���Ʒ���
	//	qDebug() << hex;
	//	writeUnit.setValue(i, hex);//���÷�������
	//}
	//if (auto *reply = modbusMaster->sendWriteRequest(writeUnit, 1)) {// 1��server address   sendWriteRequest���������д����
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

	////������
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

//window ����"com1"   linux����"/dev/ttyS0"
void CommThread::setPortName(const QString &name)
{
	m_portName = name;
}
//������ȡ���ڵ�����
QString CommThread::portName() const
{
	return m_portName;
}
//���ò����� 9600  19200  38400
void CommThread::setBaudRate(int baudRate)
{
	m_baudRate = baudRate;
}
//�����򿪴��ڣ�����ǰ�������ô������ֺͲ�����

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
//�����رմ���
void CommThread::close()
{
	if (m_serialPort->isOpen())
	{
		m_serialPort->close();
	}
}
//��������,�������
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
//�������մ��ڷ���������
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
//�������ݵ�����  ���緢��Э�� 
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


//������ʱ��  ɾ�� ����
CommThread::~CommThread()
{
	delete m_serialPort;
}
