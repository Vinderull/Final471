#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

//define oscillator initialization macro
#define OSCILLATOR (0x2<<4)|(0x1)

//1000zB1B2D
//D = display on (1)
//B1B2 = blinking frequency
#define DISPLAY_ON (0x8<<4)|(0x1)

//set brightness to 15/16 Duty Cycle
#define DISPLAY_DIM (0xE<<4)|(0xE)

int main(int argc, char **argv) {

	int fd, result;

	char i2c_device[]="/dev/i2c-1";

	unsigned char buffer[17];

	/* Open i2c device */

	fd = open("/dev/i2c-1", O_RDWR);

  /*error checking */
    if(fd<0){
        fprintf(stderr, "Error opening file! %s\n",
                            strerror(errno));
        exit(0);
    }


	/* Set slave address */

    result=ioctl(fd, I2C_SLAVE, 0x70);

/*error checking */
    if(result<0){
        fprintf(stderr, "Error setting i2c address! %s\n",
                            strerror(errno));
        exit(0);
    }
	/* Turn on oscillator */
   buffer[0] = OSCILLATOR;
   result = write(fd, buffer, 1);

    /*error checking */
    if(result<0){
        fprintf(stderr, "Error setting  oscillator! %s\n",
                                        strerror(errno));
        exit(0);
                  }

	/* Turn on Display, No Blink */

 buffer[0] = DISPLAY_ON;
 result = write(fd,buffer,1);

 /*error checking */
         if(result<0){
            fprintf(stderr, "Error setting i2c address! %s\n",
                                                           strerror(errno));
                exit(0);
         }

	/* Set Brightness */
	/* 15/16 Duty cycle */
   buffer[0] = DISPLAY_DIM;
   result = write(fd, buffer, 1);

   /*error checking */
   if(result<0){
       fprintf(stderr, "error setting brightness! %s\n",
                            strerror(errno));
       exit(0);
   }

	/* Write 1s to all Display Columns */
    buffer[0] = 0x0;

		/*loop through all bytes */
    for(int i = 0; i<16; i++) buffer[i+1] = 0xFF;
    write(fd, buffer, 17);

	/* Close device */
    close(fd);


	return 0;
}
