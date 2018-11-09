/**
  ******************************************************************************
  * @file         lock_hal.h
  * @brief        ��lock_hal.c��Ӧ��ͷ�ļ�
  * @author       Fli
  * @date         2018-06-21
  * @version      v001
  * @copyright    kaadas
  *
  * @note         �����ˡ�������
  *
  ******************************************************************************
  */
#ifndef __LOCK_HAL_H__
#define __LOCK_HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

	/* ������Ʋ��� */
#define MOTOR_MAX_LOCKED_TIMES           3    //�����������������ʧ�ܺ󣬻��Զ��ظ�������
#define MOTOR_MAX_ADC_VALUE              238  //�����ת���Ƶ���280mA ��������0.5�� 
#define MOTOR_LOCKED_MORE_TIME           50   //��������ʱ��Ms
#define MOTOR_ABNORMAL_REVERSE_GAP_TIME  100  //����쳣ʱ����ת��϶ʱ��Ms
#define MOTOR_REVERSE_GAP_TIME           1000 //�������ʱ����ת��϶ʱ��Ms
#define MOTOR_RUNNING_TIMEOUT            2000 //����������У���ʱʱ��Ms
#define MOTOR_BACK_TIMEOUT               1000 //����������У���ʱʱ��Ms
#define MOTOR_ADC_DETECT_DELAY_TIME      100   //���������ADC����ӳ�ʱ��Ms
#define MOTOR_BRAKE_TIME                 30   //���ɲ��ʱ��Ms


	/* ����ײ������������¼� */
#define LOCK_EVT_LOCK_CHANGE     0X01 //��״̬�ı�
#define LOCK_EVT_DOOR_CHANGE     0X02 //��״̬�ı�
#define LOCK_EVT_LOCKED_ABNORMAL 0X04 //���������쳣
#define LOCK_EVT_UNLOCK_ABNORMAL 0X08 //���������쳣

	/* ��״̬ */
#define	LOCK_STATUS_LOCKING        0X01 //��������
#define	LOCK_STATUS_LOCKED         0X02 //������
#define	LOCK_STATUS_UNLOCKING      0X04 //���ڿ���
#define	LOCK_STATUS_UNLOCK         0X08 //�ѿ���
#define	LOCK_STATUS_UNKNOWN        0X10 //��״̬δ֪
#define	LOCK_STATUS_LOCKED_MECH    0X20 //����������е��ʽ��
#define	LOCK_STATUS_UNLOCK_MECH    0X40 //�ѿ�������е��ʽ��

	/* ��״̬ */
#define DOOR_STATUS_OPENED       0X01//���Ѵ�
#define DOOR_STATUS_CLOSED       0X02//���ѹر�
#define DOOR_STATUS_CLOSED_LONG  0X04//���ѳ�ʱ��رգ�1.5s��
#define DOOR_STATUS_ABNORMAL     0X08//��״̬�쳣--���˿�

	/* �쳣״̬ */
#define ABNORMAL_STA_TIMEOUT     0X01//�����쳣����ʱ
#define ABNORMAL_STA_AWD         0X02//�����쳣��������ģ�⿴�Ź���
#define ABNORMAL_STA_DOOR_OPEN   0X04//�����쳣�������������ſ���


/* �ص��������� */
typedef void (*LockHalCallBackFunction)(uint8_t event, uint8_t param);


ErrorStatus LockHal_Init(LockHalCallBackFunction cb);
ErrorStatus LockHal_Unlock(void);
ErrorStatus LockHal_UnlockBack(void);
ErrorStatus LockHal_Locked(void);
void LockHal_Scan(void);


#ifdef __cplusplus
}
#endif

#endif /*__LOCK_HAL_H__ */
