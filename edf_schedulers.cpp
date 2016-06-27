#include "edf_schedulers.h"
#include "Tasks.h"
#include <cstdlib>
#include<iostream>
#include<stdio.h>
using namespace std;
void remove_from_ready_queue_edf(real_time_taskset **ready_queue, real_time_taskset **finished_task, real_time_taskset **taskset, int print_log)
{
    if(*finished_task==*ready_queue)
    {
        *ready_queue=(*ready_queue)->next_task;
        if((*taskset)==NULL)
        {
            *taskset=*finished_task;
            (*finished_task)->next_task=NULL;
        }
        else
        {
            (*finished_task)->next_task=(*taskset);
            (*taskset)=(*finished_task);
        }
    }
    else
    {
        real_time_taskset *dummy=*ready_queue;
        for(;dummy->next_task!=*finished_task;dummy=dummy->next_task);
        
        dummy->next_task=(*finished_task)->next_task;
        
        if((*taskset)==NULL)
        {
            *taskset=*finished_task;
            (*finished_task)->next_task=NULL;
        }
        else
        {
            (*finished_task)->next_task=(*taskset);
            (*taskset)=(*finished_task);
        } 
    }
    if(print_log)
        cout<<"Removed task "<<(*finished_task)->task_no<<" from ready queue\n";
    

    
}


int insert_into_ready_queue_edf(real_time_taskset **ready_queue, real_time_taskset **helper, real_time_taskset **taskset, float no_of_proc, int print_log)
{
    real_time_taskset *dummy=*taskset;
    
    if(*helper==*taskset)
    {
        *taskset=(*taskset)->next_task;
        (*helper)->next_task=NULL;
    }
    else
    {
        if(dummy!=NULL)
        {
            while(dummy->next_task!=*helper)
            {        
                dummy=dummy->next_task;
                if(dummy==NULL)
                    break;
            }
            if(dummy!=NULL)
                dummy->next_task=(*helper)->next_task;   
            (*helper)->next_task=NULL;
        }
    }
    
    if(*ready_queue==NULL)
    {
        *ready_queue=*helper;
        (*ready_queue)->next_task=NULL;
        if(print_log)
            cout<<"Inserting task "<<(*ready_queue)->task_no<<" to free ready queue\n";
    }
    else
    {
        int i=1;
        int count=0;
        real_time_taskset *t_counter=*ready_queue;
        
        if(print_log)
            cout<<"Inserting task "<<(*helper)->task_no<<" to occupied ready queue\n";

        for(dummy=*ready_queue; dummy->abs_deadline <= (*helper)->abs_deadline;)
        {
            dummy=dummy->next_task;
            i++;
            if(dummy==NULL)
                break;
            
        }
        if(i<=no_of_proc)
        {
            (*helper)->is_executing=1;
        }
        
        t_counter=*ready_queue;
        while(t_counter!=NULL)
        {
            if(t_counter->is_executing==1)
                count++;
            t_counter=t_counter->next_task;
        }
        
        if(dummy==NULL)
        {
            for(dummy=*ready_queue; dummy->next_task!=NULL;dummy=dummy->next_task);    
            dummy->next_task=*helper;
            (*helper)->next_task=NULL;
        }
        else
        {
            if(dummy==*ready_queue)
            {
                (*helper)->next_task=(*ready_queue);
                *ready_queue=*helper;
            }
            else
            {
                real_time_taskset *ptr=*ready_queue;
                for(;ptr->next_task!=dummy;ptr=ptr->next_task);
                ptr->next_task=*helper;
                (*helper)->next_task=dummy;
                
            }
        }
        
        if(i<=no_of_proc && count>=no_of_proc)
        {
            if(print_log)
                cout<<"\t\t\t****************************************** preempting!!\n";
            return 1;
        }
        else
            return 0;
    }

return 0; 
    
}

