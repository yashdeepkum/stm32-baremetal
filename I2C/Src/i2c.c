#include "stm32f1xx.h"



//pinout
//pb8 scl
//pb9 sda

#define GPIOBEN  (1U<<3)
#define AFIOEN   (1U<<0)
#define I2CMAP   (1U<<1)
#define I2CENR   (1U<<21)
#define I2C_100KHZ 80
#define SD_MODE_MAX_RISE_TIME 17
#define CR1_PE  (1U<<0)
#define BUSY    (1U<<1)
#define CR1_START (1U<<8)
#define SB        (1U<<0)
#define ADDR       (1U<<1)
#define SR1_TXE     (1U<<7)
#define ACK          (1U<<10)
#define STOP         (1U<<9)
#define RXNE         (1U<<6)
#define BTF          (1U<<2)

void i2c1_init(void)
{
	  //enable clock acess to GPIOB
	RCC->APB2ENR |= GPIOBEN;
    RCC->APB2ENR |= AFIOEN;
	 //set pb8 and pb9 mode to alt function
	AFIO->MAPR |= I2CMAP;
	//set pb8 and pb9 to output as open drain
	GPIOB->CRH |= (1U<<3);
	GPIOB->CRH |= (1U<<2);
	GPIOB->CRH |= (1U<<1);
	GPIOB->CRH &=~(1U<<0);

	    GPIOB->CRH |= (1U<<7);
		GPIOB->CRH |= (1U<<6);
		GPIOB->CRH |= (1U<<5);
		GPIOB->CRH &= ~(1U<<4);
	//enablE CLK FOR I2C
		RCC->APB1ENR |= I2CENR;
		//ENTER RESET MODE
		I2C1->CR1 |= (1U<<15);
		//COME OUT OF RESET MODE
		I2C1->CR1 &=~(1U<<15);
	    //SET 16MHZ FREQ
		I2C1->CR2 = 16;
		//SET I2C TO STD MODE 100KHZ CLK
		I2C1->CCR = I2C_100KHZ;
		//SET RISE TIME
		I2C1->TRISE = SD_MODE_MAX_RISE_TIME;
		//SET PERIPHERAL ENABLE
		I2C1->CR1 |= CR1_PE;
}
void i2c1_byteread(char saddr,char maddr,char* data)
{
 volatile int tmp;

 //WAIT UNTIL BUS NOT BUSY
	while(I2C1->SR2 & BUSY){}
	//GENERATE START
    I2C1->CR1 |= CR1_START;
    //WAIT UNTIL START FLAG IS SET
    while (!(I2C1->SR1 & SB)){}

    //transmit slave address+write
    I2C1->DR = saddr<<1;
    //WAIT UNTIL ADD FLAG IS SET
    while (!(I2C1->SR1 & ADDR)){}
//CLEAR ADDRESS FLAG
    tmp=I2C1->SR2;
    //SEND MEMORY ADDRESS
    I2C1->DR =maddr;
    //WAIT UNTIL TRANSMIT IS EMPTY
    while (!(I2C1->SR1 & SR1_TXE)){}
    //generate restart
    I2C1->CR1 |= CR1_START;
    //wait until start flag is set
    while (!(I2C1->SR1 & SB)){}
    //transmit slave add+read
    I2C1->DR = saddr<<1|1;
    //WAIT UNTIL ADDR FLAG IS SET
    while (!(I2C1->SR1 & ADDR)){}
    //DISABLE ACK
    I2C1->CR1 &=~ACK;
    //CLEAR ADDR FLAG
    tmp=I2C1->SR2;
    //GENERATE STOP AFTER DATA RECEIVED
    I2C1->CR1 |= STOP;
    //WAIT UNTIL RXNE FLAG IS SET
    while(!(I2C1->SR1 & RXNE)){}
    //READ DATA FROM DR
    *data++ = I2C1->DR;

}

void i2c1_burstread(char saddr,char maddr,int n,char* data)
{
volatile int tmp;
//WAIT UNTIL BUS NOT BUSY
	while(I2C1->SR2 & BUSY){}
	//GENERATE START
	    I2C1->CR1 |= CR1_START;
	    while (!(I2C1->SR1 & SB)){}

	        //transmit slave address+write
	    I2C1->DR = saddr<<1;
	        //WAIT UNTIL ADD FLAG IS SET
       while (!(I2C1->SR1 & ADDR)){}
	       //CLEAR ADDRESS FLAG
       tmp=I2C1->SR2;
	           //WAIT UNTIL TRANSMIT IS EMPTY
       while (!(I2C1->SR1 & SR1_TXE)){}
	              //SEND MEMORY ADDRESS
	   I2C1->DR =maddr;
	                  //WAIT UNTIL TRANSMIT IS EMPTY
	   while (!(I2C1->SR1 & SR1_TXE)){}
	                  //generate restart
	    I2C1->CR1 |= CR1_START;
	                     //WAIT UNTIL START FLAG IS SET
	     while (!(I2C1->SR1 & SB)){}
	     //transmit slave add+read
	     I2C1->DR = saddr<<1|1;
	     //WAIT UNTIL ADDR FLAG IS SET
	       while (!(I2C1->SR1 & ADDR)){}
	       //CLEAR ADDRESS FLAG
	       tmp=I2C1->SR2;
	       //ENABLE ACK
	           I2C1->CR1 |=ACK;
	       while(n>0U)
	       {
	    	   //if one byte
	    	   if (n==1U)
	   {//disable ack
	    		   I2C1->CR1 &=~ACK;
	    //generate stop
	    		   I2C1->CR1 |= STOP;
	   //wait for rxne flag set
	    while (!(I2C1->SR1 & RXNE)){}
	    //READ DATA FROM DR
	    *data++ =I2C1->DR;
	    break;
	    }
	    	   else
	    		   {
	    		   //wait until rxne flag is set
	    		   while (!(I2C1->SR1 & RXNE)){}
	    		   (*data++)=I2C1->DR;
	    		   n--;
	    		   }


	       }
}
void i2c1_burstwrite(char saddr,char maddr,int n,char* data)
{
	volatile int tmp;
	while(I2C1->SR2 & BUSY){}
		//GENERATE START
     I2C1->CR1 |= CR1_START;
     while (!(I2C1->SR1 & SB)){}
     //transmit slave address+write
     I2C1->DR = saddr<<1;
     //WAIT UNTIL ADD FLAG IS SET
while (!(I2C1->SR1 & ADDR)){}
//CLEAR ADDRESS FLAG
tmp=I2C1->SR2;
//WAIT UNTIL TRANSMIT IS EMPTY
    while (!(I2C1->SR1 & SR1_TXE)){}
    //SEND MEMORY ADDRESS
I2C1->DR =maddr;
     for (int i=0;i<n;i++)
     {
    	 //wait until data register empty
  	   while (!(I2C1->SR1 & SR1_TXE)){}
    	 //TRANSMIT MEMORY ADDRESS
  	 I2C1->DR = *data++;
     }
     //WAIT UNTIL TRANSMIT FINISHED
     while (!(I2C1->SR1 & BTF)){}
     //generate stop
      I2C1->CR1 |= STOP;

}
