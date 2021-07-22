The way I implemented round robin is that the system would take turns executing from each ready queue.
ie the system would first execute queue_1, then queue_2, then queue_3 than back to queue_1 etc... This is because
if the system only prioritized queue_1, then only moves on to queue_2 when all elements of queue_1 finishes executing,
the potentially, the user would never be able to reach queue_3 if there is an infinite amount of processes in queue_1 and queue_2.
