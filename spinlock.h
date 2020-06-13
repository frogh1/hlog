
/* spinlock.h*/
#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

typedef struct _tagSPINLOCK
{
#if defined(WIN32)
    CRITICAL_SECTION Crit_Sec;
#else
    pthread_mutex_t Mutex;
#endif
}SPINLOCK_S;


#if defined(WIN32)
#define InitSpinLock(x)  InitializeCriticalSection(&(x)->Crit_Sec)
#else
#define InitSpinLock(x) pthread_mutex_init(&(x)->Mutex,NULL)
#endif

#if defined(WIN32)
#define SpinLock_Lock(x)  EnterCriticalSection(&(x)->Crit_Sec);
#else
#define SpinLock_Lock(x) pthread_mutex_lock(&(x)->Mutex)
#endif

#if defined(WIN32)
#define SpinLock_unLock(x)  LeaveCriticalSection(&(x)->Crit_Sec)
#else
#define SpinLock_unLock(x) pthread_mutex_unlock(&(x)->Mutex)
#endif

#if defined(WIN32)
#define DestroySpinLock(x)  DeleteCriticalSection(&(x)->Crit_Sec)
#else
#define DestroySpinLock(x) pthread_mutex_destroy(&(x)->Mutex)
#endif

#endif

