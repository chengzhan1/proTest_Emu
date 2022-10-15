
#ifndef _PROTOCOL_BAMS_H_
#define _PROTOCOL_BAMS_H_

#define BMS_MX_CPW 0						 //��ط�ϵͳ n ��������繦��
#define BMS_MX_DPW 1						 //��ط�ϵͳ n �������ŵ繦��
#define BMS_CONN_HEARTBEAT 2				 //��ط�ϵͳ n ͨѶ����
#define BMS_MAIN_VOLTAGE 3					 //��ط�ϵͳ n �ܵ�ѹ
#define BMS_MX_CCURRENT 4					 //��ط�ϵͳ n ������������
#define BMS_MX_DCURRENT 5					 //��ط�ϵͳ n �������ŵ����
#define BMS_SUM_CURRENT 6					 //��ط�ϵͳ n ����ܵ���
#define BMS_SOC 7							 //��ط�ϵͳ n ��� SOC
#define BMS_remaining_charging_capacity 8	 //��ط�ϵͳ n ���ʣ��ɳ����
#define BMS_remaining_discharging_capacity 9 //��ط�ϵͳ n ���ʣ��ɷŵ���
#define BMS_single_MX_voltage 10			 //��ط�ϵͳ n ������ߵ�ѹ
#define BMS_single_MI_voltage 11			 //��ط�ϵͳ n ������͵�ѹ
#define BMS_SYS_STATUS 12					 //��ط�ϵͳ n ״̬0-��ʼ�� 1-ͣ�� 2-������ 3-���� 4-���� 5-���� 9-�ػ� 255-����
#define BMS_SYS_NEED 13						 //��ط�ϵͳ n ���� 0-�������(PCS��ֹ���ŵ�, PCSӦͣ���������) 1-ֻ�ܳ�磨PCS��ֹ�ŵ磩 2-ֻ�ܷŵ磨PCS��ֹ��磩 3-�ɳ�ɷţ�������
#define BMS_FAULT_STATUS 14					 //��ط�ϵͳ n �ܹ���״̬
enum _BMS_SYS_STATUS //��ط�ϵͳ״̬
{
	BMS_ST_INIT = 0,	 //-��ʼ��
	BMS_ST_PAUSE = 1,	 //-ͣ��
	BMS_ST_START = 2,	 //-������
	BMS_ST_WORKING = 3,	 //-����
	BMS_ST_WAITTING = 4, //-����
	BMS_ST_FAULT = 5,	 //-����
	BMS_ST_SHUTDOWN = 9, //-�ػ�
	BMS_ST_TEST = 255	 //-����
};
typedef struct
{
	unsigned char funid;	 //������
	unsigned short RegStart; //�Ĵ�����ʼ��ַ
	short para;				 //���ò���
	//	unsigned short numData;//���ݸ���
} BAMS_Fun_Struct;

extern BAMS_Fun_Struct bamsfun[];
extern int g_bms_status;
int doFunTasks(int portid, int *pPcsid);
#endif