int g_edf_schedule(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log)
{
    
    int time=0;
    int no_of_preemptions=0;
    
    int no_of_tasks=count_tasks(taskset);
    
    real_time_taskset *ready_queue=NULL;
    real_time_taskset *helper=taskset;
    real_time_taskset *ready_queue_helper=ready_queue;
    real_time_taskset *finished_task=ready_queue;
    
    while(time<MAX_TIME)
    {
        helper=taskset;
        while(helper!=NULL)
        {
            real_time_taskset *exchange=helper->next_task;
            
            if((int)time%(int)helper->period==0)
            {
                if(print_log)
                    cout<<"Task "<<helper->task_no<<" released at time "<<time<<"\n";
                helper->rem_comp_time=helper->comp_time;
                helper->abs_deadline=time+helper->period;
                helper->is_executing=0;
                
                no_of_preemptions+=insert_into_ready_queue_edf(&ready_queue, &helper, &taskset, no_of_processors, print_log);
                if(ready_queue==NULL)
                {
                    cout<<"\n\nError !!!";
                            exit(1);
                }
            }
            helper=exchange;
        }
        
        if(ready_queue!=NULL)
        {
            if(print_log)
                cout<<time+1<<". ";
            ready_queue_helper=ready_queue;
            real_time_taskset *dummy_t=NULL;
            for(int i=0;i<no_of_processors;i++)
            {
                
                if(ready_queue_helper==NULL)
                    break;
                dummy_t=ready_queue_helper->next_task;
                ready_queue_helper->rem_comp_time--;
                ready_queue_helper->is_executing=1;

                if(print_log)
                    cout<<ready_queue_helper->task_no<<" ";
 
                ready_queue_helper=dummy_t;   
            }
            if(ready_queue_helper!=NULL)
            {
                while(ready_queue_helper!=NULL)
                {
                    ready_queue_helper->is_executing=0;
                    ready_queue_helper=ready_queue_helper->next_task;
                }
            }
            
            if(print_log)
                cout<<"\n";

            finished_task=ready_queue;
            for(int i=0;i<no_of_processors;i++)
            {
                if(finished_task==NULL)
                    break;
                ready_queue_helper=finished_task->next_task;
                if(finished_task->rem_comp_time==0)
                {
                    if(finished_task->abs_deadline<time+1)
                    {
                        if(print_log)
                            cout<<"\t\t deadline miss!!!";
                        if(finished_task->abs_deadline<time+1)
                            finished_task->no_of_deadlines_missed++;
                    }
                    remove_from_ready_queue_edf(&ready_queue, &finished_task, &taskset, print_log);
                    taskset=sort_tasks(taskset);
                }
                finished_task=ready_queue_helper; 
            }     
        } 
        if(no_of_tasks!=count_tasks(taskset)+count_tasks(ready_queue))
        {
            cout<<"\n\tTime !!!!!!!!!!!!!: "<<time+1;
            exit(2);
        }
        time++;
    }
    
    if(taskset!=NULL)
    {
        for(helper=taskset;helper->next_task!=NULL;helper=helper->next_task);
        helper->next_task=ready_queue;
    }
    else
        taskset=ready_queue;
    
    taskset=sort_tasks(taskset);
    
    return no_of_preemptions;        
}

