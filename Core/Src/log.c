/******************************************************************************
 *  COPYRIGHT (C) 2023 Cresnt Ltd
 *  All rights reserved.
 *
 *  This software is the confidential and proprietary information of
 *  Cresnt Ltd ("Confidential Information"). You shall not
 *  disclose such Confidential Information and shall use it only in
 *  accordance with the terms of the license agreement you entered into
 *  with Cresnt Ltd.
 *
 *  Cresnt Ltd makes no representations or warranties about the
 *  suitability of the software, either express or implied, including but
 *  not limited to the implied warranties of merchantability, fitness for
 *  a particular purpose, or non-infringement. Cresnt Ltd shall not
 *  be liable for any damages suffered by licensee as a result of using,
 *  modifying, or distributing this software or its derivatives.
 *
 *  Unauthorized copying of this file, via any medium is strictly prohibited.
 *
 *****************************************************************************/

#include "stm32f4xx_hal.h"

#include "log.h"
#include <stdarg.h>
#include <string.h>
#include "assert.h"

extern UART_HandleTypeDef huart2;

#define LOG_MESSAGE_MAX_LENGTH 300

/* Logging message structure */
typedef struct {
    char message[LOG_MESSAGE_MAX_LENGTH];
} LogMessage_t;

const char* log_string[] = {
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG"    
};
extern const struct log_source_const_data __start_log_data[];
extern const struct log_source_const_data __stop_log_data[];

/* ---------------- Function Prototypes ---------------- */

/* ---------------- Function Definitions ---------------- */



void logTransmit( LogMessage_t *pLogMsg) {
    // Send the message over UART
    HAL_UART_Transmit(&huart2, (uint8_t*)pLogMsg->message, strlen(pLogMsg->message), HAL_MAX_DELAY);
}

/* Add the proper Formatting and then Enqueue a log message */
void formatLogAndEnqueue(uint8_t level,  const struct log_source_const_data * plog_data,  const char *fmt, ...) {
    
    if((((uint8_t*)plog_data >= (uint8_t*)__start_log_data) && 
                    ((uint8_t*)plog_data < (uint8_t*)__stop_log_data)))
    {
        if(level <= plog_data->level)
        {
            LogMessage_t logMsg;
            va_list args;

            /* Get the tick count and convert to milliseconds */            
            uint32_t totalMs = HAL_GetTick() * HAL_TICK_FREQ_1KHZ; // we have configured the tick frequency to 1KHz

            /* Calculate hours, minutes, seconds, and milliseconds */
            uint32_t ms = totalMs % 1000;
            uint32_t totalSeconds = totalMs / 1000;
            uint32_t seconds = totalSeconds % 60;
            uint32_t totalMinutes = totalSeconds / 60;
            uint32_t minutes = totalMinutes % 60;
            uint32_t hours = totalMinutes / 60;

            /* Format the log message with timestamp and level */
            snprintf(logMsg.message, LOG_MESSAGE_MAX_LENGTH, "[%02u:%02u:%02u:%03u] :: ", 
                    (unsigned int)hours, (unsigned int)minutes, (unsigned int)seconds, (unsigned int)ms);
            
            /* Format the log message */
            uint8_t usedLen = strlen(logMsg.message);
            snprintf(logMsg.message + usedLen, LOG_MESSAGE_MAX_LENGTH - usedLen,
                                    "[%s] :: [%s] ",plog_data->name, log_string[level - 1]);

            va_start(args, fmt);
            usedLen = strlen(logMsg.message); // Include brackets and space
            usedLen = vsnprintf(logMsg.message + usedLen, LOG_MESSAGE_MAX_LENGTH - usedLen, fmt, args);
            va_end(args);
            logMsg.message[usedLen + 1] = '\n'; 
            logTransmit(&logMsg);

        
        }
    }
}


void logHexDump(uint8_t level, const char * str, const struct log_source_const_data * plog_data ,
			const uint8_t*  data, size_t length)
{
    if((((uint8_t*)plog_data >= (uint8_t*)__start_log_data) && 
                ((uint8_t*)plog_data < (uint8_t*)__stop_log_data)))
    {

        if(level <= plog_data->level)
        {        
            char hexBuffer[49];  // 16 bytes * 3 chars per byte + 1 (null terminator)
            char asciiBuffer[17]; // 16 bytes + 1 (null terminator)

            formatLogAndEnqueue(level, plog_data, "\nHex Dump (%s : Length(%d)):\n",str, length);
            for (size_t i = 0; i < length; i++)
            {
                // Add hex representation to hexBuffer
                snprintf(&hexBuffer[(i % 16) * 3], 4, "%02X ", data[i]);

                // Add printable ASCII character or '.' to asciiBuffer
                asciiBuffer[i % 16] = (data[i] >= 32 && data[i] <= 126) ? data[i] : '.';

                // If line is full or this is the last byte, log the line
                if ((i % 16 == 15) || (i == length - 1)) {
                    LogMessage_t logMsg;
                    asciiBuffer[(i % 16) + 1] = '\0'; // Null-terminate ASCII buffer

                    // Log the hexBuffer and asciiBuffer
                    
                    snprintf(logMsg.message,LOG_MESSAGE_MAX_LENGTH , "\t\t\t  %04dX  |%-48s|  %s\n",i - (i % 16),  hexBuffer, asciiBuffer);
                    logTransmit(&logMsg);


                    // Reset buffers for the next line
                    memset(hexBuffer, ' ', sizeof(hexBuffer));
                    hexBuffer[48] = '\0'; // Null-terminate hexBuffer
                }
            }
        }   
    }
}

