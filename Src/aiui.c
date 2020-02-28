//
// Created by yang on 2019/10/14.
//

#include "aiui.h"

char URL[] = "openapi.xfyun.cn";
char APPID[] = "5d9e9187";
char API_KEY[] = "c2654be276ae623b708b4aa63add3a00";;
char SCENE[] = "main_box";
char AUTH_ID[] = "5d34ea828a492e472629288f2e0b8b12";
char DATA_TYPE[] = "text";
char SAMPLE_RATE[] = "16000";
char AUE[] = "raw";
char LAT[] = "39.938838";
char LNG[] = "116.368624";
char RESULT_LEVEL[] = "plain";
char FILE_PATH[] = "F:\\CLion\\study_iot\\123.txt";

#define SSID "YANG-PC"
#define PASSWORD "12345678"

#define IP  "42.62.116.27"
#define PORT  "80"

//char ip[] = "10.2.8.136";
//int port = 6666;

//const char* host = "openapi.xfyun.cn";  // 连接的主机域名
//const int httpsPort = 80;  // https端口

int check_at(){

}

void wifi_init() {
    //测试
    send_at("AT\r\n");
    HAL_Delay(1000);
    send_at("AT\r\n");
    HAL_Delay(1000);
    //设置 AP+Station 模式
    send_at("AT+CWMODE=3\r\n");
    HAL_Delay(1000);
    //连接路由器
    send_at("AT+CWJAP=\"");
    send_at(SSID);
    send_at("\",\"");
    send_at(PASSWORD);
    send_at("\"\r\n");
    HAL_Delay(6000);
}

int aiui() {
    unsigned char *buf = "天气";
    printf("\r\n%s\r\n", buf);

    unsigned char *header = NULL;
    header = buildHeader();
//    printf("header:%s\r\n", header);

    unsigned char *result;
    result = httpPost(URL, header, buf);
    if (result != NULL) {
        printf("The interface is successfully called, return:\r\n%s\r\n", result);
    } else {
        printf("The interface is failed called\r\n");
    }

    free(buf);
    free(result);
    return 0;
}

void send_at(char *command) {
    HAL_UART_Transmit(&huart2, command, strlen(command), 0xffff);
}

void get_rtc_time() {
    char data[48] = {0xE3, 0x0, 0x06, 0xEC, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x31, 0x4E, 0x31, 0x34};
    char time_buf[48] = {0};
    //设置单路模式
    send_at("AT+CIPMUX=0\r\n");
    HAL_Delay(1000);
    //连接到 NTP 服务器
    send_at("AT+CIPSTART=\"UDP\",\"1.cn.pool.ntp.org\",123\r\n");
    HAL_Delay(2000);
    //设置发送 48byte 大小的数据
    send_at("AT+CIPSEND=48\r\n");
    HAL_Delay(2000);
    //发送数据
    HAL_UART_Transmit(&huart2, data, 48, 0xffff);
    HAL_Delay(3000);
    //关闭连接
    send_at("AT+CIPCLOSE\r\n");
    HAL_Delay(2000);
    //解析数据
    memcpy(time_buf, strchr(b, ':') + 1, 48);
//    for (int i = 40; i < 44; i++) {
//        printf("time_buf[%d]:%x\r\n", i, time_buf[i]);
//    }
    uint32_t curTime;
    char temp[4] = {0};
    //大小端转换
    temp[0] = time_buf[43];
    temp[1] = time_buf[42];
    temp[2] = time_buf[41];
    temp[3] = time_buf[40];
    memcpy(&curTime, &temp[0], 4);
    curTime -= 2208988800;
//    printf("curTime:%lu\r\n", curTime);
    //设置系统实时时间
//    set_RtcTime(curTime);
}

