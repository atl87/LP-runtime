/* 
 * File:   scheduler.h
 * Author: atl05
 *
 * Created on den 4 juni 2015, 09:23
 */

#ifndef SCHEDULER_H
#define	SCHEDULER_H

#include "Tasks.h"


int insert_into_ready_queue(real_time_taskset **ready_queue, real_time_taskset **helper, real_time_taskset **taskset, float no_of_proc, int print_log);
void rds_insert_into_ready_queue(real_time_taskset **ready_queue, real_time_taskset **helper, real_time_taskset **taskset, float no_of_proc, int print_log);
void remove_from_ready_queue(real_time_taskset **ready_queue, real_time_taskset **ready_queue_helper, real_time_taskset **taskset, int print_log);

int rds_schedule_modified(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log);

int g_fps_schedule(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log);
int rds_schedule(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log);
int ads_schedule(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log);


#endif	/* SCHEDULER_H */