void insert_into_ready_queue_edf_rds(real_time_taskset **ready_queue, real_time_taskset **helper, real_time_taskset **taskset, float no_of_proc, int print_log)
{
    real_time_taskset *dummy=*taskset;
    
    if(*helper==*taskset)
    {
        *taskset=(*taskset)->next_task;
        (*helper)->next_task=NULL;
    }
    else
    {
        if(dummy!=NULL)
        {
            while(dummy->next_task!=*helper)
            {        
                dummy=dummy->next_task;
                if(dummy==NULL)
                    break;
            }
            if(dummy!=NULL)
                dummy->next_task=(*helper)->next_task;   
            (*helper)->next_task=NULL;
        }
    }
    
    if(*ready_queue==NULL)
    {
        *ready_queue=*helper;
        (*ready_queue)->next_task=NULL;
        if(print_log)
            cout<<"Inserting task "<<(*ready_queue)->task_no<<" to free ready queue\n";
    }
    else
    {        
        if(print_log)
            cout<<"Inserting task "<<(*helper)->task_no<<" to occupied ready queue\n";

        for(dummy=*ready_queue; dummy->abs_deadline <= (*helper)->abs_deadline;)
        {
            dummy=dummy->next_task;
            if(dummy==NULL)
                break;  
        }
        if(dummy==NULL)
        {
            for(dummy=*ready_queue; dummy->next_task!=NULL;dummy=dummy->next_task);    
            dummy->next_task=*helper;
            (*helper)->next_task=NULL;
        }
        else
        {
            if(dummy==*ready_queue)
            {
                (*helper)->next_task=(*ready_queue);
                *ready_queue=*helper;
            }
            else
            {
                real_time_taskset *ptr=*ready_queue;
                for(;ptr->next_task!=dummy;ptr=ptr->next_task);
                ptr->next_task=*helper;
                (*helper)->next_task=dummy;
                
            }
        }  
    }
    
    int i=1;
    dummy=*ready_queue;
    while(dummy!=NULL)
    {
        dummy->priority=i;
        i++;
        dummy=dummy->next_task;
    }
}


int rds_edf_schedule(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log)
{
    int time=0;
    int no_of_preemptions=0;
    
    real_time_taskset *ready_queue=NULL;
    real_time_taskset *rq_head=NULL;
    real_time_taskset *running_tasks[30];
    real_time_taskset *helper=taskset;
    real_time_taskset *finished_task=ready_queue;
    
    for(int i=0;i<30;i++)
        running_tasks[i]=NULL;
    
    while(time<MAX_TIME)
    {
        helper=taskset;
        while(helper!=NULL)
        {
            real_time_taskset *exchange=helper->next_task;
            
            if((int)time%(int)helper->period==0)
            {
                if(print_log)
                    cout<<"------------------Task "<<helper->task_no<<" released";
                
                helper->rem_comp_time=helper->comp_time;
                if(helper->rem_comp_time>helper->largest_NPR)
                {
                    if(helper->largest_NPR>0)
                        helper->cur_NPR=helper->largest_NPR;
                    else
                        helper->cur_NPR=1;
                }
                else
                    helper->cur_NPR=helper->rem_comp_time;
                helper->abs_deadline=time+helper->period;
                helper->executed=0;
                
                insert_into_ready_queue_edf_rds(&ready_queue, &helper, &taskset, no_of_processors,0);
                if(ready_queue==NULL)
                {
                    cout<<"\n\nError!!";
                    exit(0);
                }  
            }
            helper=exchange;
        }
        if(ready_queue!=NULL)
        {
            helper=ready_queue;
            
            while(helper!=NULL)
            {
                int is_helper_executing=0;
                for(int i=0;i<no_of_processors;i++)
                {
                    if(running_tasks[i]==helper)
                    {
                        is_helper_executing=1;
                        break;
                    }
                }
                if(is_helper_executing==0)
                {
                    int helper_allocated=0;
                    for(int i=0;i<no_of_processors;i++)
                    {
                        if(running_tasks[i]==NULL)
                        {
                            running_tasks[i]=helper;
                            helper_allocated=1;
                            break;
                        }
                    }
                    if(!helper_allocated)
                    {
                        for(int i=0;i<no_of_processors;i++)
                        {
                            if(running_tasks[i]!=NULL && running_tasks[i]->priority>helper->priority &&  running_tasks[i]->cur_NPR==0)
                            {
                                running_tasks[i]=helper;
                                no_of_preemptions++;
                                break;
                            }
                        }
                    }
                }
                
                helper=helper->next_task;
            }
        }
        for(int i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL && running_tasks[i]->cur_NPR==0)
            {
                if(running_tasks[i]->rem_comp_time>running_tasks[i]->largest_NPR)
                {
                    if(running_tasks[i]->largest_NPR>0)
                        running_tasks[i]->cur_NPR=running_tasks[i]->largest_NPR;
                    else
                        running_tasks[i]->cur_NPR=1;
                }
                else
                    running_tasks[i]->cur_NPR=running_tasks[i]->rem_comp_time;
            }
        }
        if(print_log)
            cout<<"\n"<<time<<". ";
        for(int i=0;i<no_of_processors;i++)
        {
                if(running_tasks[i]!=NULL)
                {
                    if(print_log)
                        cout<<" "<<running_tasks[i]->task_no<<"("<<running_tasks[i]->cur_NPR-1<<"-"<<running_tasks[i]->rem_comp_time-1<<")";
                    running_tasks[i]->cur_NPR--;
                    running_tasks[i]->rem_comp_time--;
                    running_tasks[i]->executed++;
                } 
        }
        int j=0;
        for(int i=0;i<no_of_processors && j<no_of_processors;j++)
        {
            if(running_tasks[i]!=NULL)
            {
                finished_task=running_tasks[i];
                if(finished_task->rem_comp_time==0)
                {
                    running_tasks[i]=NULL;
                    if(print_log)
                        cout<<"\t...finished "<<finished_task->task_no<<"\n";
                    if(finished_task->abs_deadline<time+1)
                        finished_task->no_of_deadlines_missed++;
                    if(finished_task->executed!=finished_task->comp_time)
                    {
                        cout<<"\n\n\t\t***********EDF ADS: Executed != Comp_Time********\n\n";
                        exit(1);
                    }
                    if(finished_task->cur_NPR!=0)
                    {
                        cout<<"\n\n\t\t***********EDF ADS: cur NPR != 0********\n\n";
                        exit(1);
                    }
                    remove_from_ready_queue_edf(&ready_queue, &finished_task, &taskset, 0);
                    taskset=sort_tasks(taskset);                   
                }
            }
            i++;
        } 
        time++;
    }
    
    finished_task=taskset;

    if(finished_task!=NULL)    
    {
        for(;finished_task->next_task!=NULL;finished_task=finished_task->next_task);
        finished_task->next_task=ready_queue;   
        taskset=sort_tasks(taskset);
    }
    else
    {
        taskset=ready_queue;
        taskset=sort_tasks(taskset);
        
    }
    
    return no_of_preemptions;        
}

