#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h>

int main(int argc, char **argv) {

	int fd;

	char i2c_device[]="/dev/i2c-1";

	unsigned char buffer[17];

	/* Open i2c device */

// YOUR CODE HERE

	/* Set slave address */

// YOUR CODE HERE

	/* Turn on oscillator */

// YOUR CODE HERE

	/* Turn on Display, No Blink */

// YOUR CODE HERE

	/* Set Brightness */

// YOUR CODE HERE

	/* Write 1s to all Display Columns */

// YOUR CODE HERE

	/* Close device */

// YOUR CODE HERE

	return 0;
}
