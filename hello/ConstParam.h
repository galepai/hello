
/**
*	���ܣ�ͨ�ú�������
*	���ߣ�chenhui
*/


#ifndef CONSTPARAM_H
#define CONSTPARAM_H

static const char* AreaCamera880Id = "[0] USB3_CMOS_8.8M(2)";
static const char* AreaCameraId = "[0] USB3_CMOS_8.8M(1)";
static const char* AreaCameraId2 = "[0] USB3_CMOS_1.3M_M(2)";
//static const char* LineCameraId = "003053255252_Basler_raL204848gm";
static const char* LineCameraId = "00010dc2ae11_TeledyneDALSA_LineaM20487um";

//	ץ��ˮƽ����
static const char* ZHUASHOU_LEFT_ON = "000508E2FF00";  //M226 SET ץ��ˮƽ����
static const char* ZHUASHOU_LEFT_OFF = "000508E20000";  //M226 RST ץ��ˮƽ����

//	ץ��ˮƽ����
static const char* ZHUASHOU_RIGHT_ON = "000508E3FF00";  //M227 SET ץ��ˮƽ����
static const char* ZHUASHOU_RIGHT_OFF = "000508E30000";  //M227 RST ץ��ˮƽ����

//	ץ�ִ�ֱ����
static const char* ZHUASHOU_DOWN_ON = "000508E4FF00";  //M228 SET ץ��ˮƽ����
static const char* ZHUASHOU_DOWN_OFF = "000508E40000";  //M228 RST ץ��ˮƽ����

//	ץ�ִ�ֱ����
static const char* ZHUASHOU_UP_ON = "000508E5FF00";  //M229 SET ץ��ˮƽ����
static const char* ZHUASHOU_UP_OFF = "000508E50000";  //M229 RST ץ��ˮƽ����

//	��ָ��
static const char* HAND_OPEN_ON = "000508E6FF00";  //M230 SET ��ָ��
static const char* HAND_OPEN_OFF = "000508E60000";  //M230 RST ��ָ��

//	��ָ��
static const char* HAND_CLOSE_ON = "000508E7FF00";  //M231 SET ��ָ��
static const char* HAND_CLOSE_OFF = "000508E70000";  //M231 RST ��ָ��

//	������
static const char* BOSHOU_UP_ON = "000508ECFF00";  //M236 SET ������
static const char* BOSHOU_UP_OFF = "000508EC0000";  //M236 RST ������

//	������
static const char* BOSHOU_DOWN_ON = "000508EDFF00";  //M237 SET ������
static const char* BOSHOU_DOWN_OFF = "000508ED0000";  //M237 RST ������

//	����ˮƽ��
static const char* BOSHOU_SHUIPING_UP_ON = "000508EAFF00";  //M234 SET ������
static const char* BOSHOU_SHUIPING_UP_OFF = "000508EA0000";  //M234 RST ������

//	����ˮƽ��
static const char* BOSHOU_SHUIPING_DOWN_ON = "000508EBFF00";  //M235 SET ������
static const char* BOSHOU_SHUIPING_DOWN_OFF = "000508EB0000";  //M235 RST ������

//	�Ӿ���
static const char* VISION_UP_ON = "000508A5FF00";  //M165 SET �Ӿ���
static const char* VISION_UP_OFF = "000508A50000";  //M165 RST �Ӿ���

//	�Ӿ���
static const char* VISION_DOWN_ON = "000508A6FF00";  //M166 SET �Ӿ���
static const char* VISION_DOWN_OFF = "000508A60000";  //M166 RST �Ӿ���

//	��ʼ��ת����ƽ�ź�
static const char* ROTATE_START_ON = "000508A7FF00";  //M167 SET ��ʼ��ת

//	ֹͣ��ת
static const char* ROTATE_STOP_ON = "000508A8FF00";  //M168 SET ֹͣ��ת
static const char* ROTATE_STOP_OFF = "000508A80000";  //M168 RST ֹͣ��ת
static const char* ROTATE_START_OFF = "000508A70000";  //M167 RST ��ʼ��ת

//	ֱ��ģ��ǰ��
static const char* PAOGUANG_ON = "000508A9FF00";  //M169 SET ֱ��ģ��ǰ��
static const char* PAOGUANG_OFF = "000508A90000";  //M169 RST ֱ��ģ��ǰ��

//	��D200-D205,  
//	���ص�D200��16λ���ݴ���ֱ��ģ��ľ���.	(��λ:0.1mm),
//	���ص�D202��16λ���ݴ����׹�ʱ��.	(��λ:100ms)
static const char* READ_D_REGISTER = "000310C80006";  //  ��D200-D206,  *���ص�D200(10C8)��32λ���ݴ���ֱ��ģ��ľ���.(��λ:0.1mm)		*���ص�D204��16λ���ݴ����׹�ʱ��.(��λ:100ms)

////	 ���׹���ʱ�� D202-D203,  ���ص�D202��32λ���ݴ����׹�ʱ��.	(��λ��100ms)
//static const char* READ_PAOGUANG_TIME = "000310CA0002";  //���׹���ʱ�� D202-D203,  ���ص�D202��32λ���ݴ����׹�ʱ��.	(��λ:100ms)

//	�Զ�ģʽ(��ƽ�ź�),M115��1Ϊ�Զ�
static const char* AUTO_MODE = "00050873FF00";  //�Զ�ģʽ(��ƽ�ź�),M115��1Ϊ�Զ�

//	�ֶ�ģʽ(��ƽ�ź�),M115��0Ϊ�ֶ�
static const char* MANUAL_MODE = "000508730000";  //�ֶ�ģʽ(��ƽ�ź�),M115��0Ϊ�ֶ�

//	�Զ�ģʽ����(��ƽ�ź�),M116��Ϊ1S
static const char* AUTO_START = "00050874FF00";  //�Զ�ģʽ����(��ƽ�ź�),M116��Ϊ1

//	�Զ�ģʽֹͣ(��ƽ�ź�),M116��Ϊ0
static const char* AUTO_STOP = "000508740000";  //�Զ�ģʽֹͣ(��ƽ�ź�),M116��Ϊ0

//	��������,D200(10C8) (��λ:0.1mm)
static const char* SET_DISTANCE = "000610C8";  //��������,D200(10C8) (��λ:0.1mm)

//	ʱ������,D204(10CC) ��λ:100ms
static const char* SET_TIME = "000610CC";  //ʱ������,D204(10CC) ��λ:100ms

#endif