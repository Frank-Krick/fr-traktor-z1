#include <linux/input.h>

/* Unix */
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* C */
#include <stdio.h>
#include <string.h>

const char *bus_str(int bus);

struct usb_report_1_t {
  int gain_l;
  int hi_l;
  int mid_l;
  int low_l;
  int filter_l;
  int gain_r;
  int hi_r;
  int mid_r;
  int low_r;
  int filter_r;
  int cue_mix;
  int fader_l;
  int fader_r;
  int cross_fader;
};

int main(int argc, char **argv) {
  char *device = "/dev/hidraw8";

  if (argc > 1)
    device = argv[1];

  int fd = open(device, O_RDWR);

  if (fd < 0) {
    perror("Unable to open device");
    return 1;
  }

  unsigned char buf[30];
  memset(buf, 0x0, sizeof(buf));

  struct usb_report_1_t report;
  memset(&report, 0x0, sizeof(report));

  /* Get a report from the device */
  int res = read(fd, buf, sizeof(buf));
  while (res > 0) {
    if (buf[0] == 1) {
      if (res == 30) {
        report.gain_l = (buf[2] << 8) + buf[1];
        report.hi_l = (buf[4] << 8) + buf[3];
        report.mid_l = (buf[6] << 8) + buf[5];
        report.low_l = (buf[8] << 8) + buf[7];
        report.filter_l = (buf[10] << 8) + buf[9];
        report.gain_r = (buf[12] << 8) + buf[11];
        report.hi_r = (buf[14] << 8) + buf[13];
        report.mid_r = (buf[16] << 8) + buf[15];
        report.low_r = (buf[18] << 8) + buf[17];
        report.filter_r = (buf[20] << 8) + buf[19];
        report.cue_mix = (buf[22] << 8) + buf[21];
        report.fader_l = (buf[24] << 8) + buf[23];
        report.fader_r = (buf[26] << 8) + buf[25];
        report.cross_fader = (buf[28] << 8) + buf[27];
        printf("Gain Left: %i\n", report.gain_l);
        printf("Hi Left: %i\n", report.hi_l);
        printf("Mid Left: %i\n", report.mid_l);
        printf("Low Left: %i\n", report.low_l);
        printf("Filter Left: %i\n", report.filter_l);
        printf("Gain Right: %i\n", report.gain_r);
        printf("Hi Right: %i\n", report.hi_r);
        printf("Mid Right: %i\n", report.mid_r);
        printf("Low Right: %i\n", report.low_r);
        printf("Filter Right: %i\n", report.filter_r);
        printf("Cue Mix: %i\n", report.cue_mix);
        printf("Fader Left: %i\n", report.fader_l);
        printf("Fader Right: %i\n", report.fader_r);
        printf("Cross Fader: %i\n", report.cross_fader);
      } else {
        printf("report had unexpected length");
      }
    }
    puts("\n");
    res = read(fd, buf, sizeof(buf));
  }
  close(fd);
  return 0;
}
