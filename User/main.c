/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS V9.0.0 + STM32 �����ź���ʵ��
  *********************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��STM32ȫϵ�п����� 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
/* FreeRTOSͷ�ļ� */

#include "oled.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* ������Ӳ��bspͷ�ļ� */
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "test.h"
/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* ���������� */
static TaskHandle_t Take_Task_Handle = NULL;/* Take_Task������ */
static TaskHandle_t Give_Task_Handle = NULL;/* Give_Task������ */

/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */
SemaphoreHandle_t CountSem_Handle =NULL;


/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */


/******************************* �궨�� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩ�궨�塣
 */


/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void Take_Task(void* pvParameters);/* Take_Task����ʵ�� */
static void Give_Task(void* pvParameters);/* Give_Task����ʵ�� */

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  /* ������Ӳ����ʼ�� */
  BSP_Init();
	
	OLED_ShowChinese_Row(0,0,*init);	
	OLED_ShowChinese_Row(0,4,*writer);
	
  
  /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;  
  
  while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���
  
  /* ����Test_Queue */
  CountSem_Handle = xSemaphoreCreateCounting(4,4);	 
  if(NULL != CountSem_Handle)
    printf("CountSem_Handle�����ź��������ɹ�!\r\n");

  /* ����Take_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )Take_Task, /* ������ں��� */
                        (const char*    )"Take_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&Take_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����Take_Task����ɹ�!\r\n");
  
  /* ����Give_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )Give_Task,  /* ������ں��� */
                        (const char*    )"Give_Task",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )3, /* ��������ȼ� */
                        (TaskHandle_t*  )&Give_Task_Handle);/* ������ƿ�ָ�� */ 
  if(pdPASS == xReturn)
    printf("����Give_Task����ɹ�!\n\n");
  
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}



/**********************************************************************
  * @ ������  �� Take_Task
  * @ ����˵���� Take_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void Take_Task(void* parameter)
{	
  BaseType_t xReturn = pdTRUE;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  /* ������һ������ѭ�������ܷ��� */
  while (1)
  {
    //���KEY1������
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==0)   
		{
			delay_s(2);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)==0)
			{


			/* ��ȡһ�������ź��� */
      xReturn = xSemaphoreTake(CountSem_Handle,	/* �����ź������ */
                            0); 	/* �ȴ�ʱ�䣺0 */
		if ( xReturn ) 
		{
				OLED_Clear();
				OLED_ShowChinese_Row(0,0,*carport);
			  OLED_ShowChinese_Row(0,4,*remaind);
				OLED_ShowNum(0,6,xReturn,1,6);
				delay_s(1);
			if(xReturn == 4)
				{
					OLED_Clear();
					LED1_OFF;
					delay_s(1);
			  OLED_ShowChinese_Row(0,4,*now);
				}
			else if(xReturn == 3)
				{
					OLED_Clear();
					LED2_OFF;
					delay_s(1);
				OLED_ShowChinese_Row(0,4,*now);
				}		
			else if(xReturn == 2)
				{
					OLED_Clear();
					LED3_OFF;
					delay_s(1);
				OLED_ShowChinese_Row(0,4,*now);
				}
			else if(xReturn == 1)
				{
					OLED_Clear();
					LED4_OFF;
					delay_s(1);
					
			OLED_ShowChinese_Row(0,4,*full);
				}

			}
			else{
			OLED_Clear();
			OLED_ShowChinese_Row(0,2,*full);			}		
		}
		}
			vTaskDelay(20);     //ÿ20msɨ��һ��		
  }

}

/**********************************************************************
  * @ ������  �� Give_Task
  * @ ����˵���� Give_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void Give_Task(void* parameter)
{	 
  BaseType_t xReturn = pdTRUE;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  /* ������һ������ѭ�������ܷ��� */
  while (1)
  {
	
    //���KEY2������
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)==0)
		{
			delay_s(2);
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)==0)
			{	
		

			/* ��ȡһ�������ź��� */
      xReturn = xSemaphoreGive(CountSem_Handle);//���������ź���                  
			if ( xReturn )
		{
		  OLED_Clear();
			OLED_ShowChinese_Row(0,2,*leave);
			delay_s(1);
			OLED_Clear();
			OLED_ShowChinese_Row(0,2,*now);
			OLED_ShowNum(96,4,xReturn,1,6);
			if(xReturn == 4)
				{

					LED1_ON;

				}
			else if(xReturn == 3)
				{

					LED2_ON;
				}
			else if(xReturn == 2)
				{
	
					LED3_ON;
				}		
			else if(xReturn == 1)
				{

					LED4_ON;
				}

			}
			else
				{
								OLED_Clear();
								OLED_ShowChinese_Row(0,2,*now);
			  }	
		}

	}
		vTaskDelay(20);     //ÿ20msɨ��һ��
	}
}
/***********************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED ��ʼ�� */
	LED_GPIO_Config();
	
	/* ������ʼ��	*/
  Key_GPIO_Config();

	/* ���ڳ�ʼ��	*/
	USART_Config();

	OLED_Init();			//��ʼ��OLED  
	OLED_Clear();
}

/********************************END OF FILE****************************/
