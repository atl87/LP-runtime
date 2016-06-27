#include"Tasks.h"
#include<iostream>
#include<cmath>
#include<iostream>
#include<fstream>
#include<time.h>
#include<stdlib.h>
#include<string.h>

using namespace std;

real_time_taskset *sort_task_set(real_time_taskset *taskset, char prio[20])
{
    if(taskset==NULL)
        cout<<"Error: No task to print !";
    else
    {
        if(strcmp(prio,"DM"))
        {
            real_time_taskset *sort_help1=taskset;
            while(sort_help1)
            {
                real_time_taskset *sort_help2=sort_help1->next_task;
                while(sort_help2)
                {
                    if(sort_help1->deadline > sort_help2->deadline)
                    {
                        real_time_taskset *prev_task1=taskset;
                        if(sort_help1!=taskset)
                        {             
                            while(prev_task1->next_task!=sort_help1)
                                    prev_task1=prev_task1->next_task;
                        }

                        real_time_taskset *prev_task2=taskset;
                        while(prev_task2->next_task!=sort_help2)
                            prev_task2=prev_task2->next_task;

                        real_time_taskset *dummy=sort_help1->next_task;

                        if(sort_help1!=taskset)
                        {                        
                            prev_task2->next_task=sort_help1;
                            sort_help1->next_task=sort_help2->next_task;
                            if(prev_task1!=sort_help1)
                                    prev_task1->next_task=sort_help2;

                            if(dummy!=sort_help2)
                                    sort_help2->next_task=dummy;
                            else
                                    sort_help2->next_task=sort_help1;

                        }
                        else
                        {
                            prev_task2->next_task=sort_help1;
                            sort_help1->next_task=sort_help2->next_task;

                            if(sort_help2!=dummy)
                                    sort_help2->next_task=dummy;
                            else
                                    sort_help2->next_task=sort_help1;

                            taskset=sort_help2;

                        }

                        dummy=sort_help1;
                        sort_help1=sort_help2;
                        sort_help2=dummy;

                    }

                    sort_help2=sort_help2->next_task;
                }

                sort_help1=sort_help1->next_task;
            }
        }
        else if(strcmp(prio,"D-C"))
        {
            real_time_taskset *sort_help1=taskset;
            while(sort_help1)
            {
                real_time_taskset *sort_help2=sort_help1->next_task;
                while(sort_help2)
                {
                    if((sort_help1->deadline-sort_help1->comp_time) > (sort_help2->deadline-sort_help2->comp_time))
                    {
                        real_time_taskset *prev_task1=taskset;
                        if(sort_help1!=taskset)
                        {             
                            while(prev_task1->next_task!=sort_help1)
                                    prev_task1=prev_task1->next_task;
                        }

                        real_time_taskset *prev_task2=taskset;
                        while(prev_task2->next_task!=sort_help2)
                            prev_task2=prev_task2->next_task;

                        real_time_taskset *dummy=sort_help1->next_task;

                        if(sort_help1!=taskset)
                        {                        
                            prev_task2->next_task=sort_help1;
                            sort_help1->next_task=sort_help2->next_task;
                            if(prev_task1!=sort_help1)
                                    prev_task1->next_task=sort_help2;

                            if(dummy!=sort_help2)
                                    sort_help2->next_task=dummy;
                            else
                                    sort_help2->next_task=sort_help1;

                        }
                        else
                        {
                            prev_task2->next_task=sort_help1;
                            sort_help1->next_task=sort_help2->next_task;

                            if(sort_help2!=dummy)
                                    sort_help2->next_task=dummy;
                            else
                                    sort_help2->next_task=sort_help1;

                            taskset=sort_help2;

                        }

                        dummy=sort_help1;
                        sort_help1=sort_help2;
                        sort_help2=dummy;

                    }

                    sort_help2=sort_help2->next_task;
                }

                sort_help1=sort_help1->next_task;
            }
            
        }
       
       real_time_taskset *help_ptr=taskset;
        int counter=1;
        while(help_ptr!=NULL)
        {
            help_ptr->task_no=counter;
            help_ptr=help_ptr->next_task;
            counter++;
        }
    }
 
    return taskset;
}

