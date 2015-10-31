/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
*  This example project demonstrates the basic operation of the SCB component 
*  in the UART mode. The polling method is used to wait for received character. 
*  As soon as character is received it is transmitted back. The serial terminal 
*  can be used on the PC to send characters and get them back. 
*  The example will echo every received character.
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>
#include <stdio.h>

uint8 interruptCnt;

#define LED_ON  (0u)
#define LED_OFF (1u)

#define LEFT_LINAC_LO = Pin_0;
#define LEFT_LINAC_HIGH = Pin_1;
#define RIGHT_LINAC_LO = Pin_2;
#define RIGHT_LINAC_HIGH = Pin_3;


void sendserial(unsigned char address, unsigned char command, unsigned char intensity)
{
    if(intensity >127) intensity = 127;
    unsigned char checksum = ((address+command+intensity) & 0b01111111);
   // UART_1_UartPutChar("Sending...");
    //Send actual stuff here.
    UART_1_UartPutChar(address);
    UART_1_UartPutChar(command);
    UART_1_UartPutChar(intensity);
    UART_1_UartPutChar(checksum);
}

int main()
{   
	/* Enable Global interrupts */
    CyGlobalIntEnable;
    

    //uint32 ch;
    uint32 rxData;
    //char* cmd = " ";
    //char test;
    unsigned char HIBYTE;
    unsigned char LOBYTE;
    unsigned char CMD, INT;
    int second = 0;
    /* Start SCB (UART mode) operation */
    UART_Start();
    UART_1_Start();
    CyDelay(1000);
    char str[64];
    int i;
    CyDelay(10000);
   UART_UartPutString("Please enter your next command.  Commands are in <addr> <cmd> <data>: \n\r");
   for (i = 128; i<133; i++)
    {
        sprintf(str, "Testing address %d\r\n", i);
        UART_UartPutString(str);
        UART_UartPutString("Motor 1\r\n");
        sendserial(i, 0, 50);
        CyDelay(5000);
        sendserial(i, 0, 0);
        UART_UartPutString("Motor 2\r\n");
        sendserial(i,4,50);
        CyDelay(5000);
        sendserial(i,4,0);
    }
    for (;0;)
    {
        
        //Check state of linear actuator
      // if ()
        //UART_UartPutString("Hello \n\r");      
        if (!LOBYTE) 
        {
            LOBYTE = UART_UartGetByte() & 0xFF;
            sprintf(str,"LOBYTE: %x\r\n",LOBYTE);
            UART_UartPutString(LOBYTE);
        }
        else HIBYTE = UART_UartGetByte() & 0xFF;
        
        rxData &= 0xFF; 
        if(LOBYTE && HIBYTE) {
            //rxData = UART_rx_Read();
            //cmd[sizeof(cmd)]
            //test = rxData + rxData;
            CMD = ((HIBYTE >> 4) & 0xFF);
            INT = ((HIBYTE << 4) & 0xF0) | ((LOBYTE >> 4) & 0x0F);
            
            sprintf(str,"CMD: %x INT: %x\r\n",CMD,INT);
            
            //UART_UartPutChar(rxData);
            UART_UartPutString(str);
            CyDelay(100);
            //UART_UartPutChar("CMD Buffer: \n\r");
            //UART_UartPutChar(cmd);
            //UART_UartPutChar(" \n\r");
            //rxData = 0;
            LOBYTE = 0;
            HIBYTE = 0;
        }
        
    }
}


/* [] END OF FILE */