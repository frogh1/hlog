#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>

#if defined(WIN32)
#include <windows.h>
#else
#include <errno.h>
#include <sys/stat.h>
#include <pthread.h>
#include <dirent.h>
#endif

#include <stdarg.h>
#include "spinlock.h"
#include "log.h"

#define BUFF_MAX (1024) 

char szbuf[BUFF_MAX];

#if defined(WIN32)
#define DIR_SPACE  "\\"
#else
#define DIR_SPACE  "/"
#endif


typedef struct _tagLOG
{
	SPINLOCK_S spinlock;
	unsigned int auifileline[MAX_FILE_NUM];
	unsigned int uifileindex;
	unsigned int uifilelines;
	char szCurPath[1024];
}LOG_S;


LOG_S g_stLog;

const char *log_level_str[] = {
        "EMERG",
        "ALERT",
        "CRIT",
        "ERROR",
        "WARN",
        "NOTICE",
        "INFO",
        "DEBUG"
};

char *log_get_level_string(int level)
{
	return log_level_str[level];
}


int check_folder_exist(const char *szPath)
{
    int ret = 0;
#if defined(WIN32)
    WIN32_FIND_DATA  wfd;
    HANDLE hFind = FindFirstFile(szPath, &wfd);
    if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        ret = 1;   
    }
    FindClose(hFind);
#else
    DIR *pDir = opendir(szPath);
	if(NULL != pDir)
	{
		ret = 1;  
		closedir(pDir);
	}
#endif

    return ret;
}

void create_dir(const char *p)
{
#if defined(WIN32)
	char spec = '\\';
#else
	char spec = '/';
#endif

	char *p1,*p2;
	if(p1 = strrchr(p,spec))
	{ 
		p2 = strchr(p,spec);
		if(p1==p2)
			return;

		*p1 = '\0';
		if (!check_folder_exist(p))
		{
			create_dir(p);
#if defined(WIN32)
			_mkdir(p);
#else
			mkdir(p,0750);	
#endif
		}
		*p1 = spec;
	}
}

int log( int level,const char * pformat, va_list _va_list)
{
    FILE* pfile;
	time_t t;
	struct tm *tmp;
	unsigned int len = 0;
	LOG_S *pstLog = &g_stLog;
    
    if (pformat == NULL) 
    {
	    return -1;
    }

	SpinLock_Lock(&pstLog->spinlock);
	t = time(NULL);
	tmp = localtime(&t);

	memset(szbuf,0,256);
    sprintf(szbuf, "%s%u.log", pstLog->szCurPath,pstLog->uifileindex);

	if(0 != pstLog->uifilelines)
	{
	  	pfile = fopen(szbuf, "rb+");
	}
    else
	{
	  	pfile = fopen(szbuf, "wb");
	}
	
    if(NULL != pfile)
	{
        len = sprintf(szbuf,"[%04d-%02d-%02d %02d:%02d:%02d][%s] ",
		tmp->tm_year+1900,tmp->tm_mon+1,tmp->tm_mday,tmp->tm_hour,tmp->tm_min,tmp->tm_sec,log_get_level_string(level));
	    len += vsprintf(szbuf + len, pformat, _va_list);    
		fseek(pfile,0L,SEEK_END);
		fwrite(szbuf,len,1,pfile);
		fwrite("\n",1,1,pfile);
		pstLog->uifilelines++;
		if(pstLog->uifilelines >= MAX_LOGFILE_LINE)
		{
		  	pstLog->uifilelines = 0;
			pstLog->uifileindex++;
			if(pstLog->uifileindex >= MAX_FILE_NUM)
			{
				pstLog->uifileindex = 0;
			}
		}
		fflush(pfile);
        fclose(pfile);
    }

	SpinLock_unLock(&pstLog->spinlock);
    return 0;
}


int log_print(int log_level,const char * format, ...)
{
	va_list list;
	va_start(list, format);
    log(log_level,format, list);
	va_end(list);	
	return 0;
}


int log_init(char *logdir,char *logname)
{
	FILE *pfile;
	unsigned int uilen =0;
	unsigned int i =0;
	LOG_S *pstLog = &g_stLog;
	char buf[1024];

	memset(pstLog,0,sizeof(LOG_S));

	sprintf(pstLog->szCurPath,"%s%s%s%s",logdir,DIR_SPACE,logname,DIR_SPACE);
	create_dir(pstLog->szCurPath);

	InitSpinLock(&pstLog->spinlock);

	for(i = 0; i < MAX_FILE_NUM; i++)
	{
	    sprintf(buf, "%s%u.log",pstLog->szCurPath,i);
	    pfile = fopen(buf, "r");
		if(NULL == pfile)
			continue;

		while(!feof(pfile)) 
		{
			fgets(buf,1024,pfile);
			pstLog->auifileline[i]++;
			if(pstLog->auifileline[i] >= MAX_LOGFILE_LINE)
				break;
		}

		fclose(pfile);
	}

	for(i = 0; i < MAX_FILE_NUM; i++)
	{
		if(pstLog->auifileline[i] < MAX_LOGFILE_LINE)
		{
			pstLog->uifileindex = i;
			pstLog->uifilelines = pstLog->auifileline[i];
			break;
		}
	}

	if(MAX_FILE_NUM == i)
	{
		pstLog->uifileindex = 0;
		pstLog->uifilelines = 0;
	}

	return 0;
}


int log_destory()
{
	LOG_S *pstLog = &g_stLog;
	DestroySpinLock(&pstLog->spinlock);
	return 0;
}
