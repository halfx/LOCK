/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "main.h"

const item_t main_menu[]={
    "1.", "�û����á�����", "User settings ", YONG_HU_SHE_ZHI_QING_AN,
    "2.", "ϵͳ���á�����", "System setting", XI_TONG_SHE_ZHI_QING_AN,
    "3.", "��¼��ѯ������", "Records query ", JI_LU_CHA_XUN_QING_AN,
    "4.", "��չ���ܡ�����", "Extended func ", KUO_ZHAN_GONG_NENG_QING_AN,
};
const item_t user_menu[]= {
    "1.", "����û����롡", "Add user PIN  ", TIAN_JIA_YONG_HU_MI_MA_QING_AN,
    "2.", "����û�ָ�ơ�", "Add user FP   ", TIAN_JIA_YONG_HU_ZHI_WEN_QING_AN,
    "3.", "����û���Ƭ��", "Add user card ", TIAN_JIA_YONG_HU_KA_PIAN_QING_AN,
    "4.", "ɾ���û����롡", "Del user PIN  ", SHAN_CHU_YONG_HU_MI_MA_QING_AN,
    "5.", "ɾ���û�ָ�ơ�", "Del User FP   ", SHAN_CHU_YONG_HU_ZHI_WEN_QING_AN,
    "6.", "ɾ���û���Ƭ��", "Del user card ", SHAN_CHU_YONG_HU_KA_PIAN_QING_AN,
    "7.", "���ÿ���ģʽ��", "Set Open Mode ", SHE_ZHI_KAI_MEN_MO_SHI_QING_AN,
    "9.", "�޸Ĺ������롡", "Mod master PIN", XIU_GAI_GUAN_LI_MI_MA_QING_AN,
};
const item_t sys_menu[]={
    "1.", "ʱ�����á�����", "Time settings ", SHI_JIAN_SHE_ZHI_QING_AN,
    "2.", "�������á�����", "Date settings ", RI_QI_SHE_ZHI_QING_AN,
    "3.", "�������á�����", "Voice Settings", YU_YIN_SHE_ZHI_QING_AN,
    "4.", "�������á�����", "Language Set  ", YU_YAN_SHE_ZHI_QING_AN,
};
const item_t record_menu[]={
    "1.", "ϵͳ��ѯ������", "System query  ", XI_TONG_CHA_XUN_QING_AN,
    "2.", "˳���ѯ������", "By order      ", SHUN_XU_CHA_XUN_QING_AN,
    "3.", "��Ų�ѯ������", "By number     ", BIAN_HAO_CHA_XUN_QING_AN,
};
const item_t ext_menu[]={
    "1.", "�������硡����", "Add network ", JIA_RU_WANG_LUO,
    "2.", "�˳����硡����", "Del network ", TUI_CHU_WANG_LUO,
};

const item_t Del_menu[]={
    "1.", "����ɾ��������", "Del single    ", DAN_GE_SAHN_CHU_QING_AN,
    "2.", "ȫ��ɾ��������", "Del all       ", QUAN_BU_SHAN_CHU_QING_AN,
};
const item_t Mode_menu[]={
    "1.", "����ģʽ������", "Normal Mode   ", CHANG_YONG_MO_SHI_QING_AN,
    "2.", "��ȫģʽ������", "Safe Mode     ", AN_QUAN_MO_SHI_QING_AN,
};
const item_t Voice_menu[]={
    "1.", "����ģʽ������", "Vioce Mode on ", YU_YIN_MO_SHI_QING_AN,
    "2.", "����ģʽ������", "Vioce Mode off", JING_YIN_MO_SHI_QING_AN,
};
const item_t Language_menu[]={
    "1.", "����ģʽ������", "Chinese       ", ZHONG_WEN_QING_AN,
    "2.", "Ӣ��ģʽ������", "English       ", YING_WEN_QING_AN,
};

