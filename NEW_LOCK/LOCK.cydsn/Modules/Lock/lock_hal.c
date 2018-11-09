/**
  ******************************************************************************
  * @file         lock_hal.c
  * @brief        ����Ӳ������������
  * @author       Fli
  * @date         2018-06-21
  * @version      v001
  * @copyright    kaadas
  *
  * @note         �����ˡ�������
  *
  ******************************************************************************
  */
//#include "system.h"

#include "lock_hal.h"

static bool lock_run = true;

#ifdef PLATFORM

/* ������Ӳ��IO���� */
#define SENSOR_LOCKED_GPIO  OP3_INT_Read() //����������IO�������������λ��Ӧ��
#define SENSOR_UNLOCK_GPIO  ((LOCK_INT_Read()&(1<<0))>>0) //OP2_INT_Read() //����������IO��������������λ��Ӧ��
#define SENSOR_BACK_GPIO    OP1_INT_Read() //���ϴ�����IO
#define SENSOR_DOOR_GPIO    ((LOCK_INT_Read()&(1<<3))>>3) //��״̬���IO�����ࣩ

#endif

#define MOTOA_SET(x)    do{\
                            I2C_WriteReg(FL3235_REG_PWM_OUT27, 0xFF);\
                            I2C_WriteReg(FL3235_REG_CTRL_OUT27, I_MAX_1_4|(x));\
                        }while(0)  //MOTO_A�ſ���

#define MOTOB_SET(x)    do{\
                            I2C_WriteReg(FL3235_REG_PWM_OUT26, 0xFF);\
                            I2C_WriteReg(FL3235_REG_CTRL_OUT26, I_MAX_1_4|(x));\
                        }while(0)  //MOTO_B�ſ���

#define NSLEEP_SET(x)   do{\
                            I2C_WriteReg(FL3235_REG_PWM_OUT25, 0xFF);\#ifdef PLATFORM

/* ������Ӳ��IO���� */
#define SENSOR_LOCKED_GPIO  OP3_INT_Read() //����������IO�������������λ��Ӧ��
#define SENSOR_UNLOCK_GPIO  ((LOCK_INT_Read()&(1<<0))>>0) //OP2_INT_Read() //����������IO��������������λ��Ӧ��
#define SENSOR_BACK_GPIO    OP1_INT_Read() //���ϴ�����IO
#define SENSOR_DOOR_GPIO    ((LOCK_INT_Read()&(1<<3))>>3) //��״̬���IO�����ࣩ

#endif

#define MOTOA_SET(x)    do{\
                            I2C_WriteReg(FL3235_REG_PWM_OUT27, 0xFF);\
                            I2C_WriteReg(FL3235_REG_CTRL_OUT27, I_MAX_1_4|(x));\
                        }while(0)  //MOTO_A�ſ���

#define MOTOB_SET(x)    do{\
                            I2C_WriteReg(FL3235_REG_PWM_OUT26, 0xFF);\
                            I2C_WriteReg(FL3235_REG_CTRL_OUT26, I_MAX_1_4|(x));\
                        }while(0)  //MOTO_B�ſ���

#define NSLEEP_SET(x)   do{\
                            I2C_WriteReg(FL3235_REG_PWM_OUT25, 0xFF);\
                            I2C_WriteReg(FL3235_REG_CTRL_OUT25, I_MAX_1_4|(x));\
                          }while(0)//NSLEEP�ſ���

#define T_POWER_CTRL(x) do{\
                            I2C_WriteReg(FL3235_REG_PWM_OUT6, 0xFF);\
                            I2C_WriteReg(FL3235_REG_CTRL_OUT6, I_MAX_1_4|(x));\
                          }while(0)//�����Դ����

/* ������� */
#define LOCK_HAL_MOTOR_RIGHT()  {NSLEEP_SET(HIGH);MOTOB_SET(HIGH);MOTOA_SET(LOW);FL3225_DataUpdate();} //��ת��������
#define LOCK_HAL_MOTOR_LEFT() {NSLEEP_SET(HIGH);MOTOB_SET(LOW);MOTOA_SET(HIGH);FL3225_DataUpdate();} //��ת��������
#define LOCK_HAL_MOTOR_STOP()  {NSLEEP_SET(HIGH);MOTOB_SET(HIGH);MOTOA_SET(HIGH);FL3225_DataUpdate();} //ֹͣ
#define LOCK_HAL_MOTOR_BRAKE() {NSLEEP_SET(HIGH);MOTOB_SET(HIGH);MOTOA_SET(HIGH);FL3225_DataUpdate();} //ɲ��

#define Power_LockSensorControl(endis)  {T_POWER_CTRL(endis);FL3225_DataUpdate();}    //�����Դ����
                            I2C_WriteReg(FL3235_REG_CTRL_OUT25, I_MAX_1_4|(x));\
                          }while(0)//NSLEEP�ſ���

