#include "scheduler.h"
#include "Tasks.h"
#include <cstdlib>
#include<iostream>
#include<stdio.h>
using namespace std;
int size_of_readyqueue(real_time_taskset *ready_q)
{
   int i=0;
   real_time_taskset *ready_queue=ready_q;
   while(ready_queue!=NULL)
   {
       i++;
       ready_queue=ready_queue->next_task;
   }
   return i;
}
int insert_into_ready_queue(real_time_taskset **ready_queue, real_time_taskset **helper, real_time_taskset **taskset, float no_of_proc, int print_log)
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

        for(dummy=*ready_queue; dummy->task_no <= (*helper)->task_no;)
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

void remove_from_ready_queue(real_time_taskset **ready_queue, real_time_taskset **finished_task, real_time_taskset **taskset, int print_log)
{
    if(print_log)
        cout<<"Removing task "<<(*finished_task)->task_no<<" from ready queue\n";
    

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
    
}


int g_fps_schedule(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log)
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
                
                no_of_preemptions+=insert_into_ready_queue(&ready_queue, &helper, &taskset, no_of_processors, print_log);
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
                    remove_from_ready_queue(&ready_queue, &finished_task, &taskset, print_log);
                    taskset=sort_tasks(taskset);
                }
                finished_task=ready_queue_helper; 
            }     
        } 
        if(no_of_tasks!=count_tasks(taskset)+count_tasks(ready_queue))
        {
            cout<<"\n\tTime !!!!!!!!!!!!!: "<<time+1;
        }
        time++;
    }
    
    //cout<<"\nTotal number of preemptions under P-FPS: "<<no_of_preemptions<<"\n\n";
    
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
void rds_insert_into_ready_queue(real_time_taskset **ready_queue, real_time_taskset **helper, real_time_taskset **taskset, float no_of_proc, int print_log)
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
        int i=0;
        if(print_log)
            cout<<"Inserting task "<<(*helper)->task_no<<" to occupied ready queue\n";
        

        for(dummy=*ready_queue; dummy->task_no <= (*helper)->task_no;)
        {
            dummy=dummy->next_task;
            i++;
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
    
}