//��֤ʧ��
const tips_t TIPS_YZSB={"��֤ʧ��","Verify Failed",YAN_ZHENG_SHI_BAI};

//���˳�����ģʽ
const tips_t TIPS_YTCGLMS={"���˳�����ģʽ","Exit Master Mode",YI_TUI_CHU_GUAN_LI_MO_SHI};

//�������û�������#�ż�����
const tips_t TIPS_QSRYHMMYJHJJS={"����������","Input PIN code",QING_SHU_RU_YONG_HU_MI_MA_YI_JING_HAO_JIAN_JIE_SHU};

//��������λ�����#�ż�����
const tips_t TIPS_QSRLWBHYJHJJS={"��������","Input number",QING_SHU_RU_LIANG_WEI_BIAN_HAO_YI_JING_HAO_JIAN_JIE_SHU};

//�������
const tips_t TIPS_SRCW={"�������","Input error",SHU_RU_CUO_WU};

//������ڼ�
const tips_t TIPS_MMGYJD={"������ڼ�","PIN too simple",MI_MA_GUO_YU_JIAN_DAN};

//����ʧ��
const tips_t TIPS_SZSB={"����ʧ��","Setting failed",SHE_ZHI_SHI_BAI};

//���óɹ�
const tips_t TIPS_SZCG={"���óɹ�","Setting succeed",SHE_ZHI_CHENG_GONG};

//����������
const tips_t TIPS_QCXSR={"����������","Input again",QING_CHONG_XIN_SHU_RU};

//���ÿ���ָ���ٷ�һ��
const tips_t TIPS_QNKSZZFYC={"�ٷ�һ��","Scan again",QING_NA_KAI_SHOU_ZHI_ZAI_FANG_YI_CI};

//������΢΢�ƶ���ָ
const tips_t TIPS_QXZWWYDSZ={"����΢����ָ","FP moves left",QING_XIANG_ZUO_WEI_WEI_YI_DONG_SHOU_ZHI};

//������΢΢�ƶ���ָ
const tips_t TIPS_QXYWWYDSZ={"����΢����ָ","FP moves right",QING_XIANG_YOU_WEI_WEI_YI_DONG_SHOU_ZHI};

//������΢΢�ƶ���ָ
const tips_t TIPS_QXSWWYDSZ={"����΢����ָ","FP moves up",QING_XIANG_SHANG_WEI_WEI_YI_DONG_SHOU_ZHI};

//������΢΢�ƶ���ָ
const tips_t TIPS_QXXWWYDSZ={"����΢����ָ","FP moves down",QING_XIANG_XAI_WEI_WEI_YI_DONG_SHOU_ZHI};

//���ʧ��
const tips_t TIPS_TJSB={"���ʧ��","Add failed",TIAN_JIA_SHI_BAI};

//��ӳɹ�
const tips_t TIPS_TJCG={"��ӳɹ�","Add succeed",TIAN_JIA_CHENG_GONG};

//ɾ��ʧ��
const tips_t TIPS_SCSB={"ɾ��ʧ��","Del failed",SHAN_CHU_SHI_BAI};

//ɾ���ɹ�
const tips_t TIPS_SCCG={"ɾ���ɹ�","Del success",SHAN_CHU_CHENG_GONG};

//ָ�ƿ�Ϊ��
const tips_t TIPS_ZWKWK={"ָ�ƿ�Ϊ��","FP lib empty",ZHI_WEN_KU_WEI_KONG};

//��Ų�����
const tips_t TIPS_BHBCZ={"��Ų�����","Num not exist",BIAN_HAO_BU_CUN_ZAI};

//��Ƭ������
const tips_t TIPS_KPBCZ={"��Ƭ������","Card not exist",0};

//�ѽ������ģʽ
const tips_t TIPS_YJRGLMS={"�ѽ������ģʽ","Master Mode",YI_JIN_RU_GUAN_LI_MO_SHI};

