#include "all.h"

void USART3_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //ʹ��USART3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��GPIOBʱ��

	//USART3_TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//USART3_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3

}

void USART3_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	static u8 Rec_flag=0;
	static u8 Rec_Buf[4]={1,1,1,1};
	static u8 count=0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART3);	//��ȡ���յ�������	 		 
    USART_SendData(USART1,Res);
		
		if(Res==')')Rec_flag=0,count=0;
		
		if(Rec_flag==1)
		{
		  Rec_Buf[count++]=Res;
			
			if(count==4)                //������һ֡����
			{
			  if((Rec_Buf[0]=='1')&&(Rec_Buf[1]=='1')&&(Rec_Buf[2]=='1')&&(Rec_Buf[3]=='1'))
				{Flag_Left=0,Flag_Right=0,Flag_Front=0,Flag_Back=0,Flag_Stop=1;}
				if((Rec_Buf[0]=='0')&&(Rec_Buf[1]=='1')&&(Rec_Buf[2]=='1')&&(Rec_Buf[3]=='1'))
				{Flag_Left=0,Flag_Right=0,Flag_Front=1,Flag_Back=0,Flag_Stop=0;}
				if((Rec_Buf[0]=='1')&&(Rec_Buf[1]=='0')&&(Rec_Buf[2]=='1')&&(Rec_Buf[3]=='1'))
				{Flag_Left=0,Flag_Right=0,Flag_Front=0,Flag_Back=1,Flag_Stop=0;}
				if((Rec_Buf[0]=='1')&&(Rec_Buf[1]=='1')&&(Rec_Buf[2]=='0')&&(Rec_Buf[3]=='1'))
				{Flag_Left=1,Flag_Right=0,Flag_Front=0,Flag_Back=0,Flag_Stop=0;}
				if((Rec_Buf[0]=='1')&&(Rec_Buf[1]=='1')&&(Rec_Buf[2]=='1')&&(Rec_Buf[3]=='0'))
				{Flag_Left=0,Flag_Right=1,Flag_Front=0,Flag_Back=0,Flag_Stop=0;}
			}
		}
		
		if(Res=='(')Rec_flag=1;
  } 
} 

