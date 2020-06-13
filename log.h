
#ifndef _LOG_H_
#define _LOG_H_


#ifdef __cplusplus
extern "C" {
#endif

#define LOG_LEVEL_DEBUG   7
#define LOG_LEVEL_INFO    6
#define LOG_LEVEL_NOTICE  5
#define LOG_LEVEL_WARN    4
#define LOG_LEVEL_ERROR   3
#define LOG_LEVEL_CRIT    2
#define LOG_LEVEL_ALERT   1
#define LOG_LEVEL_EMERG   0

#define MAX_FILE_NUM  4
#define MAX_LOGFILE_LINE 40960

int log_init(char *logdir,char *logname);
int log_print(int log_level,const char * pformat, ...);
int log_destory();

#define log_debug(...)    log_print(LOG_LEVEL_DEBUG,##__VA_ARGS__)
#define log_info(...)     log_print(LOG_LEVEL_INFO,##__VA_ARGS__)
#define log_warn(...)     log_print(LOG_LEVEL_WARN,##__VA_ARGS__)
#define log_err(...)      log_print(LOG_LEVEL_ERROR,##__VA_ARGS__)
#define log_alert(...)    log_print(LOG_LEVEL_ALERT,##__VA_ARGS__)



#ifdef __cplusplus
}
#endif


#endif //_LOG_H_

