/**
  ******************************************************************************
  * @file         lock_evt.c
  * @brief        ������ʱ�¼�����ʵ�־�ȷ��������ʱִ��ĳ���¼�
  * @author       Fli
  * @date         2018-06-22
  * @version      v001
  * @copyright    kaadas
  * 
  * @note         �����ˡ�������
  *
  ******************************************************************************
  */
#include "lock_evt.h"

#define EVENT_MAX_QTY  (5)

static __IO uint32_t timeBeatCount;//ʱ�����

static struct
{
	void (*pFunction)(void);
	uint32_t timeout;
}eventList[EVENT_MAX_QTY]; //�¼���

static uint8_t mutexFlag;  //�¼�����ʻ����־



/**
  * @brief  ���������ʱ�¼�
  *         
  * @note   
  */
void LockEvt_Clear(void)
{
	mutexFlag = 1;
	
	timeBeatCount = 0;
	memset(eventList, 0, sizeof(eventList));
	
	mutexFlag = 0;
}

/**
  * @brief  ����һ����ʱ�¼�
  * @note   
  *         
  * @param  pFun���¼�������
  * @param  timeMs����ʱʱ��MS
  */
ErrorStatus LockEvt_Create(void (*pFun)(void), uint16_t timeMs)
{
	uint8_t i;
	
	mutexFlag = 1;

	if (pFun == NULL)
	{
		mutexFlag = 0;
		return ERROR;
	}
	
	for (i=0; i<EVENT_MAX_QTY; i++)
	{
		if (eventList[i].pFunction == NULL)
		{
			break;
		}
	}
	
	if (i == EVENT_MAX_QTY)
	{
		mutexFlag = 0;
		return ERROR;
	}

	eventList[i].timeout = timeMs + timeBeatCount;
	eventList[i].pFunction = pFun;
	
	mutexFlag = 0;
	return SUCCESS;
}

/**
  * @brief  ��ʱ�¼� ʱ����º���
  *         
  * @note   �ú�������ʹ�ö�ʱ���ж�ÿ���2ms����һ��
  */
void LockEvt_TimeUpdate(void)
{
	uint8_t i;
	void (*pFun)(void);
	
	timeBeatCount += TIME_BASIC_MS;
	
	if (mutexFlag != 0)
	{
		return;
	}
	
	for (i=0; i<EVENT_MAX_QTY; i++)
	{
		if (eventList[i].pFunction != NULL && timeBeatCount > eventList[i].timeout)
		{
			pFun = eventList[i].pFunction;
			
			eventList[i].pFunction = NULL;
			eventList[i].timeout = 0;
			
			pFun();
			break;
		}
	}
}

