typedef void Sigfunc_rt(int, siginfo_t *, void *);

Sigfunc_rt * Signal_rt(int signo, Sigfunc_rt *func, sigset_t *mask)
{
	struct sigaction act, oact;
	act.sa_sigaction = func;
	act.sa_mask = *mask;
	act.sa_flags = SA_SIGINFO;
	if (signo == SIGALRM){
#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;
#endif
	}else{
#ifdef SA_RESATRT
		act.sa_flags |= SA_RESTART;
#endif
	}

	if (sigaction(signo, &act, &oact)<0){
		return ((Sigfunc_rt *) SIG_ERR);
	}

	return (oact.sa_sigaction);
}
