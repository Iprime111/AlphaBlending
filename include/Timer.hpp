#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <cstddef>
#include <cstdint>
#include <ctime>

#include "ErrorCode.hpp"

ErrorCode InitTimers    (const size_t timersCount);
ErrorCode DestroyTimers ();
ErrorCode StartTimer    (const size_t timerIndex);
uint64_t  GetTimerValue (const size_t timerIndex);

#endif  