//��֤�ɹ�
const tips_t TIPS_YZCG={"��֤�ɹ�","Verify succeed",YAN_ZHENG_CHENG_GONG_QING_KAI_MEN};

//��������������
const tips_t TIPS_DLDQGHDC={"��������������","Low power",DIAN_LIANG_DI_QING_GENG_HUAN_DIAN_CHI};

//����Ѵ���
const tips_t TIPS_BHYCZ={"����Ѵ���","Num exist",BIAN_HAO_YI_CUN_ZAI};

//ָ���Ѵ���
const tips_t TIPS_ZWYCZ={"ָ���Ѵ���","FP exist",ZHI_WEN_YI_CUN_ZAI};

//��������ָ
const tips_t TIPS_QSRSZ={"��������ָ","Input Fp",QING_AN_ZHI_WEN};

//������ģʽ
const tips_t TIPS_GYMS={"������ģʽ","High volume",GAO_YIN_LIANG_MO_SHI};

//������ģʽ
const tips_t TIPS_DYMS={"������ģʽ","Low volume",DI_YIN_LIANG_MO_SHI};

//����ģʽ
const tips_t TIPS_JYMS={"����ģʽ","Voice guide off",JING_YIN_MO_SHI};

//ϵͳ�����������Ժ�����
const tips_t TIPS_XTYSD={"ϵͳ������","System locked",XI_TONG_YI_SUO_DING_QING_SHAO_HOU_ZAI_SHI};

//������󱨾�
const tips_t TIPS_SRCWBJ={"������󱨾�","In Put Error",0};

//���Ժ�����
const tips_t TIPS_QSHZS={"���Ժ�����","Try It Later",XI_TONG_YI_SUO_DING_QING_SHAO_HOU_ZAI_SHI};

//�����Ѵ���
const tips_t TIPS_MMYCZ={"�����Ѵ���","Pwd exists",0};

//��Ƭ�Ѵ���
const tips_t TIPS_MKYCZ={"��Ƭ�Ѵ���","Card exists",KA_PIAN_YI_CUN_ZAI};

//�������
const tips_t TIPS_MMCW={"�������","Wrong PIN code",0};

//������Ч
const tips_t TIPS_MMWX={"������Ч","Invalid PIN code",0};

//��������һ��
const tips_t TIPS_QZSRYC={"��������һ��","Enter Again",QING_ZAI_SHU_RU_YI_CI};

//�밴ָ��
const tips_t TIPS_QAZW={"�밴ָ��","Input FP",QING_AN_ZHI_WEN};

//��ˢ����ָ��
const tips_t TIPS_QSRZWHKP={"��ˢ����ָ��","FP or card",QING_SHUA_KA_HUO_AN_ZHI_WEN};

//δ���ָ��
const tips_t TIPS_WTJZW={"δ���ָ��","None FP",WEI_TIAN_JIA_ZHI_WEN};

//δ��������
const tips_t TIPS_WSZMM={"δ��������","None PIN code",WEI_SHE_ZHI_MI_MA};

//δ����û���
const tips_t TIPS_WTJYHK={"δ����û���","None card",WEI_TIAN_JIA_YONG_HU_KA};

//����ӹ���������#�ż�����
const tips_t TIPS_QTJGLMM={"����ӹ�������","Add Master PIN",QING_TIAN_JIA_GUAN_LI_MI_MA_YI_JING_HAO_JIAN_JIE_SHU};

//��ˢ��α��
const tips_t TIPS_QSFWM={"��ˢ��α��","Scan Code",QING_SHUA_FANG_WEI_MA};

//������ˢ��
const tips_t TIPS_QCXSK={"������ˢ��","Swipe Again",0};

//�ѻָ�����������
const tips_t TIPS_YHFDCCSZ={"�ѻָ�����������","Factory reset",YI_HUI_FU_DAO_CHU_CHANG_SHE_ZHI};

//���ڻָ������Ժ�
const tips_t TIPS_ZZHFQSH={"���Ժ�","Wait��",ZHNEG_ZAI_HUI_FU_QING_SHAO_HOU};

