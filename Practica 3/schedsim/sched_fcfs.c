/*
Archivo FCFS que nos da el profesor hecho para la pr 3
Instalar gnuplot, python (Está instalado en la màquina virtual)
*/

#include "sched.h"
int fcfs_quantum = 3;

struct fcfs_data {
	int remaining_ticks_slice;
};

static int task_new_fcfs(task_t* t)
{
	struct fcfs_data* cs_data=malloc(sizeof(struct fcfs_data));

	if (!cs_data)
		return 1;  /* Cannot reserve memory */

	/* initialize the quantum */
	cs_data->remaining_ticks_slice=fcfs_quantum;
	t->tcs_data=cs_data;
	return 0;
}


static task_t* pick_next_task_fcfs(runqueue_t* rq){
	task_t* t = head_slist(&rq->tasks);
	if (t){
		remove_slist(&rq->tasks,t);
	}
	return t;
}

static void enqueue_task_fcfs(task_t* t, runqueue_t* rq, int preemted){

struct fcfs_data* cs_data=(struct fcfs_data*) t->tcs_data;
	if(t->on_rq || is_idle_task(t)){
		return;
	}
    else{
		insert_slist(&rq->tasks,t);
        cs_data->remaining_ticks_slice=fcfs_quantum;
	}
}

static void task_tick_fcfs(runqueue_t* rq)
{
	task_t* current=rq->cur_task;
	struct fcfs_data* cs_data=(struct fcfs_data*) current->tcs_data;

	if (is_idle_task(current))
		return;

	cs_data->remaining_ticks_slice--; /* Charge tick */

	if (cs_data->remaining_ticks_slice<=0)
		rq->need_resched=TRUE; //Force a resched !!
}

static task_t* steal_task_fcfs (runqueue_t* rq){
	task_t* t = tail_slist(&rq -> tasks);
	if(t)
		remove_slist(&rq->tasks,t);
        return t;
	
}

sched_class_t fcfs_sched = {// Para el prio también hay que ejecutar estas tres funciones
        .task_new=task_new_fcfs,
		.pick_next_task = pick_next_task_fcfs,
		.enqueue_task = enqueue_task_fcfs,
        .task_tick=task_tick_fcfs,
		.steal_task = steal_task_fcfs,
};

/*
Modificar enum del shced.h para añadir el nuevo pranificador y cambiar el avaible añadiendo el nuevo planificador
Y TAMBIEN EL SHEDULIN EXTERN O ALGO SI
*/