#define T_POWER_CTRL(x) do{\
                            I2C_WriteReg(FL3235_REG_PWM_OUT6, 0xFF);\
                            I2C_WriteReg(FL3235_REG_CTRL_OUT6, I_MAX_1_4|(x));\
                          }while(0)//�����Դ����

/* ������� */
#define LOCK_HAL_MOTOR_RIGHT()  {NSLEEP_SET(HIGH);MOTOB_SET(HIGH);MOTOA_SET(LOW);FL3225_DataUpdate();} //��ת��������
#define LOCK_HAL_MOTOR_LEFT() {NSLEEP_SET(HIGH);MOTOB_SET(LOW);MOTOA_SET(HIGH);FL3225_DataUpdate();} //��ת��������
#define LOCK_HAL_MOTOR_STOP()  {NSLEEP_SET(HIGH);MOTOB_SET(HIGH);MOTOA_SET(HIGH);FL3225_DataUpdate();} //ֹͣ
#define LOCK_HAL_MOTOR_BRAKE() {NSLEEP_SET(HIGH);MOTOB_SET(HIGH);MOTOA_SET(HIGH);FL3225_DataUpdate();} //ɲ��

#define Power_LockSensorControl(endis)  {T_POWER_CTRL(endis);FL3225_DataUpdate();}    //�����Դ����


/* ���������� */
typedef enum
{
	SENSOR_TYPE_DOOR = 0,    //���ഫ��������״̬��⣩
	SENSOR_TYPE_LOCKED = 1,  //�����������������������������
	SENSOR_TYPE_UNLOCK = 2,  //������������������������������
	SENSOR_TYPE_BACK = 3,    //���ϴ�����
	SENSOR_MAX_QTY,
} SensorType_enum_t;

/* ������״̬ */
typedef enum
{
	SENSOR_STA_RELEASE,     //״̬Ϊ�ɿ���δ�ڵ���
	SENSOR_STA_PRESS,       //״̬Ϊ���£��ڵ���
	SENSOR_STA_LONG_RELEASE,//״̬Ϊ���ɿ���1.5s��
	SENSOR_STA_LONG_PRESS,  //״̬Ϊ������
	SENSOR_STA_UNKNOWN,     //״̬δ֪
} SensorStatus_enum_t;

/* �����ǰ����״̬ */
__IO static enum
{
	MOTOR_STA_UNLOCKING,       //���ڿ���
	MOTOR_STA_UNLOCK_BACK,     //���ڿ�������
	MOTOR_STA_WAIT_UNLOCK_BACK,//���ڵȴ���������
	MOTOR_STA_UNLOCKED,    //�ѿ���
	MOTOR_STA_LOCKING,     //��������
	MOTOR_STA_LOCKED_BACK, //������������
	MOTOR_STA_LOCKED,      //������
	MOTOR_STA_UNKNOWN,     //δ֪״̬
} motorStatus;

/* ���嵱ǰ����״̬ */
__IO static enum
{
	LOCK_STA_UNLOCKING,   //���ڿ�����������æ��
	LOCK_STA_LOCKING,     //����������������æ��
	LOCK_STA_IDLE,        //δִ���κζ���
    LOCK_STA_INIT,        //��ʼ״̬
} lockStatus;

__IO static SensorStatus_enum_t sensorRealStatus[(int)SENSOR_MAX_QTY]; //������ʵʱ״̬
__IO static LockHalCallBackFunction pFunctionCb; //�ص�
__IO static uint8_t repeatLockedCnt;             //�ظ���������������ʧ�ܻ��ظ�������
__IO static uint8_t awdEventFlag;                //AWD�¼���־
__IO static uint8_t unlockOkFlag;                //�����ɹ���־
__IO static uint8_t abnormalEvent;               //�����쳣�¼�
__IO static uint8_t firstFlag;                   //�����״��ϵ�У׼���

/* ���غ������� */
static uint8_t LockHal_MotorUnlock(void);
static uint8_t LockHal_MotorLocked(void);
static void LockHal_MotorRepeatLocked(void);
static void LockHal_AbnormalProcess(void);


/**
  * @brief  ģ�⿴�Ź��ص�����
  *
  * @note   ����������ʱ������ģ�⿴�Ź�
	*         ����ģ�⿴�Ź��¼����Զ�ִ�иú���
  */
//static void LockHal_AdcDogCB(void)
//{
//	if (awdEventFlag == 0)
//	{
//		awdEventFlag = 1; //��ǵ������
//	}
//	else
//	{
//		AdcHal_StopAdcDog();//�ر�ģ�⿴�Ź�
//	}
//}

