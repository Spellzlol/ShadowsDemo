#pragma once

#include <android/log.h>
#include "assert.h"
#include <stdio.h>
#include <string>
#include <sstream>

#define LOG_TAG "SimpleEngine"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define Assert(condition, ...)                      							\
        {                                                   					\
            auto __result__ = condition;                    					\
            if (not __result__) {                           					\
                __android_log_print(ANDROID_LOG_ERROR,LOG_TAG, "%s, Failed! File: %s, Line: %d", #condition, __FILE__, __LINE__);\
                __android_log_print(ANDROID_LOG_ERROR,LOG_TAG, ##__VA_ARGS__); 	\
                assert(false);													\
            }                                               					\
        }																		
        
#define CHECK_GL_ERROR                                                                                  \
        {                                                                                               \
            GLenum error = glGetError();                                                                \
            if (error != GL_NO_ERROR)                                                                   \
            {                                                                                           \
                LOGE("Got GL Error in %s, File:%s, Line: %d,  error code: %d", __FUNCTION__, __FILE__, __LINE__, error);   \
            }                                                                                           \
        }                                                                                                                                                                                       \

namespace utils
{
	long systemTime();
	float degreesToRad(float degrees);
    std::string formatString(const char* fmt,...);
    float clamp(float value, float min, float max);

    template <typename T>
    T toNumber(const std::string& text)
    {
        std::istringstream ss(text);
        T result;
        return ss >> result ? result : 0;
    }
};