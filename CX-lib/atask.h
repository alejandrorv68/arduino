#ifndef ATASK_H
#define ATASK_H

class ATask
{

protected:
	unsigned long nextRunTime;

public:
	ATask* nextTask;
	unsigned long nextRunTime_ = 0;
	int update_interval_;

	ATask()	{
		nextTask = NULL;
	}

	virtual int getPriority(unsigned long now) = 0;
	virtual bool run() = 0;
};


#endif // ATASK_H