void set_RtcTime(uint32_t curTime) {
    uint8_t Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    uint16_t monDays[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    RTC_DateTypeDef date;
    RTC_TimeTypeDef time;
    uint32_t days;
    uint16_t leap_num;

    time.Seconds = curTime % 60;
    curTime /= 60;    //获取分
    time.Minutes = curTime % 60;
    curTime += 8 * 60;
    curTime /= 60;    //获取小时
    time.Hours = curTime % 24;
    days = curTime / 24;
    leap_num = (days + 365) / 1461;
    if (((days + 366) % 1461) == 0) {
        date.Year = (days / 366) + 1970 - 2000;
        date.Month = 12;
        date.Date = 31;
    } else {
        days -= leap_num;
        date.Year = (days / 365) + 1970 - 2000;
        days %= 365;
        days += 1;
        if (((date.Year % 4) == 0) && (days == 60)) {
            date.Month = 2;
            date.Date = 29;
        } else {
            if (((date.Year % 4) == 0) && (days > 60)) --days;
            for (date.Month = 0; Days[date.Month] < days; date.Month++) {
                days -= Days[date.Month];
            }
            ++date.Month;
            date.Date = days;
        }
    }
    date.WeekDay = RTC_WEEKDAY_TUESDAY;

//    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
//    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

    printf("20%02d:%02d:%02d:%02d\t", date.Year, date.Month, date.Date, date.WeekDay);
    printf("%02d:%02d:%02d\r\n", time.Hours, time.Minutes, time.Seconds);
//    HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
//    HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
}

void get_curTime(char *curTime) {

    struct tm stm;
    RTC_TimeTypeDef stTime;
    RTC_DateTypeDef stDate;
    HAL_RTC_GetTime(&hrtc, &stTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &stDate, RTC_FORMAT_BIN);
    stm.tm_year = stDate.Year + 100;        //RTC_Year rang 0-99,but tm_year since 1900
    stm.tm_mon = stDate.Month - 1;          //RTC_Month rang 1-12,but tm_mon rang 0-11
    stm.tm_mday = stDate.Date;              //RTC_Date rang 1-31 and tm_mday rang 1-31
    stm.tm_hour = stTime.Hours - 8;         //RTC_Hours rang 0-23 and tm_hour rang 0-23
    stm.tm_min = stTime.Minutes;            //RTC_Minutes rang 0-59 and tm_min rang 0-59
    stm.tm_sec = stTime.Seconds;
    sprintf(curTime, "%lu", mktime(&stm));
//    printf("20%02d:%02d:%02d:%02d\t", stDate.Year, stDate.Month, stDate.Date, stDate.WeekDay);
//    printf("%02d:%02d:%02d\r\n", stTime.Hours, stTime.Minutes, stTime.Seconds);
}

unsigned char *buildHeader() {
    //时间戳
    char curTime[10] = {0};
    get_curTime(curTime);

    //param 格式及 base64 编码
    unsigned char param[512] = {0};
    unsigned char paramBase64[512] = {0};
    int paramBase64len = 0;
    sprintf(param,
            "{\"lat\":\"%s\",\"lng\":\"%s\",\"result_level\":\"%s\",\"aue\":\"%s\",\"auth_id\":\"%s\",\"data_type\":\"%s\",\"sample_rate\":\"%s\",\"scene\":\"%s\"}",
            LAT, LNG, RESULT_LEVEL, AUE, AUTH_ID, DATA_TYPE, SAMPLE_RATE, SCENE);
    base64_encode(param, (int) strlen(param), paramBase64, &paramBase64len);

    //checkSum的 MD5 加密
    unsigned char checkSumTemp[16] = {0};
    unsigned char checkSum[16] = {0};
    unsigned char encrypt[512] = {0};
    sprintf(encrypt, "%s%s%s", API_KEY, curTime, paramBase64);
    MD5_CTX md5;
    MD5Init(&md5);
    MD5Update(&md5, encrypt, strlen((char *) encrypt));
    MD5Final(&md5, checkSumTemp);
    unsigned char temp[3] = {0};
    for (int i = 0; i < 16; i++) {
        sprintf(temp, "%02x", checkSumTemp[i]);
        strcat(checkSum, temp);
    }

    //header 封装
    unsigned char header[512] = {0};
    sprintf(header, "X-Appid:%s\r\nX-CurTime:%s\r\nX-Param:%s\r\nX-CheckSum:%s\r\n", APPID, curTime, paramBase64,
            checkSum);

    //返回 pHeader 指針
    unsigned char *pHeader;
    pHeader = (unsigned char *) calloc(strlen(header) + 1, 1);
    sprintf(pHeader, "%s", header);

//    printf("paramBase64:%s\r\n", paramBase64);
//    printf("curTime:%s\r\n", curTime);
//    printf("checkSum:%s\r\n", checkSum);
//    printf("header:%s\r\n", header);
    return pHeader;
}

unsigned char *httpPost(char *URL, unsigned char *header, unsigned char *dataByte) {

    //连接服务器
    send_at("AT+CIPSTART=\"TCP\",\"");
    send_at(IP);
    send_at("\",");
    send_at(PORT);
    send_at("\r\n");
    HAL_Delay(2000);
    //设置透传模式
    send_at("AT+CIPMODE=1\r\n");
    HAL_Delay(1000);
    //进入透传模式
    send_at("AT+CIPSEND\r\n");
    HAL_Delay(1000);
    //封装需要发送的数据
    int dataLen = strlen(dataByte) + 2;
    unsigned char temp[32] = {0};
    unsigned char PostMessage[4096] = {0};
    memset(PostMessage, 0, 4096);
    strcat(PostMessage, "POST /v2/aiui HTTP/1.1\r\n");
    strcat(PostMessage, "Host:openapi.xfyun.cn\r\n");
    strcat(PostMessage, "Content-Type:application/x-www-form-urlencoded\r\n");
    strcat(PostMessage, header);
    sprintf(temp, "Content-Length:%d\r\n", dataLen);
    strcat(PostMessage, temp);
    strcat(PostMessage, "\r\n\r\n");
    strcat(PostMessage, dataByte);
    strcat(PostMessage, "\r\n");
    //发送数据
    send_at(PostMessage);
    HAL_Delay(3000);

    //关闭连接
    send_at("+++");
    printf("\r\nClient has closed the connection\r\n");

    //解析数据
//    unsigned char tmp[6] = {0};
//    memcpy(tmp, strstr(b, "Content-Length:") + 16, 6);
//    receiveLen = strtol(tmp, NULL, 10);
    long receiveLen = 0;
    receiveLen = strrchr(b, '}') - strchr(b, '{') + 1;
    unsigned char *pReceiveMessage = (unsigned char *) calloc(receiveLen, 1);
    memcpy(pReceiveMessage, strchr(b, '{'), receiveLen);

    return pReceiveMessage;
}