/**
  * @brief  ����ģ�⿴�Ź�
  *
  * @note   PC5 -- ADC_IN15
  */
static void LockHal_AdcDogStart(void)
{
//	if (AdcHal_InitChangeAndDog(1,LockHal_AdcDogCB) != SUCCESS)
//	{
//		LockEvt_Create(LockHal_AdcDogStart, 10);//10ms���ٴ�����
//		return;
//	}
    uint16_t adcval = 0;
    
    awdEventFlag = 0;//�������
    
    if (AdcHal_InitChange(1))
    {
        adcval = AdcHal_ReadResult(1);
        
        if (adcval > MOTOR_MAX_ADC_VALUE)
        {
            awdEventFlag = 1; //����
        }
        else
        {
            awdEventFlag = 0;    
        }
    }
    LockEvt_Create(LockHal_AdcDogStart, 10);//10ms���ٴ�����
}

/**
  * @brief  ֹͣ���
  *
  * @note
  */
static void LockHal_MotorStop(void)
{
	LOCK_HAL_MOTOR_STOP(); //ͣ���
//	AdcHal_StopAdcDog();   //�ر�ģ�⿴�Ź�

	if (motorStatus == MOTOR_STA_UNLOCK_BACK)
	{
		/* ����¼�����ǰ�������̽��� */
		LockEvt_Clear();
		motorStatus = (unlockOkFlag == 1) ? MOTOR_STA_UNLOCKED : MOTOR_STA_UNKNOWN;

		if (lockStatus == LOCK_STA_UNLOCKING && unlockOkFlag == 0)
		{
			/* �ص�֪ͨ������ʧ�� */
			pFunctionCb(LOCK_EVT_UNLOCK_ABNORMAL, abnormalEvent);
			if (sensorRealStatus[(int)SENSOR_TYPE_LOCKED] == SENSOR_STA_PRESS || sensorRealStatus[(int)SENSOR_TYPE_LOCKED] == SENSOR_STA_LONG_PRESS)
			{
				pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_LOCKED);
			}
			else
			{
				pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNKNOWN);
			}
		}
		else if (lockStatus == LOCK_STA_LOCKING)
		{
			if (unlockOkFlag == 1)
			{
				pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNLOCK);
			}
			else
			{
				pFunctionCb(LOCK_EVT_LOCKED_ABNORMAL, abnormalEvent);//�ص�֪ͨ������ʧ��
				pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNKNOWN);
			}
		}
		lockStatus = LOCK_STA_IDLE;
	}
	else if (motorStatus == MOTOR_STA_LOCKED_BACK)
	{
		/* ����¼�����ǰ�������̽��� */
		LockEvt_Clear();

		/* �ж�����Ͳ��ഫ����״̬ */
		if ( (sensorRealStatus[(int)SENSOR_TYPE_LOCKED] == SENSOR_STA_PRESS || sensorRealStatus[(int)SENSOR_TYPE_LOCKED] == SENSOR_STA_LONG_PRESS)
		        && (sensorRealStatus[(int)SENSOR_TYPE_DOOR] == SENSOR_STA_RELEASE || sensorRealStatus[(int)SENSOR_TYPE_DOOR] == SENSOR_STA_LONG_RELEASE) )
		{
			motorStatus = MOTOR_STA_LOCKED;
		}
		else
		{
			motorStatus = MOTOR_STA_UNKNOWN;
		}

		if (lockStatus == LOCK_STA_LOCKING)
		{
			if (motorStatus == MOTOR_STA_LOCKED)//�����ɹ�
			{
				lockStatus = LOCK_STA_IDLE;
				pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_LOCKED);//�ص�֪ͨ����״̬�ı䣬�����ɹ�
			}
			else
			{
				pFunctionCb(LOCK_EVT_LOCKED_ABNORMAL, abnormalEvent);//�ص�֪ͨ������ʧ��
				if (sensorRealStatus[(int)SENSOR_TYPE_DOOR] == SENSOR_STA_PRESS || sensorRealStatus[(int)SENSOR_TYPE_DOOR] == SENSOR_STA_LONG_PRESS)
				{
					/* ����ʧ�ܣ������˻�ȥ */
					if (LockHal_MotorUnlock() == 0)
					{
						lockStatus = LOCK_STA_IDLE;
						pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNLOCK);
					}
				}
				else
				{
					LockEvt_Create(LockHal_MotorRepeatLocked, MOTOR_REVERSE_GAP_TIME);
				}
			}
		}
	}
}

/**
  * @brief  ֹͣ����
  *
  * @note   1.�������ã����ϴ�������λ����øú���
  *         2.��ʱ���ã����ϳ�ʱ����øú���
  */
