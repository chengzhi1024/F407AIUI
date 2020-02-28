//
// Created by yang on 2019/10/16.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "md5.h"
#include "base64.h"
#include "rtc.h"
#include "usart.h"


void send_at(char *command);

unsigned char *buildHeader();

unsigned char *httpPost(char *URL, unsigned char *header, unsigned char *dataByte);

void get_curTime(char *curTime);

void get_rtc_time();

int aiui();

void wifi_init();

void set_RtcTime(uint32_t curTime);