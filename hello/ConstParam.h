
/**
*	功能：通用函数定义
*	作者：chenhui
*/


#ifndef CONSTPARAM_H
#define CONSTPARAM_H

static const char* AreaCamera880Id = "[0] USB3_CMOS_8.8M(2)";
static const char* AreaCameraId = "[0] USB3_CMOS_8.8M(1)";
static const char* AreaCameraId2 = "[0] USB3_CMOS_1.3M_M(2)";
//static const char* LineCameraId = "003053255252_Basler_raL204848gm";
static const char* LineCameraId = "00010dc2ae11_TeledyneDALSA_LineaM20487um";

//	抓手水平左移
static const char* ZHUASHOU_LEFT_ON = "000508E2FF00";  //M226 SET 抓手水平左移
static const char* ZHUASHOU_LEFT_OFF = "000508E20000";  //M226 RST 抓手水平左移

//	抓手水平右移
static const char* ZHUASHOU_RIGHT_ON = "000508E3FF00";  //M227 SET 抓手水平右移
static const char* ZHUASHOU_RIGHT_OFF = "000508E30000";  //M227 RST 抓手水平右移

//	抓手垂直下移
static const char* ZHUASHOU_DOWN_ON = "000508E4FF00";  //M228 SET 抓手水平下移
static const char* ZHUASHOU_DOWN_OFF = "000508E40000";  //M228 RST 抓手水平下移

//	抓手垂直上移
static const char* ZHUASHOU_UP_ON = "000508E5FF00";  //M229 SET 抓手水平上移
static const char* ZHUASHOU_UP_OFF = "000508E50000";  //M229 RST 抓手水平上移

//	手指开
static const char* HAND_OPEN_ON = "000508E6FF00";  //M230 SET 手指开
static const char* HAND_OPEN_OFF = "000508E60000";  //M230 RST 手指开

//	手指关
static const char* HAND_CLOSE_ON = "000508E7FF00";  //M231 SET 手指关
static const char* HAND_CLOSE_OFF = "000508E70000";  //M231 RST 手指关

//	拨手伸
static const char* BOSHOU_UP_ON = "000508ECFF00";  //M236 SET 拨手伸
static const char* BOSHOU_UP_OFF = "000508EC0000";  //M236 RST 拨手伸

//	拨手缩
static const char* BOSHOU_DOWN_ON = "000508EDFF00";  //M237 SET 拨手缩
static const char* BOSHOU_DOWN_OFF = "000508ED0000";  //M237 RST 拨手缩

//	拨手水平伸
static const char* BOSHOU_SHUIPING_UP_ON = "000508EAFF00";  //M234 SET 拨手伸
static const char* BOSHOU_SHUIPING_UP_OFF = "000508EA0000";  //M234 RST 拨手伸

//	拨手水平缩
static const char* BOSHOU_SHUIPING_DOWN_ON = "000508EBFF00";  //M235 SET 拨手缩
static const char* BOSHOU_SHUIPING_DOWN_OFF = "000508EB0000";  //M235 RST 拨手缩

//	视觉升
static const char* VISION_UP_ON = "000508A5FF00";  //M165 SET 视觉升
static const char* VISION_UP_OFF = "000508A50000";  //M165 RST 视觉升

//	视觉降
static const char* VISION_DOWN_ON = "000508A6FF00";  //M166 SET 视觉降
static const char* VISION_DOWN_OFF = "000508A60000";  //M166 RST 视觉降

//	开始旋转，电平信号
static const char* ROTATE_START_ON = "000508A7FF00";  //M167 SET 开始旋转

//	停止旋转
static const char* ROTATE_STOP_ON = "000508A8FF00";  //M168 SET 停止旋转
static const char* ROTATE_STOP_OFF = "000508A80000";  //M168 RST 停止旋转
static const char* ROTATE_START_OFF = "000508A70000";  //M167 RST 开始旋转

//	直线模组前进
static const char* PAOGUANG_ON = "000508A9FF00";  //M169 SET 直线模组前进
static const char* PAOGUANG_OFF = "000508A90000";  //M169 RST 直线模组前进

//	读D200-D205,  
//	返回的D200的16位数据代表直线模组的距离.	(单位:0.1mm),
//	返回的D202的16位数据代表抛光时间.	(单位:100ms)
static const char* READ_D_REGISTER = "000310C80006";  //  读D200-D206,  *返回的D200(10C8)的32位数据代表直线模组的距离.(单位:0.1mm)		*返回的D204的16位数据代表抛光时间.(单位:100ms)

////	 读抛光轮时间 D202-D203,  返回的D202的32位数据代表抛光时间.	(单位是100ms)
//static const char* READ_PAOGUANG_TIME = "000310CA0002";  //读抛光轮时间 D202-D203,  返回的D202的32位数据代表抛光时间.	(单位:100ms)

//	自动模式(电平信号),M115置1为自动
static const char* AUTO_MODE = "00050873FF00";  //自动模式(电平信号),M115置1为自动

//	手动模式(电平信号),M115置0为手动
static const char* MANUAL_MODE = "000508730000";  //手动模式(电平信号),M115置0为手动

//	自动模式启动(电平信号),M116置为1S
static const char* AUTO_START = "00050874FF00";  //自动模式启动(电平信号),M116置为1

//	自动模式停止(电平信号),M116置为0
static const char* AUTO_STOP = "000508740000";  //自动模式停止(电平信号),M116置为0

//	距离设置,D200(10C8) (单位:0.1mm)
static const char* SET_DISTANCE = "000610C8";  //距离设置,D200(10C8) (单位:0.1mm)

//	时间设置,D204(10CC) 单位:100ms
static const char* SET_TIME = "000610CC";  //时间设置,D204(10CC) 单位:100ms

#endif