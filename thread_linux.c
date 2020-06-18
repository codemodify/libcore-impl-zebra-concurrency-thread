#ifdef libCoreLinux

#include <libcore-concurrency-thread/thread.h>
#include <stdlib.h>	  // malloc
#include <memory.h>	  // memset
#include <pthread.h>  // pthread_*

typedef pthread_t ThreadHandle;
typedef pthread_attr_t ThreadAttributes;

bool isPointerNull(AbstractPointer pointer) {
	return (pointer == nil);
}

ThreadRef ThreadNew() {
	ThreadRef thread = (ThreadRef)malloc(sizeof(Thread));
	memset(thread, 0, sizeof(Thread));
	return thread;
}

void ThreadDelete(ThreadRefRef thread) {
	if (isPointerNull(thread) || isPointerNull(*thread)) {
		return;
	}

	free(*thread);
	*thread = nil;
}

void threadRun(ThreadRef thread, ThreadInputData inputData, bool waitForFinish) {
	if (isPointerNull(thread)) {
		return;
	}

	if (thread->ThreadOnSart != nil) {
		thread->ThreadOnSart(inputData);
	}

	if (thread->CodeToRun != nil) {
		ThreadHandle threadHandle;

		ThreadAttributes threadAttributes;
		pthread_attr_init(&threadAttributes);

		pthread_create(
			&threadHandle,
			&threadAttributes,
			thread->CodeToRun,
			inputData);

		thread->houseKeeping = (AbstractPointer)threadHandle;

		if (waitForFinish) {
			ThreadWaitForFinish(thread);
		}
	}

	if (thread->ThreadOnFinish != nil) {
		thread->ThreadOnFinish(inputData);
	}
}

void ThreadRun(ThreadRef thread, ThreadInputData inputData) {
	return threadRun(thread, inputData, true);
}

void ThreadRunAsync(ThreadRef thread, ThreadInputData inputData) {
	return threadRun(thread, inputData, false);
}

void ThreadWaitForFinish(ThreadRef thread) {
	if (isPointerNull(thread)) {
		return;
	}

	pthread_join((ThreadHandle)thread->houseKeeping, nil);
}

#endif