
#ifndef __MCUBOOT_LOGGING_H__
#define __MCUBOOT_LOGGING_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "log.h"

#define DBG_TAG "mcuboot"
#define DBG_LVL DBG_LOG

#define MCUBOOT_LOG_LEVEL_OFF     0
#define MCUBOOT_LOG_LEVEL_ERROR   1
#define MCUBOOT_LOG_LEVEL_WARNING 2
#define MCUBOOT_LOG_LEVEL_INFO    3
#define MCUBOOT_LOG_LEVEL_DEBUG   4
#define MCUBOOT_LOG_LEVEL_SIM     5

#ifndef MCUBOOT_LOG_LEVEL
#define MCUBOOT_LOG_LEVEL MCUBOOT_LOG_LEVEL_DEBUG
#endif

#if !MCUBOOT_HAVE_LOGGING
#undef MCUBOOT_LOG_LEVEL
#define MCUBOOT_LOG_LEVEL MCUBOOT_LOG_LEVEL_OFF
#endif

#define MCUBOOT_LOG_MODULE_DECLARE(domain)	LOG_MODULE_DECLARE(domain)

#define MCUBOOT_LOG_MODULE_REGISTER(domain)	LOG_MODULE_DEFINE(domain, MCUBOOT_LOG_LEVEL)

#if MCUBOOT_LOG_LEVEL >= MCUBOOT_LOG_LEVEL_ERROR
#define MCUBOOT_LOG_ERR(...)   LOG_ERROR(__VA_ARGS__)    
#else
#define MCUBOOT_LOG_ERR(...)
#endif

#if MCUBOOT_LOG_LEVEL >= MCUBOOT_LOG_LEVEL_WARNING
#define MCUBOOT_LOG_WRN(...)  LOG_WARN(__VA_ARGS__)    
#else
#define MCUBOOT_LOG_WRN(...)
#endif

#if MCUBOOT_LOG_LEVEL >= MCUBOOT_LOG_LEVEL_INFO
#define MCUBOOT_LOG_INF(...)  LOG_INFO(__VA_ARGS__)
#else
#define MCUBOOT_LOG_INF(...)
#endif

#if MCUBOOT_LOG_LEVEL >= MCUBOOT_LOG_LEVEL_DEBUG
#define MCUBOOT_LOG_DBG(...)  LOG_DEBUG(__VA_ARGS__)
#else
#define MCUBOOT_LOG_DBG(...)
#endif

#define MCUBOOT_LOG_SIM(...)

#ifdef __cplusplus
}
#endif

#endif /* __MCUBOOT_LOGGING_H__ */