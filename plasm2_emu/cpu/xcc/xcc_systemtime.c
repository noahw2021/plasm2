//
//  xcc_systemtime.c
//  plasm2_emu
//
//  Created by Noah Wooten on 11/27/23.
//

#include "../cpu.h"
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

WORD64 CpuTimerGetPreciseTimeNanoseconds(void) {
#ifdef _WIN32
    WORD64 Return;
    QueryPerformanceCounter(&Return);
    CpuCtx->LastTrackedNanoSecond = Return;
    return Return;
#elif __MACH__
    clock_serv_t ClockService;
    mach_timespec_t Timer;
    
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, 
        &ClockService);
    clock_get_time(ClockService, &Timer);
    mach_port_deallocate(mach_task_self(), ClockService);
    WORD64 NSeconds = Timer.tv_sec;
    NSeconds *= NS_PER_S;
    NSeconds += Timer.tv_nsec;
    return NSeconds;
    
#elif __unix__
    struct timespec TimeSpec;
    clock_gettime(CLOCK_REALTIME, &TimeSpec);
    
    WORD64 RealNSeconds = NS_PER_S * TimeSpec.tv_secs;
    RealNSeconds += TImeSpec.tv_nsecs;
    return RealNSeconds;
#else
#error "No high precision timer implementation!"
#endif
}

WORD64 CpuTimerGetPreciseTimeMicroseconds(void) {
    return CpuTimerGetPreciseTimeNanoseconds() / 1000;
}

WORD64 CpuTimerGetPreciseTimeMilliseconds(void) {
    return CpuTimerGetPreciseTimeMicroseconds() / 1000;
}

WORD64 CpuTimerGetPreciseTimeSeconds(void) {
    return CpuTimerGetPreciseTimeMilliseconds() / 1000;
}
