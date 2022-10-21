#include "protocol_bams.h"
#include "sys.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include "serial.h"
#include <string.h>

#include "crc.h"
#include "stdlib.h"
#include "threads_bams.h"
#include "main.h"
int g_bms_status = BMS_ST_INIT;
BAMS_Fun_Struct bamsfun[] = {

	{0x10, 0x0001, 15001}, //��ط�ϵͳ n ��������繦��
						   // PCS ֱ�����繦�ʲ��ó�����ֵ��PCS �����ϱ��Ŀ��ý�����繦��ʱ���ɰ��ո�ֵ�ϱ���EMS����λ����
						   // ������BAMS ���� PCS �ĵ�ط�ϵͳ��������繦��Ϊ 1500kW���� PCS �ϱ��� EMS �ķ�ϵͳ�����
						   // ���繦��Ϊ 1500kW
	{0x10, 0x0002, 14701}, //��ط�ϵͳ n �������ŵ繦��
						   // //PCS ֱ����ŵ繦�ʲ��ó�����ֵ��PCS �����ϱ��Ŀ��ý����ŵ繦��ʱ���ɰ��ո�ֵ����PCS �ŵ�Ч��
						   // //���ϱ��� EMS ����λ����
						   // //������BAMS ���� PCS �ĵ�ط�ϵͳ�������ŵ繦��Ϊ 1500kW��PCS �ŵ�Ч��Ϊ 98%���� PCS�ϱ���
						   // // EMS �ķ�ϵͳ�������ŵ繦��Ϊ1500��98%=1470kW
	{0x10, 0x0003, 0},	   //��ط�ϵͳ n ͨѶ����
						   // //0~128 �ۼӣ�PCS �ж�����ֵ�����³��� 10s����ΪͨѶ�жϣ�PCS Ӧ������ͣ��

	{0x10, 0x0004, 7982}, //��ط�ϵͳ n �ܵ�ѹ

	{0x10, 0x0005, 30001}, //��ط�ϵͳ n ������������
						   // //���Ȱ�����������繦�����У��ڲ���������������繦�����������£��ɰ�����������������
						   // //�ϣ�PCS ֱ������������ó�����ֵ��PCS�����ϱ��Ŀ��ý���������ʱ���ɰ��ո�ֵ�ϱ��� EMS ��
						   // //��λ����
	{0x10, 0x0006, 20001}, //��ط�ϵͳ n �������ŵ����
						   // //���Ȱ����������ŵ繦�����У��ڲ��������������ŵ繦�����������£��ɰ����������ŵ������
						   // //�ϣ�PCS ֱ����ŵ�������ó�����ֵ��PCS �����ϱ��Ŀ��ý����ŵ����ʱ���ɰ��ո�ֵ���� PCS �ŵ�
						   // //Ч�ʺ��ϱ��� EMS����λ����

	{0x10, 0x0007, 22001}, //��ط�ϵͳ n ����ܵ���(�з�������)
						   // //��ֵ����Ե�س�磬��ֵ����Ե�طŵ磬PCS ���� BAMS ���͵ĵ�ǰ����ֵ��������ɼ�����ֵ�Ƚϣ�
						   // //��������� 10A��Ӧ�澯��ʾ
	{0x10, 0x0008, 900},   //��ط�ϵͳ n ��� SOC:0%~100%�������������� 5%-95%
	{0x10, 0x0009, 200},   //��ط�ϵͳ n ���ʣ��ɳ����(kWh)

	{0x10, 0x000a, 200},  //��ط�ϵͳ n ���ʣ��ɷŵ���(kWh)
	{0x10, 0x000b, 7500}, //��ط�ϵͳ n ������ߵ�ѹ
						  // //����������ŵ��ֹ��ѹ���� 2.90V~3.55V��PCS ��⵽��ط�ϵͳ������ߵ�ѹ�ﵽ3.6V��PCS Ӧͣ��
						  // //������壻��ط�ϵͳ������ߵ�ѹ�ﵽ 3.63V��PCS Ӧ�ػ���

	{0x10, 0x000c, 6990}, //��ط�ϵͳ n ������͵�ѹ
						  // //����������ŵ��ֹ��ѹ���� 2.90V~3.55V��PCS ��⵽��ط�ϵͳ������͵�ѹ�ﵽ2.85V��PCS Ӧͣ
						  // //��������壻��ط�ϵͳ������͵�ѹ�ﵽ 2.75V��PCS Ӧ�ػ���

	{0x10, 0x000d, 3}, //��ط�ϵͳ n ״̬
					   // //0-��ʼ�� 1-ͣ�� 2-������ 3-���� 4-���� 5-���� 9-�ػ� 255-����

	{0x10, 0x000e, 3}, //��ط�ϵͳ n ����
					   // //0-�������(PCS��ֹ���ŵ�, PCSӦͣ���������)
					   // // 1-ֻ�ܳ�磨PCS��ֹ�ŵ磩
					   // // 2-ֻ�ܷŵ磨PCS��ֹ��磩
					   // // 3-�ɳ�ɷţ�������
	{0x10, 0x000f, 0}, //��ط�ϵͳ n �ܹ���״̬
					   // //0-���� 1-���ϣ�����ʱ��PCS Ӧͣ����������

};

