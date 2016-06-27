/* 
 * File:   Tasks.h
 * Author: atl05
 *
 * Created on den 3 juni 2015, 16:16
 */

#ifndef TASKS_H
#define	TASKS_H

struct real_time_taskset{
    
    int task_no;
    float comp_time;
    float deadline;
    float period;
    float utilization;
    
    float largest_NPR;
    float no_of_pp;
    
    float rem_comp_time;
    float cur_NPR;
    float release_time;
    float abs_deadline;
    
    int is_executing;
    int priority;
    int executed;
    
    
    int no_of_preemptions;
    int no_of_deadlines_missed;
    
    real_time_taskset *next_task;
};

void print_tasks(real_time_taskset *tasket);
real_time_taskset *sort_task_set(real_time_taskset *tasket);
real_time_taskset *sort_tasks(real_time_taskset *tasket);
int delete_taskset(real_time_taskset *taskset, int print_result);
int write_taskset_to_file(int task_set_no, real_time_taskset *taskset);
void restore_taskset(real_time_taskset *taskset);
float return_utilization(real_time_taskset *taskset);
void generate_NP_taskset(real_time_taskset *taskset);
int count_tasks(real_time_taskset *taskset);
int calculate_LCM(real_time_taskset *FPS_tasks);


#endif	/* TASKS_H */