int rds_schedule(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log)
{
    
    int time=0;
    int no_of_preemptions=0;
    
    real_time_taskset *ready_queue=NULL;
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
                    cout<<"Task "<<helper->task_no<<" released at time "<<time<<"\n";
                
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
                
                rds_insert_into_ready_queue(&ready_queue, &helper, &taskset, no_of_processors,print_log);
                if(ready_queue==NULL)
                {
                    cout<<"\n\nError!!";
                    exit(0);
                }
                
            }
            helper=exchange;
        }
        
        for(int i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]==NULL)
            {
                if(ready_queue)
                {
                    running_tasks[i]=ready_queue;
                    ready_queue=ready_queue->next_task;
                }
            }
        }
        for(int j=0;j<no_of_processors;j++)
        {
            for(int i=0;i<no_of_processors;i++)
            {
                if(running_tasks[i]!=NULL && running_tasks[i]->cur_NPR==0)
                {
                    if(ready_queue!=NULL)
                    {
                        if(ready_queue->task_no < running_tasks[i]->task_no)
                        {
                            finished_task=running_tasks[i];
                            if(print_log)
                                cout<<"\n\t\t******************************************preempting task"<<finished_task->task_no<<"*\n";
                            running_tasks[i]=ready_queue;
                            ready_queue=ready_queue->next_task;
                            rds_insert_into_ready_queue(&ready_queue, &finished_task, &taskset, no_of_processors,print_log);

                            no_of_preemptions++;
                        }
                    }
                }
            }
        }
        for(int i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL && running_tasks[i]->rem_comp_time==0)
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
                        cout<<" "<<running_tasks[i]->task_no;
                    running_tasks[i]->cur_NPR--;
                    running_tasks[i]->rem_comp_time--;
                } 
        }
        
        for(int i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL)
            {
                if(running_tasks[i]->cur_NPR==0)
                {
                    finished_task=running_tasks[i];
                    if(finished_task->rem_comp_time==0)
                    {
                        if(print_log)
                            cout<<"\t...finished "<<finished_task->task_no<<"\n";
                        if(finished_task->abs_deadline<time+1)
                            finished_task->no_of_deadlines_missed++;
                        
                        rds_insert_into_ready_queue(&ready_queue, &finished_task, &taskset, no_of_processors,print_log);
                        finished_task=running_tasks[i];
                        remove_from_ready_queue(&ready_queue, &finished_task, &taskset, print_log);
                        taskset=sort_tasks(taskset);

                        running_tasks[i]=NULL;
                    }
//                    else
//                    {
//                        if(print_log)
//                            cout<<" ("<<finished_task->cur_NPR<<")";
//                        if(finished_task->rem_comp_time>finished_task->largest_NPR)
//                        {
//                            if(finished_task->largest_NPR>0)
//                                finished_task->cur_NPR=finished_task->largest_NPR;
//                            else
//                                finished_task->cur_NPR=1;
//                        }
//                        else
//                            finished_task->cur_NPR=finished_task->rem_comp_time;
//                    }
                }            
            }
        } 
        time++;
    }
    
    finished_task=taskset;

    if(finished_task!=NULL)        
        for(;finished_task->next_task!=NULL;finished_task=finished_task->next_task);
    
    if(finished_task!=NULL)
    {
        for(int i=0; i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL && running_tasks[i]->cur_NPR==0)
            {
                finished_task->next_task=running_tasks[i];
                finished_task=finished_task->next_task;
                finished_task->next_task=NULL;
            }
        }
        if(ready_queue!=NULL)
            finished_task->next_task=ready_queue;   
        taskset=sort_tasks(taskset);
    }
    else
    {
        int i=0;
        for(i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL)
            {
                finished_task=running_tasks[i];
                i++;
                break;
            }
        }
        taskset=finished_task;
        for(; i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL)
            {
                finished_task->next_task=running_tasks[i];
                finished_task=finished_task->next_task;
                finished_task->next_task=NULL;
            }
        }
        
        if(finished_task!=NULL)
            finished_task->next_task=ready_queue;
        else
        {
            taskset=ready_queue;
        }

        taskset=sort_tasks(taskset);
        
    }
    
    return no_of_preemptions;        
}