static void LockHal_MotorBackStop(void)
{
	LOCK_HAL_MOTOR_BRAKE();//����ɲ��

	LockEvt_Clear();
	LockEvt_Create(LockHal_MotorStop, MOTOR_BRAKE_TIME);//30ms��ֹͣ���
}

/**
  * @brief  �����������
  *
  * @note
  */
static void LockHal_MotorBackStart(void)
{
	switch (motorStatus)
	{
		/* �������ִ������ */
	case MOTOR_STA_LOCKING:
		motorStatus = MOTOR_STA_LOCKED_BACK;
		LOCK_HAL_MOTOR_RIGHT();
		LockEvt_Clear();
		LockEvt_Create(LockHal_MotorBackStop, MOTOR_BACK_TIMEOUT);//�������ϳ�ʱ
		break;

		/* �������ִ�п��� */
	case MOTOR_STA_UNLOCKING:
		if ( (sensorRealStatus[(int)SENSOR_TYPE_UNLOCK] == SENSOR_STA_PRESS || sensorRealStatus[(int)SENSOR_TYPE_UNLOCK] == SENSOR_STA_LONG_PRESS) )
		{
			unlockOkFlag = 1;
		}

		if (lockStatus == LOCK_STA_UNLOCKING && unlockOkFlag == 1)
		{
			/* �����ɹ����ص�֪ͨ���������̽��� */
			LockEvt_Clear();
			lockStatus = LOCK_STA_IDLE;
			pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNLOCK);

			/* �����û�д򿪣����õ��״̬Ϊ�ȴ��������� */
			if ((sensorRealStatus[(int)SENSOR_TYPE_DOOR] == SENSOR_STA_RELEASE || sensorRealStatus[(int)SENSOR_TYPE_DOOR] == SENSOR_STA_LONG_RELEASE))
			{
				motorStatus = MOTOR_STA_WAIT_UNLOCK_BACK;
				break;
			}
		}

		/* ������ڵȴ��������� */
	case MOTOR_STA_WAIT_UNLOCK_BACK:
		motorStatus = MOTOR_STA_UNLOCK_BACK;
		LOCK_HAL_MOTOR_LEFT();
		LockEvt_Clear();
		LockEvt_Create(LockHal_MotorBackStop, MOTOR_BACK_TIMEOUT);//�������ϳ�ʱ
		break;

	default:
		break;
	}
}

/**
  * @brief  �����쳣����
  *
  * @note  �쳣�¼�
  *         ABNORMAL_STA_DOOR_OPEN 0X04�������쳣�������������ſ��ˣ�
  *         ABNORMAL_STA_AWD       0X02�������쳣������-ģ�⿴�Ź���
  *         ABNORMAL_STA_TIMEOUT   0X01��������������ʱ��
  */
static void LockHal_AbnormalProcess(void)
{
	LockHal_MotorStop();
	LockEvt_Clear();
	LockEvt_Create(LockHal_MotorBackStart, MOTOR_ABNORMAL_REVERSE_GAP_TIME);
}

/**
  * @brief  �������״̬
  *
  * @note   ��һ���ϵ���Ҫ��ȡ����״̬��֪ͨ�ϲ�
  *         ����ȴ�����״̬�ȶ���⵽���ϵ�20ms�󣩣��ٵ���
  */
static void LockHal_MotorCheck(void)
{
	if (sensorRealStatus[(int)SENSOR_TYPE_DOOR] == SENSOR_STA_RELEASE)
	{
		pFunctionCb(LOCK_EVT_DOOR_CHANGE, DOOR_STATUS_CLOSED);
	}
    
    if (sensorRealStatus[(int)SENSOR_TYPE_BACK] == SENSOR_STA_PRESS
	        || sensorRealStatus[(int)SENSOR_TYPE_BACK] == SENSOR_STA_LONG_PRESS)
	{
		if ( sensorRealStatus[(int)SENSOR_TYPE_LOCKED] == SENSOR_STA_PRESS
		        || sensorRealStatus[(int)SENSOR_TYPE_LOCKED] == SENSOR_STA_LONG_PRESS)
		{
			motorStatus = MOTOR_STA_LOCKED;//����״̬Ϊ������
		}
		else if ( sensorRealStatus[(int)SENSOR_TYPE_UNLOCK] == SENSOR_STA_PRESS
		          || sensorRealStatus[(int)SENSOR_TYPE_UNLOCK] == SENSOR_STA_LONG_PRESS)
		{
//			motorStatus = MOTOR_STA_WAIT_UNLOCK_BACK;//����״̬Ϊ�ѿ���
            motorStatus = MOTOR_STA_UNLOCKED;//����״̬Ϊ�ѿ���
		}
	}

	if (motorStatus == MOTOR_STA_LOCKED)
	{
		pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_LOCKED_MECH);//�ص�֪ͨ
		firstFlag = 0XFF;
	}
