#ifndef _SALOF_H_
#define _SALOF_H_

#include "format.h"
#include "fifo.h"
#include <stdio.h>
#include <rtconfig.h>

int salof_init(void);
void salof(const char *fmt, ...);

/** font color */
#define     FC_BLACK    30       
#define     FC_RED      31
#define     FC_GREEN    32
#define     FC_YELLOW   33
#define     FC_BLUE     34
#define     FC_PURPLE   35
#define     FC_DARK     36
#define     FC_WHITE    37

#ifdef KAWAII_MQTT_USE_LOG

#ifdef KAWAII_MQTT_USE_SALOF
    #define     PRINT_LOG       salof
#else
#if ((! defined USE_SALOF) && (! defined PRINT_LOG))
    #define         PRINT_LOG   rt_kprintf
#endif

#ifndef PRINT_LOG
    #error "If the USE_LOG macro definition is turned on, you must define PRINT_LOG as the LOG output, such as #definePRINT_LOG printf"
#endif
#endif

#ifdef KAWAII_MQTT_LOG_COLOR
    #define LOG_START(l, c)     PRINT_LOG("\033\n["#c"m["#l"] >> ")
    #define LOG_END             PRINT_LOG("\033[0m")  
#else
    #define LOG_START(l, c)     PRINT_LOG("\n["#l"] >> ")
    #define LOG_END       
#endif

#if ((defined KAWAII_MQTT_LOG_TS) && (defined KAWAII_MQTT_LOG_TAR))
    #define LOG_T           PRINT_LOG("[TS: %d] [TAR: %s] ",salof_get_tick(), salof_get_task_name())
#elif (defined KAWAII_MQTT_LOG_TS)
    #define LOG_T           PRINT_LOG("[TS: %d] ", salof_get_tick())
#elif (defined KAWAII_MQTT_LOG_TAR)
    #define LOG_T           PRINT_LOG("[TAR: %s] ", salof_get_task_name())
#else
    #define LOG_T
#endif

#define LOG_LINE(l, c, fmt, ...)        \
    do {                                \
        LOG_START(l, c);                \
        LOG_T;                          \
        PRINT_LOG(fmt, ##__VA_ARGS__);  \
        LOG_END;                        \
    } while (0)

#define KAWAII_MQTT_BASE_LEVEL      (0)
#define KAWAII_MQTT_ASSERT_LEVEL    (KAWAII_MQTT_BASE_LEVEL + 1)
#define KAWAII_MQTT_ERR_LEVEL       (KAWAII_MQTT_ASSERT_LEVEL + 1)
#define KAWAII_MQTT_WARN_LEVEL      (KAWAII_MQTT_ERR_LEVEL + 1)
#define KAWAII_MQTT_INFO_LEVEL      (KAWAII_MQTT_WARN_LEVEL + 1)
#define KAWAII_MQTT_DEBUG_LEVEL     (KAWAII_MQTT_INFO_LEVEL + 1)

#ifndef KAWAII_MQTT_LOG_LEVEL
    #define KAWAII_MQTT_LOG_LEVEL   KAWAII_MQTT_DEBUG_LEVEL
#endif 

#if KAWAII_MQTT_LOG_LEVEL < KAWAII_MQTT_DEBUG_LEVEL
    #define LOG_DEBUG(fmt, ...)
#else
    #define LOG_DEBUG(fmt, ...)     LOG_LINE(D, 0, fmt, ##__VA_ARGS__)
#endif

#if KAWAII_MQTT_LOG_LEVEL < KAWAII_MQTT_INFO_LEVEL
    #define LOG_INFO(fmt, ...)
#else
    #define LOG_INFO(fmt, ...)      LOG_LINE(I, FC_GREEN, fmt, ##__VA_ARGS__)
#endif

#if KAWAII_MQTT_LOG_LEVEL < KAWAII_MQTT_WARN_LEVEL
    #define LOG_WARN(fmt, ...)
#else
    #define LOG_WARN(fmt, ...)      LOG_LINE(W, FC_YELLOW, fmt, ##__VA_ARGS__)
#endif

#if KAWAII_MQTT_LOG_LEVEL < KAWAII_MQTT_ERR_LEVEL
    #define LOG_ERR(fmt, ...)
#else
    #define LOG_ERR(fmt, ...)       LOG_LINE(E, FC_RED, fmt, ##__VA_ARGS__)
#endif

#if KAWAII_MQTT_LOG_LEVEL < KAWAII_MQTT_ASSERT_LEVEL
    #define LOG_ASSERT(fmt, ...)
    #define ASSERT(x)
#else
    #define LOG_ASSERT(fmt, ...)    LOG_LINE(A, FC_RED, fmt, ##__VA_ARGS__)
    #define ASSERT(x)     if((x)==0) LOG_ASSERT("%s, %d\n",__FILE__,__LINE__)
#endif

#if KAWAII_MQTT_LOG_LEVEL < KAWAII_MQTT_BASE_LEVEL
    #define LOG(fmt, ...)
#else
    #define LOG(fmt, ...)           PRINT_LOG(fmt, ##__VA_ARGS__)
#endif
#else
#define LOG_DEBUG(fmt, ...)
#define LOG_INFO(fmt, ...)
#define LOG_WARN(fmt, ...)
#define LOG_ERR(fmt, ...)
#define LOG(fmt, ...)
#define ASSERT(x)
#endif

#endif // !_SALOF_H_

