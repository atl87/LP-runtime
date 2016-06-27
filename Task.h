/* 
 * File:   Task.h
 * Author: atl05
 *
 * Created on den 3 juni 2015, 16:08
 */

#ifndef TASK_H
#define	TASK_H

struct real_time_taskset{
    
    int task_no;
    float comp_time;
    float rem_comp_time;
    float period;
    float deadline;
    float utilization;
    float largest_NPR;
    float cur_NPR;
    float no_of_pp;
    
    int no_of_preemptions;
    
    real_time_taskset *next_task;
};

void print_tasks(real_time_taskset *tasket);
real_time_taskset *sort_task_set(real_time_taskset *tasket);
int delete_taskset(real_time_taskset *taskset, int print_result);
int write_taskset_to_file(int task_set_no, real_time_taskset *taskset);
void restore_taskset(real_time_taskset *taskset);
void generate_pseudo_taskset(real_time_taskset *taskset);
float return_utilization(real_time_taskset *taskset);
void generate_NP_taskset(real_time_taskset *taskset);



#endif	/* TASK_H */