//	else if (motorStatus == MOTOR_STA_WAIT_UNLOCK_BACK)
    else if (motorStatus == MOTOR_STA_UNLOCKED)
	{
		pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNLOCK_MECH);//�ص�֪ͨ
		firstFlag = 0XFF;
	}
	else
	{
		firstFlag = 0;//�����ҪУ׼����λ��
		pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNKNOWN);//�ص�֪ͨ
	}
    lockStatus = LOCK_STA_IDLE;
}

/**
  * @brief  �����������
  * @note
  *
  * @return ��ǰ���λ���Ѵ��������Ŀ���״̬����0�����򷵻�1
  */
static uint8_t LockHal_MotorUnlock(void)
{
	LockEvt_Clear();

	if ( sensorRealStatus[(int)SENSOR_TYPE_UNLOCK] == SENSOR_STA_PRESS
	        || sensorRealStatus[(int)SENSOR_TYPE_UNLOCK] == SENSOR_STA_LONG_PRESS)
	{
		if (sensorRealStatus[(int)SENSOR_TYPE_BACK] == SENSOR_STA_PRESS
		        || sensorRealStatus[(int)SENSOR_TYPE_BACK] == SENSOR_STA_LONG_PRESS)
		{
			motorStatus = MOTOR_STA_UNLOCKED;//�ѿ���
			return 0;
		}
	}

	if (firstFlag == 0)
	{
		firstFlag = 1;
	}
	else
	{
		firstFlag = 0XFF;
	}

	unlockOkFlag = 0;
	motorStatus = MOTOR_STA_UNLOCKING;  //����״̬Ϊ���ڿ���
	LOCK_HAL_MOTOR_RIGHT();             //�����������

	LockEvt_Create(LockHal_AdcDogStart, MOTOR_ADC_DETECT_DELAY_TIME);//100ms�������������

	abnormalEvent = ABNORMAL_STA_TIMEOUT;
    
	LockEvt_Create(LockHal_AbnormalProcess, MOTOR_RUNNING_TIMEOUT);  //����2s��ִ���쳣����
	return 1;
}

/**
  * @brief  �����������
  * @note
  *
  * @return ��ǰ���λ���Ѵ�������������״̬����0�����򷵻�1
  */
static uint8_t LockHal_MotorLocked(void)
{
	LockEvt_Clear();

	if ( sensorRealStatus[(int)SENSOR_TYPE_LOCKED] == SENSOR_STA_PRESS
	        || sensorRealStatus[(int)SENSOR_TYPE_LOCKED] == SENSOR_STA_LONG_PRESS)
	{
		if (sensorRealStatus[(int)SENSOR_TYPE_BACK] == SENSOR_STA_PRESS
		        || sensorRealStatus[(int)SENSOR_TYPE_BACK] == SENSOR_STA_LONG_PRESS)
		{
			motorStatus = MOTOR_STA_LOCKED;//����״̬Ϊ������
			return 0;
		}
	}

	if (firstFlag == 0)
	{
		firstFlag = 1;
	}
	else
	{
		firstFlag = 0XFF;
	}

	motorStatus = MOTOR_STA_LOCKING;  //����״̬Ϊ��������
	LOCK_HAL_MOTOR_LEFT();            //�����������

	LockEvt_Create(LockHal_AdcDogStart, MOTOR_ADC_DETECT_DELAY_TIME);//100ms�������������

	abnormalEvent = ABNORMAL_STA_TIMEOUT;
	LockEvt_Create(LockHal_AbnormalProcess, MOTOR_RUNNING_TIMEOUT);  //����2s��ִ���쳣����
	return 1;
}

/**
  * @brief  ��������
  *
  * @note   �����쳣�󣬻Ὣ����ص�����״̬���ٵ��øú�����������
  *         ��δ�������������
  */
static void LockHal_MotorRepeatLocked(void)
{
	if ( (++repeatLockedCnt < MOTOR_MAX_LOCKED_TIMES)
	        && (sensorRealStatus[(int)SENSOR_TYPE_DOOR] == SENSOR_STA_RELEASE || sensorRealStatus[(int)SENSOR_TYPE_DOOR] == SENSOR_STA_LONG_RELEASE) )
	{
		if (LockHal_MotorLocked() == 0)
		{
			pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_LOCKED);//�ص�֪ͨ����״̬�ı䣬������
			lockStatus = LOCK_STA_IDLE;
		}
	}
	else
	{
		/* �������̽��� */
		if (sensorRealStatus[(int)SENSOR_TYPE_UNLOCK] == SENSOR_STA_PRESS || sensorRealStatus[(int)SENSOR_TYPE_UNLOCK] == SENSOR_STA_LONG_PRESS)
		{
			pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNLOCK);//�ص�֪ͨ����״̬�ı䣬�ѿ���
		}
		else
		{
			pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNKNOWN);//�ص�֪ͨ����״̬�ı䣬״̬δ֪
		}
		lockStatus = LOCK_STA_IDLE;
	}
}