//���޸Ĺ�������
const tips_t TIPS_QXGGLMM={"���޸Ĺ�������","Modify PIN code",QING_XIU_GAI_GUAN_LI_MI_MA};

//�밴ң����
const tips_t TIPS_QAYKQ={"�밴ң����","Press remote",QING_AN_YAO_KONG_QI};

//��ˢ��
const tips_t TIPS_QSK={"��ˢ��","Contact card",QING_SHUA_KA};

//������ָ�ƻ�������λ��ţ���#�ż�����
const tips_t TIPS_QSRZWHSRLWBHQJHJJS={"����ָ�ƻ���","PIN or FP",QING_SHU_RU_ZHI_WEN_HUO_SHU_RU_LIANG_WEI_BAIN_HAO};

//��ˢ����������λ��ţ���#�ż�����
const tips_t TIPS_QSKHSRLWBHYJHJJS={"��ˢ����������","PIN or card",QING_SHUA_KA_HUO_SHU_RU_LIANG_WEI_BAIN_HAO};

//��Ƭ��Ϊ��
const tips_t TIPS_KPKWK={"��Ƭ��Ϊ��","Cards empty",KA_PIAN_KU_WEI_KONG};

//��Ƭ������
const tips_t TIPS_KPKYM={"��Ƭ������","Cards full",KA_PIAN_KU_YI_MAN};

//��Ƭ�Ѵ���
const tips_t TIPS_KPYCZ={"��Ƭ�Ѵ���","Card exist",KA_PIAN_YI_CUN_ZAI};

//��������п�Ƭ
const tips_t TIPS_YQKSYKP={"��������п�Ƭ","Empty Card",0};

//ָ�ƿ�����
const tips_t TIPS_ZWKYM={"ָ�ƿ�����","FP Lib full",ZHI_WEN_KU_YI_MAN};

//���������
const tips_t TIPS_MMKYM={"���������","PIN Lib Full",MI_MA_KU_YI_MAN};

//�����Ϊ��
const tips_t TIPS_MMKWK={"�����Ϊ��","PIN Lib Empty",MI_MA_KU_WEI_KONG};

//����������
const tips_t TIPS_FZCKS={"����������","Open error",FEI_ZHENG_CHANG_KAI_SUO};

//���˱���
const tips_t TIPS_FQBJ={"���˱���","Tamper Alarm",0};

//�����쳣
const tips_t TIPS_MSYC={"�����쳣","Error",0};

//��������
const tips_t TIPS_BFBJ={"��������","Defence Alarm",0};

//����������
const tips_t TIPS_JXYSKM={"����������","Machine Alarm",FEI_ZHENG_CHANG_KAI_SUO};

//��ˢ����ָ��
const tips_t TIPS_QSKHAZW={"��ˢ����ָ��","Card or Fp",QING_SHUA_KA_HUO_AN_ZHI_WEN};

//�����������Ƭ
const tips_t TIPS_QSRMMHKP={"�����������Ƭ","PIN or Card",QING_SHU_RU_MI_MA_HUO_KA_PIAN};

//�����������ָ��
const tips_t TIPS_QSRMMHZW={"�����������ָ��","PIN or FP",QING_SHU_RU_MI_MA_HUO_ZHI_WEN};

//���ż�¼Ϊ��
const tips_t TIPS_KMJLWK={"���ż�¼Ϊ��","Record Empty",0};

//����
const tips_t TIPS_MM={"����","PIN",0};

//ָ��
const tips_t TIPS_ZW={"ָ��","FP",0};

//��Ƭ
const tips_t TIPS_KP={"��Ƭ","Card",0};

//��еԿ��
const tips_t TIPS_JXYC ={"��еԿ��","Machine",0};

//ң�ؿ���
const tips_t TIPS_YKKM={"ң�ؿ���","Remote",0};