real_time_taskset *sort_tasks(real_time_taskset *taskset)
{
    if(taskset==NULL)
        cout<<"Error: No task to print !";
    else
    {      
       real_time_taskset *sort_help1=taskset;
        while(sort_help1)
        {
            real_time_taskset *sort_help2=sort_help1->next_task;
            while(sort_help2)
            {
                if(sort_help1->task_no > sort_help2->task_no)
                {
                    real_time_taskset *prev_task1=taskset;
                    if(sort_help1!=taskset)
                    {             
                        while(prev_task1->next_task!=sort_help1)
                                prev_task1=prev_task1->next_task;
                    }
                        
                    real_time_taskset *prev_task2=taskset;
                    while(prev_task2->next_task!=sort_help2)
                        prev_task2=prev_task2->next_task;
                    
                    real_time_taskset *dummy=sort_help1->next_task;
                    
                    if(sort_help1!=taskset)
                    {                        
                        prev_task2->next_task=sort_help1;
                        sort_help1->next_task=sort_help2->next_task;
                        if(prev_task1!=sort_help1)
                                prev_task1->next_task=sort_help2;
                        
                        if(dummy!=sort_help2)
                                sort_help2->next_task=dummy;
                        else
                                sort_help2->next_task=sort_help1;
 
                    }
                    else
                    {
                        prev_task2->next_task=sort_help1;
                        sort_help1->next_task=sort_help2->next_task;
                        
                        if(sort_help2!=dummy)
                                sort_help2->next_task=dummy;
                        else
                                sort_help2->next_task=sort_help1;
                        
                        taskset=sort_help2;                        
                    }                               
                    dummy=sort_help1;
                    sort_help1=sort_help2;
                    sort_help2=dummy;
                }                
                sort_help2=sort_help2->next_task;
            }            
            sort_help1=sort_help1->next_task;
        }       
    }
 
    return taskset;
}


void generate_NP_taskset(real_time_taskset *taskset){
    real_time_taskset *helper=taskset;
    
    while(helper!=NULL){
        
       
        
        helper->largest_NPR=helper->comp_time;
        helper->no_of_pp=0;
        
        helper=helper->next_task;
    }
}

void print_tasks(real_time_taskset *taskset){
    
        real_time_taskset *help_ptr=taskset;
        cout<<"T.No\tC_i\tD_i\tT_i\tNPR\tPP\n";
        while(help_ptr!=NULL)
        {
            printf("%d\t%.0lf\t%.0lf\t%.0lf\t%.0lf\t%.0lf\n",help_ptr->task_no,help_ptr->comp_time,help_ptr->deadline,help_ptr->period,help_ptr->largest_NPR,help_ptr->no_of_pp);
            help_ptr=help_ptr->next_task;
        }
}

int delete_taskset(real_time_taskset *taskset, int print_result)
{
    real_time_taskset *helper=taskset;
    
    while(taskset)
    {
        helper=taskset;
        taskset=taskset->next_task;
        free(helper);
    }
    
    if(print_result)
        cout<<"\nSuccessfully deleted the taskset!";
    return 1;
}
int write_taskset_to_file(int task_set_no, real_time_taskset *taskset)
{
    
    char task_set_name[100];
    char utilization_filename[50];
    fstream fobj_task, fobj_utilization;
    real_time_taskset *helper=taskset;
    float total_util=0.00;
    
    
    sprintf(task_set_name,"./Tasksets/task_set%d.dat",task_set_no); 
    sprintf(utilization_filename,"./Tasksets/a-utilization.dat");
    
    fobj_task.open(task_set_name,ios::out);
    if(!fobj_task)
    {
        cout<<"\nError opening file: "<<task_set_name;
        exit(1);
    }
    else
    {
        //cout<<"\nSuccessfully opened file: "<<task_set_name<<"\n";
    }
    
    fobj_utilization.open(utilization_filename,ios::out|ios::app);
    if(!fobj_utilization)
    {
        cout<<"\nError opening file: "<<utilization_filename;
        exit(0);
    }
    else
    {
        cout<<"\nSuccessfully opened file: "<<utilization_filename<<"\n";
    }
    
    while(helper)
    {
        cout<<helper->task_no<<"\t"<<helper->comp_time<<"\t"<<helper->deadline<<"\t"<<helper->period<<"\n";
        fobj_task<<helper->task_no<<"\t"<<helper->comp_time<<"\t"<<helper->deadline<<"\t"<<helper->period<<"\t"<<helper->utilization<<"\t"<<helper->largest_NPR<<"\n";
        total_util=total_util+helper->utilization;
        
        helper=helper->next_task;
    }
    
    
    fobj_utilization<<total_util<<"\n";    
    
    return 1;
}
float return_utilization(real_time_taskset *taskset)
{
    
    real_time_taskset *helper=taskset;
    float total_util=0.00;
    
    
    while(helper)
    {
        total_util=total_util+helper->utilization;
        
        helper=helper->next_task;
    }
     
    
    return total_util;
}
void restore_taskset(real_time_taskset *taskset)
{
    real_time_taskset *help_ptr=taskset;
    
    while(help_ptr!=NULL)
    {
        
        help_ptr->no_of_preemptions=0;
        help_ptr->no_of_deadlines_missed=0;
        
        help_ptr=help_ptr->next_task;
                
    }
}