/**
  * @brief  ������״̬�ı䴦��
  *
  * @note   1.�����������⵽�½��أ���ǰΪ����������50ms��ֹͣ���������
  *         2.������������⵽�½��أ���ǰΪ���ڿ���������ֹͣ���������
  *         3.���ഫ������⵽�½��أ��ſ��ˣ���
  *           ��ǰ����������ִ���쳣����
  *           ��ǰ���������ű��ˣ�����
  *         4.���ഫ������⵽��ʱ���ɿ����Źز�����1.5s�����ص�֪ͨ
  *         5.���ϴ�������⵽�½��أ���ǰ���ڿ������ϻ�������ϣ�����ֹͣ����
  *
  * @param  sensorType������������
  * @param  sensorSta����������ǰ״̬
  */
static void LockHal_SensorChangeProcess(SensorType_enum_t sensorType, SensorStatus_enum_t sensorSta)
{
	if (sensorType == SENSOR_TYPE_LOCKED)
	{
		if (sensorSta == SENSOR_STA_PRESS)
		{
			if (motorStatus == MOTOR_STA_LOCKING)
			{
				/* ������λ�� */
				if (firstFlag != 1)//����������У׼״̬�£���λ��Ҳ��ͣ������ȴ�������ͣ
				{
					LockEvt_Create(LockHal_MotorStop, MOTOR_LOCKED_MORE_TIME);//����---50ms��ֹͣ���
					LockEvt_Create(LockHal_MotorBackStart, MOTOR_REVERSE_GAP_TIME + MOTOR_LOCKED_MORE_TIME);//1050ms�����
				}
			}
			else if (lockStatus == LOCK_STA_IDLE)
			{
				pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_LOCKED_MECH);//��е��ʽ����
				motorStatus = MOTOR_STA_LOCKED;
			}
		}
		else if (sensorSta == SENSOR_STA_RELEASE)
		{
			if (lockStatus == LOCK_STA_IDLE && motorStatus == MOTOR_STA_LOCKED)
			{
				pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNKNOWN);//��е��ʽ��������״̬δ֪
				motorStatus = MOTOR_STA_UNKNOWN;
			}
		}
	}
	else if (sensorType == SENSOR_TYPE_UNLOCK && sensorSta == SENSOR_STA_PRESS)
	{
		if (motorStatus == MOTOR_STA_UNLOCKING)
		{
			/* ������λ�� */
			if (firstFlag != 1)//����������У׼״̬�£���λ��Ҳ��ͣ������ȴ�������ͣ
			{
				LockHal_MotorStop();//ֹͣ���
				LockEvt_Create(LockHal_MotorBackStart, MOTOR_REVERSE_GAP_TIME);//1000ms�����
			}
		}
		else if (lockStatus == LOCK_STA_IDLE)
		{
			pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNLOCK_MECH); //��е��ʽ����
			motorStatus = MOTOR_STA_UNLOCKED;
		}
	}
	else if (sensorType == SENSOR_TYPE_DOOR) //��״̬�ı䣨���ද����
	{
		switch (sensorSta)
		{
		case SENSOR_STA_PRESS:
			if (motorStatus == MOTOR_STA_LOCKING || motorStatus == MOTOR_STA_LOCKED_BACK)
			{
				abnormalEvent = ABNORMAL_STA_DOOR_OPEN;
				if (motorStatus == MOTOR_STA_LOCKING)
				{
					LockHal_AbnormalProcess();//ִ���쳣���������������ſ��ˣ�
				}
			}
			else if (motorStatus == MOTOR_STA_WAIT_UNLOCK_BACK)
			{
				LockHal_MotorBackStart();//�ſ��ˣ�ִ�п�������
			}
			else if (sensorRealStatus[(int)SENSOR_TYPE_LOCKED] == SENSOR_STA_PRESS || sensorRealStatus[(int)SENSOR_TYPE_LOCKED] == SENSOR_STA_LONG_PRESS)
			{
				if (lockStatus == LOCK_STA_IDLE)
				{
					pFunctionCb(LOCK_EVT_DOOR_CHANGE, DOOR_STATUS_ABNORMAL);//�ű���
					break;
				}
			}
			pFunctionCb(LOCK_EVT_DOOR_CHANGE, DOOR_STATUS_OPENED);
			break;

		case SENSOR_STA_RELEASE:
			pFunctionCb(LOCK_EVT_DOOR_CHANGE, DOOR_STATUS_CLOSED);
			break;
		case SENSOR_STA_LONG_RELEASE:
			pFunctionCb(LOCK_EVT_DOOR_CHANGE, DOOR_STATUS_CLOSED_LONG);
			break;
		default:
			break;
		}
	}
	else if (sensorType == SENSOR_TYPE_BACK && sensorSta == SENSOR_STA_PRESS)
	{
		if (motorStatus == MOTOR_STA_UNLOCK_BACK || motorStatus == MOTOR_STA_LOCKED_BACK)
		{
			LockHal_MotorBackStop();//ֹͣ����
		}
	}
}

