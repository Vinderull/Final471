#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include "TSLHeader.h"

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>




void enable(int fd);
void disable(int fd);
float luxCalc(int ch0, int ch1);


int main(int argc, char **argv) {

	int fd, result;
	char i2c_device[]="/dev/i2c-1";
	unsigned char buffer[2];
	uint32_t returnVal = 0;
	uint16_t sensorLuxa = 0;
	uint32_t sensorLuxb = 0;
	uint32_t visible = 0;
	uint32_t ir = 0;
	uint32_t fullSpec = 0;

	/* Open i2c device */

	fd = open("/dev/i2c-1", O_RDWR);

  /*error checking */
    if(fd<0){
        fprintf(stderr, "Error opening file! %s\n",
                            strerror(errno));
        exit(0);
    }


	/* Set slave address */

    result=ioctl(fd, I2C_SLAVE, 0x29);

/*error checking */
    if(result<0){
        fprintf(stderr, "Error setting i2c address! %s\n",
                            strerror(errno));
        exit(0);
    }
	/* Turn on the stupid thing */

	enable(fd);

	/* Set timing for integrating ADCs */
   buffer[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_CONTROL;
	 buffer[1] = TSL2591_INTEGRATIONTIME_100MS | TSL2591_GAIN_LOW;
   result = write(fd, buffer, 2);

   /*error checking */
   if(result<0){
       fprintf(stderr, "error setting ADC stuff%s\n",
                            strerror(errno));
       exit(0);
   }



disable(fd);

/*Infinite loop to alternate ECE and 471 on display */
while(1){

sensorLuxa = 0;
sensorLuxb = 0;
returnVal = 0;
enable(fd);


// Wait x ms for ADC to complete
//for (uint8_t d=0; d<=100; d++)
//{
	usleep(500000);
//}

	/*write to read chan 0 */
	buffer[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN0_LOW;

	result = write(fd,buffer,1);

	/*error checking */
	if(result<0){
		fprintf(stderr, "Error reading ADC!  %s\n",
																					 strerror(errno));
		exit(0);
		}

	/*read chan 0 */
	result = read(fd, &returnVal, 2);
	if(result < 0){
			fprintf(stderr, "error reading after issuing command! %s\n");
		exit(0);
 	}
	/*sensor val */
	//printf("sensorluxa is %d\n\r", returnVal);
	sensorLuxa |= returnVal;



  /*get chan1 value */
	buffer[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_CHAN1_LOW;
	result = write(fd,buffer,1);

	/*error checking */
			 if(result<0){
				 fprintf(stderr, "Error reading ADC! %s\n",
																					 strerror(errno));
					 exit(0);
					}


	 /*read chan 0 */
	 result = read(fd, &returnVal, 2);

	 if(result < 0){
			 fprintf(stderr, "error reading after issuing command! %s\n");
		 exit(0);
		 }

	 /*get chan 1 lux val */
	 sensorLuxb = returnVal;
	 /*shift to top 16 bits, make room for other channel */
	 sensorLuxb <<= 16;
	 /*or in other channel reading */
	 sensorLuxb |= sensorLuxa;

	/*turn off converted until next conversion */
  disable(fd);


	/*extract light values */
	fullSpec = (0xFFFF & sensorLuxb);
	ir = (sensorLuxb >> 16);
	visible = fullSpec - ir;

	printf("Visble light = %d and IR = %d\n", visible, ir);
	printf("The Lux value is %f\n", luxCalc(fullSpec, ir));
	printf("\n");
					/*wait 1 second in usec */
					usleep(1000000);
}
	/* Close device */
    close(fd);

// YOUR CODE HERE

	return 0;
}



void enable(int fd)
{
	unsigned char buffer[2];
	int result;

		buffer[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE;
		buffer[1] = TSL2591_ENABLE_POWERON | TSL2591_ENABLE_AEN | TSL2591_ENABLE_AIEN;
	 //printf("Command bit being written out %#08X \r\n", buffer[0]);
	 // buffer[0] = 0x00;
	result = write(fd,buffer,2);

	/*error checking */
			 if(result<0){
				 fprintf(stderr, "Error writing turn on commands! %s\n",
																					 strerror(errno));
					 exit(0);
					}
}

void disable(int fd)
{
	unsigned char buffer[2];
	int result;

		buffer[0] = TSL2591_COMMAND_BIT | TSL2591_REGISTER_ENABLE;
		buffer[1] = TSL2591_ENABLE_POWEROFF;
		result = write(fd,buffer,2);

		/*error checking */
				 if(result<0){
					 fprintf(stderr, "Error disable device! %s\n",
																						 strerror(errno));
						 exit(0);
						}


}

//channel 0 is Full spectrum and channel 1 is just IR

float luxCalc(int ch0, int ch1)
{
	float conFactor, lux;
	//time * gain / Dispersion factor

	// Check if value is saturated
if ((ch0 == 0xFFFF) | (ch1 == 0xFFFF))
{
	/* return 0 if saturated */
	return 0;
}

  /*gain times conversion time divided by D factor */
	conFactor = (100.0 * 1.0) / TSL2591_LUX_DF;

	//lux calculation
	/*from data sheet and arduino tutorial *?
	lux = ( ((float)ch0 - (float)ch1 )) * (1.0 - ((float)ch1/(float)ch0) ) / conFactor;

	return lux;
}
