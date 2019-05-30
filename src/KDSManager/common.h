// 定义一些全局意义的变量，类似Resource.h

#pragma once

#include "resource.h"
#include <vector>

// 显示风格 - 单品，订单
enum KDS_STYLE{E_STYLE_DETAIL, E_STYLE_ORDER};

// 厨显类型 - 烹饪，传菜 ,叫号
enum KDS_Mode{E_M_COOK, E_M_SEND, E_M_CNT};

// 与(history_)order_datail中的is_make对应(E_P_COOKING对应NULL)
enum KDS_Process{E_P_COOKING, E_P_COOKED, E_P_SENDED, E_P_COOKPEND, E_P_SENDPEND, E_P_INVALID, E_P_CNT};

// 显示的三种类型  zzl-2017.3.9 添加一个不可制作类型
enum KDS_ShowType{E_S_DONING, E_S_PENDED, E_S_DONE, E_S_INVALID};

// 用餐方式字符串ID，0-堂食，1-外带
const INT g_strEatTypeID[]= {IDS_STR_EATIN, IDS_STR_TAKEOUT, IDS_STR_DELIVERY};

// 两种数据表
enum E_ORDER_TYPE{E_TMP_ORDER=1, E_HISTORY_ORDER};

// 2017.3.9 zzl
//菜品名称显示模式 1-只显示菜品名1  2-只显示菜品名2  3-显示菜品名1和菜品名2
enum E_ItemName_ShowType{E_ONLYNAME1=1,E_ONLYNAME2,E_NAME1ANDNAME2};

//催菜优先 0-订单时间优先  1-催菜优先
enum E_SORT_TYPE{E_ORDER_TIME,E_RUSH};

#ifdef _DEBUG
#define DebugMessageBox(X) AfxMessageBox(X)
#else
#define DebugMessageBox(X)
#endif

// 把时间格式中的year去掉
// "2016-07-01 00:00:00" -> "07-01 00:00:00"
#define DATETIME_CUTTER(str) (str).Right(14)
//BOOL VectorCmp(std::vector<INT> &vec_1, std::vector<INT> &vec_2);

void  POSMessageBox(INT nStrId);
void  POSMessageBox(CString &strMsg);

#include "MultiLanguage.h"

extern CMultiLanguage g_lang;

int ScaleX(int pt);

int ScaleY(int pt);