int ads_edf_schedule(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log)
{
    int time=0;
    int no_of_preemptions=0;
    
    real_time_taskset *ready_queue=NULL;
    real_time_taskset *rq_head=NULL;
    real_time_taskset *running_tasks[30];
    real_time_taskset *helper=taskset;
    real_time_taskset *finished_task=ready_queue;
    
    for(int i=0;i<30;i++)
        running_tasks[i]=NULL;
    
    while(time<MAX_TIME)
    {
        helper=taskset;
        while(helper!=NULL)
        {
            real_time_taskset *exchange=helper->next_task;
            
            if((int)time%(int)helper->period==0)
            {
                if(print_log)
                    cout<<"------------------Task "<<helper->task_no<<" released";
                
                helper->rem_comp_time=helper->comp_time;
                if(helper->rem_comp_time>helper->largest_NPR)
                {
                    if(helper->largest_NPR>0)
                        helper->cur_NPR=helper->largest_NPR;
                    else
                        helper->cur_NPR=1;
                }
                else
                    helper->cur_NPR=helper->rem_comp_time;
                helper->abs_deadline=time+helper->period;
                helper->executed=0;
                
                insert_into_ready_queue_edf_rds(&ready_queue, &helper, &taskset, no_of_processors,0);
                if(ready_queue==NULL)
                {
                    cout<<"\n\nError!!";
                    exit(0);
                }  
            }
            helper=exchange;
        }
        if(ready_queue!=NULL)
        {
            helper=ready_queue;
            
            while(helper!=NULL)
            {
                int is_helper_executing=0;
                for(int i=0;i<no_of_processors;i++)
                {
                    if(running_tasks[i]==helper)
                    {
                        is_helper_executing=1;
                        break;
                    }
                }
                if(is_helper_executing==0)
                {
                    int helper_allocated=0;
                    for(int i=0;i<no_of_processors;i++)
                    {
                        if(running_tasks[i]==NULL)
                        {
                            running_tasks[i]=helper;
                            helper_allocated=1;
                            break;
                        }
                    }
                    if(!helper_allocated)
                    {
                        int j=0;
                        int lowest=1;
                        while(j<no_of_processors)
                        {
                            if(running_tasks[j]!=NULL)
                            {
                                lowest=1;
                                for(int i=j+1;i<no_of_processors;i++)
                                {
                                    if(running_tasks[i]!=NULL)
                                    {
                                        if(running_tasks[j]->priority<running_tasks[i]->priority)
                                            lowest=0;
                                    }
                                }
                                if(lowest==1)
                                    break;
                            }
                            j++;
                        }
                        
                        if(running_tasks[j]!=NULL && j<no_of_processors)
                        {
                            if(running_tasks[j]->priority>helper->priority && running_tasks[j]->cur_NPR==0)
                            {
                                if(print_log)
                                    cout<<"=========================================preempting task "<<running_tasks[j]->task_no<<" by "<<helper->task_no;
                                running_tasks[j]=helper;
                                no_of_preemptions++;
                            }
                        }
                    }
                }
                
                helper=helper->next_task;
            }
        }
        for(int i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL && running_tasks[i]->cur_NPR==0)
            {
                if(running_tasks[i]->rem_comp_time>running_tasks[i]->largest_NPR)
                {
                    if(running_tasks[i]->largest_NPR>0)
                        running_tasks[i]->cur_NPR=running_tasks[i]->largest_NPR;
                    else
                        running_tasks[i]->cur_NPR=1;
                }
                else
                    running_tasks[i]->cur_NPR=running_tasks[i]->rem_comp_time;
            }
        }
        if(print_log)
            cout<<"\n"<<time<<". ";
        for(int i=0;i<no_of_processors;i++)
        {
                if(running_tasks[i]!=NULL)
                {
                    if(print_log)
                        cout<<" "<<running_tasks[i]->task_no<<"("<<running_tasks[i]->cur_NPR-1<<"-"<<running_tasks[i]->rem_comp_time-1<<")";
                    running_tasks[i]->cur_NPR--;
                    running_tasks[i]->rem_comp_time--;
                    running_tasks[i]->executed++;
                } 
        }
        int j=0;
        for(int i=0;i<no_of_processors && j<no_of_processors;j++)
        {
            if(running_tasks[i]!=NULL)
            {
                finished_task=running_tasks[i];
                if(finished_task->rem_comp_time==0)
                {
                    running_tasks[i]=NULL;
                    if(print_log)
                        cout<<"\t...finished "<<finished_task->task_no<<"\n";
                    if(finished_task->abs_deadline<time+1)
                        finished_task->no_of_deadlines_missed++;
                    if(finished_task->executed!=finished_task->comp_time)
                    {
                        cout<<"\n\n\t\t***********EDF ADS: Executed != Comp_Time********\n\n";
                        exit(1);
                    }
                    if(finished_task->cur_NPR!=0)
                    {
                        cout<<"\n\n\t\t***********EDF ADS: cur NPR != 0********\n\n";
                        exit(1);
                    }
                    remove_from_ready_queue_edf(&ready_queue, &finished_task, &taskset, 0);
                    taskset=sort_tasks(taskset);                   
                }
            }
            i++;
        } 
        time++;
    }
    
    finished_task=taskset;

    if(finished_task!=NULL)    
    {
        for(;finished_task->next_task!=NULL;finished_task=finished_task->next_task);
        finished_task->next_task=ready_queue;   
        taskset=sort_tasks(taskset);
    }
    else
    {
        taskset=ready_queue;
        taskset=sort_tasks(taskset);
        
    }
    
    return no_of_preemptions;        
}