int ads_schedule(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log)
{
    int time=0;
    int no_of_preemptions=0;
    
    real_time_taskset *ready_queue=NULL;
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
                    cout<<"---------------------------------Task "<<helper->task_no<<" released at time "<<time<<"\n";
                
                helper->rem_comp_time=helper->comp_time;
                helper->executed=0;
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
                
                rds_insert_into_ready_queue(&ready_queue, &helper, &taskset, no_of_processors,print_log);
                if(ready_queue==NULL)
                {
                    cout<<"\n\nError!!";
                    exit(0);
                }
                
            }
            helper=exchange;
        }
        
        for(int i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]==NULL)
            {
                if(ready_queue)
                {
                    running_tasks[i]=ready_queue;
                    ready_queue=ready_queue->next_task;
                }
            }
            else if(running_tasks[i]->rem_comp_time<0)
            {
                cout<<"\n\n\n\t\t***********Negative remaining execution time****************\n\n\n";
            }
        }
        
        for(int k=0;k<no_of_processors;k++)
        {
            for(int i=0;i<no_of_processors;i++)
            {
                if(running_tasks[i]!=NULL)
                {
                    int lowest=1;
                    for(int j=0;j<no_of_processors;j++)
                    {
                        if(running_tasks[j]!=NULL && j!=i)
                        {
                            if(running_tasks[i]->task_no < running_tasks[j]->task_no)
                            {
                                lowest=0;
                                break;
                            }
                        }
                    }
                    if(ready_queue!=NULL)
                    {
                        if(lowest==1 && ready_queue->task_no < running_tasks[i]->task_no && running_tasks[i]->cur_NPR==0)
                        {
                            finished_task=running_tasks[i];
                            if(print_log)
                                cout<<"===================================preempting task"<<finished_task->task_no<<" by "<<ready_queue->task_no;
                            running_tasks[i]=ready_queue;
                            ready_queue=ready_queue->next_task;
                            rds_insert_into_ready_queue(&ready_queue, &finished_task, &taskset, no_of_processors,print_log);

                            no_of_preemptions++;
                        }
                    } 
                }
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
                        cout<<" "<<running_tasks[i]->task_no<<" ("<<running_tasks[i]->cur_NPR<<"-"<<running_tasks[i]->rem_comp_time<<")";
                    running_tasks[i]->cur_NPR--;
                    running_tasks[i]->rem_comp_time--;
                    running_tasks[i]->executed++;
                    if(running_tasks[i]->rem_comp_time<0)
                    {
                        cout<<"\n\n\n\t\t\tADS rem_comp_time <0\n\n";
                        exit(0);
                    }
                } 
        }
        
        for(int i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL)
            {

                finished_task=running_tasks[i];
                if(finished_task->rem_comp_time==0)
                {
                    if(print_log)
                        cout<<"\t...finished "<<finished_task->task_no<<"\n";

                    if(finished_task->cur_NPR!=0)
                    {
                        cout<<"\n\n\n\t\t**********ADS cur_NPR!=0 at finish**********\n\n";
                        exit(1);
                    }

                    if(finished_task->executed!=finished_task->comp_time)
                    {
                        cout<<"\n\n\n\t\t**********Error task executed for more than C_i**********\n\n";
                        exit(1);
                    }

                    if(finished_task->abs_deadline<time+1)
                        finished_task->no_of_deadlines_missed++;

                    rds_insert_into_ready_queue(&ready_queue, &finished_task, &taskset, no_of_processors,print_log);
                    finished_task=running_tasks[i];
                    remove_from_ready_queue(&ready_queue, &finished_task, &taskset, print_log);
                    taskset=sort_tasks(taskset);

                    running_tasks[i]=NULL;
                }
                
            }
        } 
        time++;
    }
    
    finished_task=taskset;

    if(finished_task!=NULL)        
        for(;finished_task->next_task!=NULL;finished_task=finished_task->next_task);
    
    if(finished_task!=NULL)
    {
        for(int i=0; i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL)
            {
                finished_task->next_task=running_tasks[i];
                finished_task=finished_task->next_task;
                finished_task->next_task=NULL;
            }
        }
        if(ready_queue!=NULL)
            finished_task->next_task=ready_queue;   
        taskset=sort_tasks(taskset);
    }
    else
    {
        int i=0;
        for(i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL)
            {
                finished_task=running_tasks[i];
                i++;
                break;
            }
        }
        taskset=finished_task;
        for(; i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL)
            {
                finished_task->next_task=running_tasks[i];
                finished_task=finished_task->next_task;
                finished_task->next_task=NULL;
            }
        }
        
        if(finished_task!=NULL)
            finished_task->next_task=ready_queue;
        else
        {
            taskset=ready_queue;
        }

        taskset=sort_tasks(taskset);
        
    }
    
    return no_of_preemptions;        
}