//һ������
const tips_t TIPS_YJKQ={"һ������","One Key",0};

//APP����
const tips_t TIPS_APP={"�ֻ�Ӧ�ÿ���","APP",0};

//δ֪����
const tips_t TIPS_WZLX={"δ֪����","Unknown",0};

//���Ժ�
const tips_t TIPS_QSH={"���Ժ�","Wait...",0};

//ɾ��ʧ�ܣ�Ϊ��ȫģʽ
const tips_t TIPS_WAQMS={"Ϊ��ȫģʽ","Safe Mode",SHAN_CHU_SHI_BAI_WEI_AN_QUAN_MO_SHI};

//ң���Ѵ���
const tips_t TIPS_YKYCZ={"ң���Ѵ���","Remote Exist",0};

//ң�ز�����
const tips_t TIPS_YKBCZ={"ң�ز�����","Remote Not Exist",0};

//ң��������
const tips_t TIPS_YKYPM={"ң��������","Remote Add Full",0};

//����ɾ������
const tips_t TIPS_ZZSCMM={"����ɾ������","Delete PIN",0};

//����ɾ��ָ��
const tips_t TIPS_ZZSCZW={"����ɾ��ָ��","Delete FP",0};

//����ɾ����Ƭ
const tips_t TIPS_ZZSCKP={"����ɾ����Ƭ","Delete Card",0};

//����ɾ�����ż�¼
const tips_t TIPS_ZZSCKMJL={"����ɾ�����ż�¼","Delete Record",0};

//��֤�ɹ�,�뿪��
const tips_t TIPS_QKM={"�뿪��","Open",YAN_ZHENG_CHENG_GONG_QING_KAI_MEN};

//�ѹ���
const tips_t TIPS_YGM={"�ѹ���","Close",YI_GUAN_MEN};

//�����ѿ���
const tips_t TIPS_LYKQ={"�����ѿ���","On Bluetooth",LAN_YA_YI_KAI_QI};

//�����ѹر�
const tips_t TIPS_LYGB={"�����ѹر�","Off Bluetooth",LAN_YA_YI_GUAN_BI};

//����������
const tips_t TIPS_YQDBF={"����������","Defense Mode on",YI_QI_DONG_BU_FANG};

//�ѷ���
const tips_t TIPS_YFS={"�ѷ���","Privacy Mode on",YI_FAN_SUO};

//����ģʽ
const tips_t TIPS_YYMS={"����ģʽ","Voice guide on",0};

//�Զ�ģʽ
const tips_t TIPS_ZDMS={"�Զ�ģʽ","Auto Mode",ZI_DONG_MO_SHI};

//�ֶ�ģʽ
const tips_t TIPS_SDMS={"�ֶ�ģʽ","Manual Mode",SHOU_DONG_MO_SHI};

//��δ����
const tips_t TIPS_MWSS={"��δ����","Not locked",MEN_WEI_SHANG_SUO};


void DisplayTips(const tips_t *tip)
{
    Display_clear_screen();
    if(Function_status()->ManageMode.Language==CHN)
    {
        Display_chinese_str((128-strlen(tip->TipsChinese)*8)/2,1,tip->TipsChinese,false);
    }
    else if(Function_status()->ManageMode.Language==EN)
    {
        Display_english_str((128-strlen(tip->TipsEnglish)*8)/2,1,tip->TipsEnglish,false);
    }
}
void DisplayTipsEx(const tips_t *tip,uint8_t y,bool clr)
{
    if(clr)
        Display_clear_screen();
    if(Function_status()->ManageMode.Language==CHN)
    {
        Display_chinese_str((128-strlen(tip->TipsChinese)*8)/2,y,tip->TipsChinese,false);
    }
    else if(Function_status()->ManageMode.Language==EN)
    {
        Display_english_str((128-strlen(tip->TipsEnglish)*8)/2,y,tip->TipsEnglish,false);
    }
}


/* [] END OF FILE */