static int createFunFrame(int portid, int *pPcsid, int *pLenframe, unsigned char *framebuf)
{
	int pcsid = *pPcsid;
	int numTask = ARRAY_LEN(bamsfun);

	printf("PROTEST_EMU createFunFrame:portid=%d pcsid=%d Bams'sNum=%d\n", portid, pcsid, pParaBams->pcs_num[portid]);
	unsigned short regstart = bamsfun[0].RegStart + 16 * pcsid;
	int pos = 0; //, pos1;
	int len = 0;
	int i = 0;
	unsigned short crcval;

	framebuf[pos++] = pParaBams->devid[portid];
	framebuf[pos++] = bamsfun[0].funid;
	framebuf[pos++] = regstart / 256;
	framebuf[pos++] = regstart % 256;
	framebuf[pos++] = numTask / 256;
	framebuf[pos++] = numTask % 256;
	len = numTask * 2;
	// pos1 = pos;
	framebuf[pos++] = len;

	for (i = 0; i < numTask; i++)
	{
		framebuf[pos++] = bamsfun[i].para / 256;
		framebuf[pos++] = bamsfun[i].para % 256;
	}

	crcval = crc16_check(&framebuf[0], pos);
	framebuf[pos++] = crcval / 256;
	framebuf[pos++] = crcval % 256;

	pcsid++;
	if (pcsid == (pParaBams->pcs_num[portid]))
	{
		pcsid = 0;
	}
	*pLenframe = pos;
	*pPcsid = pcsid;
	return 0;
}

int doFunTasks(int portid, int *pPcsid)
{
	unsigned char commbuf[256];
	bzero(commbuf, sizeof(commbuf));

	int lencomm = 0, lentemp, i;

	unsigned short crcval;
	unsigned char b1, b2;

	createFunFrame(portid, pPcsid, &lencomm, commbuf);
	printf("׼���������ݰ�����Ϊ:%d  ����Ϊ��", lencomm);
	for (i = 0; i < lencomm; i++)
	{
		printf("%#x ", commbuf[i]);
	}
	printf("\n");
	int res = WriteComPort(portid, commbuf, lencomm);
	if (res > 0)
	{
		printf(" ���ͳɹ� res=%d\n", res);
	}
	else
	{
		printf("����ʧ�� ret=%d\n", res);
		return 4;
	}
	lentemp = ReadComPort(portid, commbuf, 256);
	printf("lentemp:%d\n", lentemp);

	if (lentemp == 0)
	{
		return 253;
	}
	else if (lentemp == -1)
	{
		return 255;
	}

	crcval = crc16_check(commbuf, lentemp - 2);

	b1 = crcval / 256;
	b2 = crcval % 256;
	// printf("lentemp���ȣ�%d  b1:%#x  b2:%#x  commbuf[lentemp-2]:%#x commbuf[lentemp-1]:%#x\n", lentemp, b1, b2, commbuf[8 - 2], commbuf[8 - 1]);
	if (b1 != commbuf[lentemp - 2] && b2 != commbuf[lentemp - 1])
		return 254;

	if (commbuf[1] == 0x90)
		return commbuf[2];

	printf("ģ��BAMS�յ���ȷ����:%d\n", lentemp);
	return 0;
}