int rds_schedule_modified(real_time_taskset *taskset, float no_of_processors, int MAX_TIME, int print_log)
{
    
    int time=0;
    int no_of_preemptions=0;
    
    real_time_taskset *ready_queue=NULL;
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
                    cout<<"Task "<<helper->task_no<<" released at time "<<time<<"\n";
                
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
                
                rds_insert_into_ready_queue(&ready_queue, &helper, &taskset, no_of_processors,print_log);
                if(ready_queue==NULL)
                {
                    cout<<"\n\nError!!";
                    exit(0);
                }
                
            }
            helper=exchange;
        }
        
        int preemptible_tasks[100], preemptible=0;
        for(int i=0;i<100;i++)
            preemptible_tasks[i]=0;
        
        for(int i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL)
            {
                if(running_tasks[i]->cur_NPR==0)
                {
                    preemptible_tasks[preemptible++]=i;
                }
            }
        }       
        
        for(int i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]==NULL)
            {
                if(ready_queue)
                {
                    running_tasks[i]=ready_queue;
                    ready_queue=ready_queue->next_task;
                }
            }
        }
        
        for(int i=0;i<preemptible;i++)
        {
            for(int j=i+1; j< preemptible;j++)
            {
                if(running_tasks[preemptible_tasks[i]]->task_no<running_tasks[preemptible_tasks[j]]->task_no)
                {
                    int saved=preemptible_tasks[i];
                    preemptible_tasks[i]=preemptible_tasks[j];
                    preemptible_tasks[j]=saved;
                }
            }
        }
        for(int i=0;i<preemptible;i++)
            {
                if(ready_queue!=NULL)
                {
                    if(ready_queue->task_no < running_tasks[preemptible_tasks[i]]->task_no)
                    {
                        finished_task=running_tasks[preemptible_tasks[i]];
                        if(print_log)
                            cout<<"\n\t\t******************************************preempting task"<<finished_task->task_no<<"*\n";
                        running_tasks[preemptible_tasks[i]]=ready_queue;
                        ready_queue=ready_queue->next_task;
                        rds_insert_into_ready_queue(&ready_queue, &finished_task, &taskset, no_of_processors,print_log);

                        no_of_preemptions++;
                    }
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
                        cout<<" "<<running_tasks[i]->task_no;
                    running_tasks[i]->cur_NPR--;
                    running_tasks[i]->rem_comp_time--;
                    if(running_tasks[i]->rem_comp_time<0)
                    {
                       cout<<"\n\n\t\t\tRDS rem_comp_time <<\n\n";
                       exit(1);
                    }
                    running_tasks[i]->executed++;
                } 
        }
        
        for(int i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL)
            {
//                if(running_tasks[i]->cur_NPR==0)
//                {
                finished_task=running_tasks[i];
                if(finished_task->rem_comp_time==0)
                {
                    if(print_log)
                        cout<<"\t...finished "<<finished_task->task_no<<"\n";
                    if(finished_task->abs_deadline<time+1)
                        finished_task->no_of_deadlines_missed++;

                    if(finished_task->cur_NPR!=0)
                    {
                        cout<<"\n\n\n\t\t**********ADS cur_NPR!=0 at finish**********\n\n";
                        exit(1);
                    }
                    if(finished_task->executed!=finished_task->comp_time)
                    {
                        cout<<"\n\n\n\t\t\tError task executed for more than C_i under RDS******\n\n\n";
                        exit(1);
                    }
                    rds_insert_into_ready_queue(&ready_queue, &finished_task, &taskset, no_of_processors,print_log);
                    finished_task=running_tasks[i];
                    remove_from_ready_queue(&ready_queue, &finished_task, &taskset, print_log);
                    taskset=sort_tasks(taskset);

                    running_tasks[i]=NULL;
                }

//                }            
            }
        } 
        time++;
    }
    
    finished_task=taskset;

    if(finished_task!=NULL)        
        for(;finished_task->next_task!=NULL;finished_task=finished_task->next_task);
    
    if(finished_task!=NULL)
    {
        for(int i=0; i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL)
            {
                finished_task->next_task=running_tasks[i];
                finished_task=finished_task->next_task;
                finished_task->next_task=NULL;
            }
        }
        if(ready_queue!=NULL)
            finished_task->next_task=ready_queue;   
        taskset=sort_tasks(taskset);
    }
    else
    {
        int i=0;
        for(i=0;i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL)
            {
                finished_task=running_tasks[i];
                i++;
                break;
            }
        }
        taskset=finished_task;
        for(; i<no_of_processors;i++)
        {
            if(running_tasks[i]!=NULL)
            {
                finished_task->next_task=running_tasks[i];
                finished_task=finished_task->next_task;
                finished_task->next_task=NULL;
            }
        }
        
        if(finished_task!=NULL)
            finished_task->next_task=ready_queue;
        else
        {
            taskset=ready_queue;
        }

        taskset=sort_tasks(taskset);
        
    }
    
    return no_of_preemptions;        
}