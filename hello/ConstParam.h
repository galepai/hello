
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

#endif