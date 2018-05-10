
/**
*	功能：通用函数定义
*	作者：chenhui
*/


#ifndef CONSTPARAM_H
#define CONSTPARAM_H

static const char* AreaCameraId = "[0] USB3_CMOS_8.8M(1)";
static const char* AreaCameraId2 = "[0] USB3_CMOS_1.3M_M(2)";
static const char* LineCameraId = "003053255252_Basler_raL204848gm";

//	水平左移
static const char* SHUIPING_ZHUASHOU_LEFT_ON = "000508E2FF00";  //M226 SET 水平左移
static const char* SHUIPING_ZHUASHOU_LEFT_OFF = "000508E20000";  //M226 RST 水平左移

//	水平右移
static const char* SHUIPING_ZHUASHOU_RIGHT_ON = "000508E3FF00";  //M227 SET 水平右移
static const char* SHUIPING_ZHUASHOU_RIGHT_OFF = "000508E30000";  //M227 RST 水平右移

//	手指开
static const char* SHUIPING_HAND_OPEN_ON = "000508E6FF00";  //M226 SET 手指开
static const char* SHUIPING_HAND_OPEN_OFF = "000508E60000";  //M226 RST 手指开

//	手指关
static const char* SHUIPING_HAND_CLOSE_ON = "000508E7FF00";  //M227 SET 手指关
static const char* SHUIPING_HAND_CLOSE_OFF = "000508E70000";  //M227 RST 手指关

#endif