int calculate_LCM(real_time_taskset *FPS_tasks)
{
	real_time_taskset *task_pointer, *help_pointer, *last_task=FPS_tasks;
	int LCM;
	int count=0, LCM_found=0;

	if(FPS_tasks)
	{
		task_pointer=FPS_tasks;
		LCM=task_pointer->period;
	}
	else
		return 0;

        
        while(last_task->next_task!=NULL)
            last_task=last_task->next_task;
        
	if(task_pointer)
	{
		while(! LCM_found)
		{
			help_pointer=task_pointer;
			count++;
			LCM=last_task->period*count;
			while(help_pointer!=NULL)
			{
				if(LCM % (int)help_pointer->period == 0)
				{
					help_pointer=help_pointer->next_task;
					LCM_found=1;
				}
				else
				{
					help_pointer=NULL;
					LCM_found=0;
				}
			}
		}
	}
	return LCM;
}

int count_tasks(real_time_taskset *taskset)
{
    int count=0;
    for(real_time_taskset *helper=taskset;helper!=NULL;)
    {
        helper=helper->next_task;
        count++;
    }
    return count;
}

real_time_taskset *sort_tasks_edf(real_time_taskset *taskset)
{
    if(taskset==NULL)
        cout<<"Error: No task to print !";
    else
    {      
       real_time_taskset *sort_help1=taskset;
        while(sort_help1)
        {
            real_time_taskset *sort_help2=sort_help1->next_task;
            while(sort_help2)
            {
                if(sort_help1->priority > sort_help2->priority)
                {
                    real_time_taskset *prev_task1=taskset;
                    if(sort_help1!=taskset)
                    {             
                        while(prev_task1->next_task!=sort_help1)
                                prev_task1=prev_task1->next_task;
                    }
                        
                    real_time_taskset *prev_task2=taskset;
                    while(prev_task2->next_task!=sort_help2)
                        prev_task2=prev_task2->next_task;
                    
                    real_time_taskset *dummy=sort_help1->next_task;
                    
                    if(sort_help1!=taskset)
                    {                        
                        prev_task2->next_task=sort_help1;
                        sort_help1->next_task=sort_help2->next_task;
                        if(prev_task1!=sort_help1)
                                prev_task1->next_task=sort_help2;
                        
                        if(dummy!=sort_help2)
                                sort_help2->next_task=dummy;
                        else
                                sort_help2->next_task=sort_help1;
 
                    }
                    else
                    {
                        prev_task2->next_task=sort_help1;
                        sort_help1->next_task=sort_help2->next_task;
                        
                        if(sort_help2!=dummy)
                                sort_help2->next_task=dummy;
                        else
                                sort_help2->next_task=sort_help1;
                        
                        taskset=sort_help2;
                        
                    }
                               
                    dummy=sort_help1;
                    sort_help1=sort_help2;
                    sort_help2=dummy;
   
                }
                
                sort_help2=sort_help2->next_task;
            }
            
            sort_help1=sort_help1->next_task;
        }
       
    }
 
    return taskset;
}