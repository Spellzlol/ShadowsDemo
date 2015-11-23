#include "utils.h"
#include <chrono>
#include <algorithm>

long utils::systemTime() 
{
	using namespace std::chrono;
	milliseconds ms = duration_cast< milliseconds >( system_clock::now().time_since_epoch() );

	return ms.count();
}

float utils::degreesToRad(float degrees)
{
	return degrees * 0.017453278f; // pi / 180
}

std::string utils::formatString(const char* fmt,...)
{
    va_list lst;
    va_list cp;
    va_start(lst, fmt);
    va_copy(cp,lst);
    int len=vsnprintf(NULL,0,fmt,cp);
    va_end(cp);
    char* str=new char[len+1];
    vsnprintf(str,len+1,fmt,lst);
    str[len]=0;
    std::string res=str;
    delete[] str;
    va_end(lst);
    return std::move(res);
}

float utils::clamp(float value, float min, float max)
{
	return std::max<float>(min, std::min<float>(value, max));
}