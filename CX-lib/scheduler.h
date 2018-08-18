#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <atask.h>
#include <Arduino.h>

#define DEBUG 1

class Scheduler
{
private:
	ATask* firstTask, *lastTask;

public:

	Scheduler()
	{
		firstTask = lastTask = NULL;
	}

	void addTask(ATask* task)
	{
		if (firstTask!=NULL) 
		{
			lastTask->nextTask = task;
			lastTask = task;
		}
		else
		{
			firstTask = task;
      		lastTask = task;
		}
	}

	void update()
	{
		// loop through tasks
		int minPriority = 0;
		int selected = 0;
		ATask* task = firstTask;
		ATask* selected_task = NULL;
    	unsigned long now = millis();
#if DEBUG
    	int count_task = 0;
#endif
		while(task != NULL)
		{
			int p = task->getPriority(now);
			if (p < minPriority)
			{
				minPriority = p;
				selected_task = task;
				selected = count_task - 1;
			}
			task = task->nextTask;
		}
		if (selected_task) 
	    {
	        selected_task->run();
	    }
	}
};

#endif // SCHEDULER_H