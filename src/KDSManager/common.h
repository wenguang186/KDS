// ����һЩȫ������ı���������Resource.h

#pragma once

#include "resource.h"
#include <vector>

// ��ʾ��� - ��Ʒ������
enum KDS_STYLE{E_STYLE_DETAIL, E_STYLE_ORDER};

// �������� - ��⿣����� ,�к�
enum KDS_Mode{E_M_COOK, E_M_SEND, E_M_CNT};

// ��(history_)order_datail�е�is_make��Ӧ(E_P_COOKING��ӦNULL)
enum KDS_Process{E_P_COOKING, E_P_COOKED, E_P_SENDED, E_P_COOKPEND, E_P_SENDPEND, E_P_INVALID, E_P_CNT};

// ��ʾ����������  zzl-2017.3.9 ���һ��������������
enum KDS_ShowType{E_S_DONING, E_S_PENDED, E_S_DONE, E_S_INVALID};

// �òͷ�ʽ�ַ���ID��0-��ʳ��1-���
const INT g_strEatTypeID[]= {IDS_STR_EATIN, IDS_STR_TAKEOUT, IDS_STR_DELIVERY};

// �������ݱ�
enum E_ORDER_TYPE{E_TMP_ORDER=1, E_HISTORY_ORDER};

// 2017.3.9 zzl
//��Ʒ������ʾģʽ 1-ֻ��ʾ��Ʒ��1  2-ֻ��ʾ��Ʒ��2  3-��ʾ��Ʒ��1�Ͳ�Ʒ��2
enum E_ItemName_ShowType{E_ONLYNAME1=1,E_ONLYNAME2,E_NAME1ANDNAME2};

//�߲����� 0-����ʱ������  1-�߲�����
enum E_SORT_TYPE{E_ORDER_TIME,E_RUSH};

#ifdef _DEBUG
#define DebugMessageBox(X) AfxMessageBox(X)
#else
#define DebugMessageBox(X)
#endif

// ��ʱ���ʽ�е�yearȥ��
// "2016-07-01 00:00:00" -> "07-01 00:00:00"
#define DATETIME_CUTTER(str) (str).Right(14)
//BOOL VectorCmp(std::vector<INT> &vec_1, std::vector<INT> &vec_2);

void  POSMessageBox(INT nStrId);
void  POSMessageBox(CString &strMsg);

#include "MultiLanguage.h"

extern CMultiLanguage g_lang;

int ScaleX(int pt);

int ScaleY(int pt);