/**
  * @brief  ���崫����ɨ�裬�ú�������ʹ�ö�ʱ���ж�ÿ���2ms����һ��
  *
  * @note   ��ȡ��������ƽ״̬��ȥ����ˢ��״̬
	*         ���ݴ�����״̬�仯��������Ӧ�ĵ������
  */
static void LockHal_SensorScan(void)
{
	static uint8_t  gpioBuf[(int)SENSOR_MAX_QTY] = {0};
	static uint16_t timing[(int)SENSOR_MAX_QTY] = {0};//��ʱ��
	uint8_t i;

	for (i=0; i<(int)SENSOR_MAX_QTY; i++)
	{
		gpioBuf[i] <<= 1;
		switch (i)
		{
		case SENSOR_TYPE_LOCKED:
			gpioBuf[i] |= SENSOR_LOCKED_GPIO;
			break;
		case SENSOR_TYPE_UNLOCK:
			gpioBuf[i] |= SENSOR_UNLOCK_GPIO;
			break;
		case SENSOR_TYPE_BACK:
			gpioBuf[i] |= SENSOR_BACK_GPIO;
			break;
		case SENSOR_TYPE_DOOR:
			gpioBuf[i] |= SENSOR_DOOR_GPIO;
			break;
		}

		if (gpioBuf[i] == 0)
		{
			/* Ӳ��IO�ȶ��͵�ƽ */
//      if (sensorRealStatus[i] == SENSOR_STA_UNKNOWN)
			if (sensorRealStatus[i] != SENSOR_STA_PRESS && sensorRealStatus[i] != SENSOR_STA_LONG_PRESS)
			{
				timing[i] = 0;
				sensorRealStatus[i] = SENSOR_STA_PRESS;//���Ϊ����
				LockHal_SensorChangeProcess((SensorType_enum_t)i, sensorRealStatus[i]);
			}

			/* ����״̬��ʱ */
			if (timing[i] < (1500/TIME_BASIC_MS))
			{
				timing[i]++;
				if (timing[i] >= (1500/TIME_BASIC_MS))  //����ʱ������Ϊ750 * 2 = 1500MS
				{
					sensorRealStatus[i] = SENSOR_STA_LONG_PRESS;//���Ϊ����
					LockHal_SensorChangeProcess((SensorType_enum_t)i, sensorRealStatus[i]);
				}
			}
		}
		else if (gpioBuf[i] == 0XFF)
		{
			/* Ӳ��IO�ȶ��ߵ�ƽ */
//      if (sensorRealStatus[i] == SENSOR_STA_UNKNOWN)
			if (sensorRealStatus[i] != SENSOR_STA_RELEASE && sensorRealStatus[i] != SENSOR_STA_LONG_RELEASE)
			{
				timing[i] = 0;
				sensorRealStatus[i] = SENSOR_STA_RELEASE;//���Ϊ�ɿ�
				LockHal_SensorChangeProcess((SensorType_enum_t)i, sensorRealStatus[i]);
			}

			/* �ɿ�״̬��ʱ */
			if (timing[i] < 1500/TIME_BASIC_MS)
			{
				timing[i]++;
				if (timing[i] == 1500/TIME_BASIC_MS)  //����ʱ������Ϊ750 * 2 = 1500MS
				{
					sensorRealStatus[i] = SENSOR_STA_LONG_RELEASE;//���Ϊ���ɿ�
					LockHal_SensorChangeProcess((SensorType_enum_t)i, sensorRealStatus[i]);
				}
			}
		}
		else
		{
			/* Ӳ��IO״̬δ֪ */
//			sensorRealStatus[i] = SENSOR_STA_UNKNOWN;
		}
	}
}

/**
  * @brief  �����ӿ�
  *
  * @note   ���øú�����ִ�п������̣��������ͨ���ص�֪ͨӦ�ò�
  * @return ��ǰ�����æ������ERROR�����򷵻�SUCCESS
  */
