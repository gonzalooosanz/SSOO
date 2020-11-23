pthread_mutex_t mutex;
pthread_cond_t lect;
pthread_cond_t escrit;
int num_lect;
int num_escr;


ReadEnter(){
	lock(&mutex);
	num_lect++;
	while(num_escr > 0 ){
		cond_wait(mutex, lect);
	}
	unlock(&mutext);
}


ReadExit(){
	lock(&mutex);
	num_lect--;
	if(num_lect == 0){
		cond_signal(escrit);
	}
	unlock(&mutext);
}

WriteEnter(){
	lock(mutex);

	while(num_escr > 0|| num_lect > 0){
		cond_wait(mutext, escrit);
	}
	num_escr++;
	unlock(mutex);
	
}


WriteExit(){
	lock(mutext);
	num_escr--;
	if( num_lect == 0)
		cond_signal(escrit);
	else 
		cond_broadcast(lect); // obligatorio hacer un broadcast
	unlock(mutext);
}