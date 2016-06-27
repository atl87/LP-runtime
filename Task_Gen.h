/* 
 * File:   Task_Gen.h
 * Author: atl05
 *
 * Created on den 4 juni 2015, 09:07
 */

#ifndef TASK_GEN_H
#define	TASK_GEN_H

#include"Tasks.h"


int UUniFast(int number_of_tasks, float total_util, real_time_taskset *taskset, float deadline_percentage, int MAX_PERIOD, int MIN_PERIOD);
int create_tasks(int number_of_tasks, real_time_taskset *taskset);
void create_NPRS(real_time_taskset *taskset, float num_of_processors, float npr_percentage);


#endif	/* TASK_GEN_H */

