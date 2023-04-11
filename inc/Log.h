#include <sys/time.h>
#include <stdio.h>
#include <time.h>
 
/* debug level define */
int g_dbg_level;
FILE *g_log_fp;
 
/* debug level */
#define DBG_INFOR       0x01  // call information
#define DBG_WARNING     0x02  // paramters invalid,  
#define DBG_ERROR       0x04  // process error, leading to one call fails
#define DBG_CRITICAL    0x08  // process error, leading to voip process can't run exactly or exit
 
/* debug macro */
#define DBG(level, fmt, para...)    do \
    { \
        time_t t = time(NULL);  \
        struct tm *tmm = localtime(&t); \
        if(g_dbg_level & level)     \
            printf("[%d-%02d-%02d %02d:%02d:%02d][%s][%s:%d]" fmt "\n",tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday,tmm->tm_hour,tmm->tm_min,tmm->tm_sec,__FUNCTION__,__FILE__,__LINE__,##para); \
        if((g_log_fp != NULL) && (level >= DBG_CRITICAL))   \
            fprintf(g_log_fp, "[%d-%02d-%02d %02d:%02d:%02d][%s][%s:%d]" fmt "\n",tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday,tmm->tm_hour,tmm->tm_min,tmm->tm_sec,__FUNCTION__,__FILE__,__LINE__,##para); \
    }while(0)