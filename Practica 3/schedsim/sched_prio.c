#include "sched.h"

// Variable global:
struct prio_data {
	int remaining_ticks_slice;
};
int prio_quantum = 3;

static int task_new_prio(task_t* t)
{
	struct prio_data* cs_data=malloc(sizeof(struct prio_data));

	if (!cs_data)
		return 1;  /* Cannot reserve memory */

	/* initialize the quantum */
	cs_data->remaining_ticks_slice=prio_quantum;
	t->tcs_data=cs_data;
	return 0;
}



static task_t* pick_next_task_prio(runqueue_t* rq)
{
	task_t* t=head_slist(&rq->tasks);
  
	/* Current is not on the rq -> let's remove it */
	if (t) 
		remove_slist(&rq->tasks,t);

	return t;
}

static int compare_tasks_prio(void *t1,void *t2){
	task_t* tsk1=(task_t*)t1;
	task_t* tsk2=(task_t*)t2;
	return tsk1->prio-tsk2->prio;
}

static void enqueue_task_prio(task_t* t,runqueue_t* rq, int preempted)
{
    struct prio_data* cs_data=(struct prio_data*) t->tcs_data;
	if (t->on_rq || is_idle_task(t))
		return;

	if (t->flags & TF_INSERT_FRONT) {
		//Clear flag
		t->flags&=~TF_INSERT_FRONT;
		sorted_insert_slist_front(&rq->tasks, t, 1, compare_tasks_prio);  //Push task
        cs_data->remaining_ticks_slice=prio_quantum;
	} else
		sorted_insert_slist(&rq->tasks, t, 1, compare_tasks_prio);  //Push task
        cs_data->remaining_ticks_slice=prio_quantum;         
        t->on_rq=TRUE;

    /* If the task was not using the current CPU, check whether a preemption is in order or not */
	if (!preempted) {
		task_t* current=rq->cur_task;
        //t->last_preempted = preempted;
		/* Trigger a preemption if this task has a shorter CPU burst than current */		
    if (preemptive_scheduler && !is_idle_task(current) && t->prio<current->prio) {
			rq->need_resched=TRUE;
			current->flags|=TF_INSERT_FRONT; /* To avoid unfair situations in the event
                                                //another task with the //same length wakes up as well*/
		}
	}
}

static void task_tick_prio(runqueue_t* rq)
{
	task_t* current=rq->cur_task;
	struct prio_data* cs_data=(struct prio_data*) current->tcs_data;

	if (is_idle_task(current))
		return;

	cs_data->remaining_ticks_slice--; /* Charge tick */

	if (cs_data->remaining_ticks_slice<=0)
		rq->need_resched=TRUE; //Force a resched !!
}

static task_t* steal_task_prio(runqueue_t* rq)
{
	task_t* t=tail_slist(&rq->tasks);

	if (t) 
		remove_slist(&rq->tasks,t);
	     t->on_rq=FALSE;
        
	return t;
}


sched_class_t prio_sched= {
    .task_new=task_new_prio,
	.pick_next_task=pick_next_task_prio,
	.enqueue_task=enqueue_task_prio,
    .task_tick=task_tick_prio,
	.steal_task=steal_task_prio
};
