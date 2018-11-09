/**
  ******************************************************************************
  * @file         lock_evt.h
  * @brief        ��lock_evt.c��Ӧ��ͷ�ļ�
  * @author       Fli
  * @date         2018-06-22
  * @version      v001
  * @copyright    kaadas
  * 
  * @note         �����ˡ�������
  *
  ******************************************************************************
  */
#ifndef __LOCK_EVT_H__
#define __LOCK_EVT_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"


ErrorStatus LockEvt_Create(void (*pFun)(void), uint16_t timeMs);
void LockEvt_Clear(void);
void LockEvt_TimeUpdate(void);


#ifdef __cplusplus
}
#endif

#endif /*__LOCK_EVT_H__ */
