real_time_taskset *dummy=*taskset;
    if(helper==taskset)
    {
        *taskset=(*taskset)->next_task;
        (*helper)->next_task=NULL;
    }
    else
    {
        while(dummy->next_task!=*helper)
        {        
            dummy=dummy->next_task;
            if(dummy==NULL)
                break;
        }
        dummy->next_task=(*helper)->next_task;   
        (*helper)->next_task=NULL;
    }
    
    if(*ready_queue==NULL)
        {
            ready_queue=helper;
            cout<<"Inserting task "<<(*ready_queue)->task_no<<" to free ready queue";
        }
    else
    {
        int i=0;
        cout<<"Inserting task "<<(*ready_queue)->task_no<<" to occupied ready queue";

        for(dummy=*ready_queue; dummy->period < (*helper)->period && dummy->next_task!=NULL;dummy=dummy->next_task,i++);
        if(dummy->next_task==NULL)
        {
            dummy->next_task=*helper;
            (*helper)->next_task=NULL;
        }
        else
        {
            if(dummy==*ready_queue)
            {
                (*helper)->next_task=(*ready_queue);
                ready_queue=helper;
            }
            else
            {
                real_time_taskset *ptr=*ready_queue;
                for(;ptr->next_task!=dummy;ptr=ptr->next_task);
                ptr->next_task=*helper;
                (*helper)->next_task=dummy;
            }
        }
        if(i<=no_of_proc)
                dummy->no_of_preemptions++;

    }
