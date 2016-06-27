/* 
 * File:   edf.h
 * Author: atl05
 *
 * Created on den 16 juni 2015, 10:48
 */

#ifndef EDF_SCHEDULER_H
#define	EDF_SCHEDULER_H
#include "Tasks.h"

int g_edf_schedule(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log);
int ads_edf_schedule(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log);
int rds_edf_schedule(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log);

#endif	/* EDF_H */