ErrorStatus LockHal_Unlock(void)
{
	ErrorStatus result = ERROR;

//	TIM4->CR1 &= ~1;//��ͣ��ʱ������Ҫ���ж�
    lock_run = false;

	if (lockStatus == LOCK_STA_IDLE)
	{
		if (motorStatus != MOTOR_STA_WAIT_UNLOCK_BACK)
		{
			if (LockHal_MotorUnlock() != 0)
			{
				lockStatus = LOCK_STA_UNLOCKING;//��ǵ�ǰ����ִ�п���
				pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNLOCKING);//�ص�֪ͨ
                result = SUCCESS;
			}
			else
			{
				pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNLOCK);  //�ص�֪ͨ
			}
		}
		else
		{
			pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_UNLOCK);  //�ص�֪ͨ
		}
//		result = SUCCESS;
	}

//	TIM4->CR1 |= 1;//������ʱ��
    lock_run = true;
	return result;
}

/**
  * @brief  �������Ͻӿ�
  *
  * @note   ֻ�е���������ڿ���״̬�£�����δ���ϣ�ִ�иú�������Ч
  * @return ��������SUCCESS, ʧ�ܷ���ERROR
  */
ErrorStatus LockHal_UnlockBack(void)
{
	ErrorStatus result = ERROR;

//	TIM4->CR1 &= ~1;//��ͣ��ʱ������Ҫ���ж�
    lock_run = false;
	if (motorStatus == MOTOR_STA_WAIT_UNLOCK_BACK)
	{
		LockHal_MotorBackStart();
		result = SUCCESS;
	}
//	TIM4->CR1 |= 1;//������ʱ��
    lock_run = true;
	return result;
}

/**
  * @brief  �����ӿ�
  *
  * @note   ���øú�����ִ���������̣��������ͨ���ص�֪ͨӦ�ò�
  * @return ��ǰ�����æ������ERROR�����򷵻�SUCCESS
  */
ErrorStatus LockHal_Locked(void)
{
	ErrorStatus result = ERROR;

//	TIM4->CR1 &= ~1;//��ͣ��ʱ������Ҫ���ж�
    
    lock_run = false;
    
	if (lockStatus == LOCK_STA_IDLE)
	{
		repeatLockedCnt = 0;              //�����ظ���������
		if (LockHal_MotorLocked() != 0)
		{
			lockStatus = LOCK_STA_LOCKING;  //��ǵ�ǰ����ִ������
			pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_LOCKING);//�ص�֪ͨ
		}
		else
		{
			pFunctionCb(LOCK_EVT_LOCK_CHANGE, LOCK_STATUS_LOCKED);//�ص�֪ͨ
		}
		result = SUCCESS;
	}

//	TIM4->CR1 |= 1;//������ʱ��
    lock_run = true;
    
	return result;
}

/**
  * @brief  ����Ӳ�����ʼ��
  *
  * @note   ������GPIO
	*         PC0 -- �����Ӧ
	*         PC1 -- ���ϸ�Ӧ
	*         PC2 -- ������������Ӧ
	*         PC3 -- �����������Ӧ
	*         PC5 -- ���������⣨ADC_IN15��
  *
  * @param  cb���ص�����
  * @return �������󷵻�ERROR����������SUCCESS
  */
ErrorStatus LockHal_Init(LockHalCallBackFunction cb)
{
	static uint8_t resetFlag;
	uint8_t i;

	/* ���ûص� */
	if (cb == NULL)
	{
		return ERROR;
	}
	pFunctionCb = cb;

	/* ��һ���ϵ磺ִ�����帴λ */
	if (resetFlag == 0)
	{
		resetFlag = 1;
		LockEvt_Create(LockHal_MotorCheck, 30);

		/* ״̬��ʼ�� */
		for (i=0; i<(int)SENSOR_MAX_QTY; i++)
		{
			sensorRealStatus[i] = SENSOR_STA_UNKNOWN;
		}
        sensorRealStatus[SENSOR_TYPE_DOOR] = SENSOR_STA_RELEASE;
		motorStatus = MOTOR_STA_UNKNOWN;
//		lockStatus = LOCK_STA_IDLE;
        lockStatus = LOCK_STA_INIT;
	}

	/* ��������Դ�ر� */
	Power_LockSensorControl(ENABLE);

	return SUCCESS;
}

/**
  * @brief  timer4�жϷ�����
  *
  * @note   ����2ms
  */
void LockHal_Scan(void)
{
    if (lock_run)
    {
    //	TIM4->SR = 0;

    /* ɨ�贫���� */
    //	Power_LockSensorControl(ENABLE);
    	LockHal_SensorScan();
    //	Power_LockSensorControl(DISABLE);

    	/* ��ʱ�¼��¼����� */
    	LockEvt_TimeUpdate();

    	/* ����������� */
    	if (awdEventFlag == 1)
    	{
    		awdEventFlag = 0;
            
//            LockEvt_Clear();

    		abnormalEvent = ABNORMAL_STA_AWD;
            
    		LockHal_AbnormalProcess();//ִ���쳣�������������
    	}
    }
}

