// gcc ./raw_reset.c -o raw_reset
// sudo ./raw_reset "$(lsusb | awk '/0483:5740/{printf("/dev/bus/usb/%s/%s\n", $2, substr($4, 0, 3))}')"
#include <stdlib.h>              // EXIT_SUCCESS
#include <fcntl.h>               // openat(), AT_FDCWD, O_RDWR
#include <stdio.h>               // perror(), fprintf()
#include <sys/ioctl.h>           // ioctl()
#include <unistd.h>              // close()
#include <linux/usbdevice_fs.h>  // USBDEVFS_RESET, USBDEVFS_GET_CAPABILITIES

int main(int argc, char** argv) {
	int r = 0;
	int fd = -1;

	if (argc < 2) {
		fprintf(stderr, "USAGE: %s /dev/bus/usb/<BUS#>/<DEV#>\n", argv[0]);
		return EXIT_FAILURE;
	}

	fd = openat(AT_FDCWD, argv[1], O_RDWR);
	if (fd == -1) {
		perror(argv[1]);
		return EXIT_FAILURE;
	}

	r = ioctl(fd, USBDEVFS_RESET);
	if (r == -1) {
		perror(argv[1]);
		return EXIT_FAILURE;
	}

	close(fd);
	return EXIT_SUCCESS;
}
