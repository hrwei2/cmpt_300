#include "list.h"
enum state {running, ready, blocked};

struct PCB {
  int PID;
  int n_PID;//id of the send to
  int priority;
  enum state process_state;
  char* msg;
};

struct Semaphore {
  int value;
  List* processes;// only currently runing proceses
};
struct Semaphore*  semaphores [5];
struct PCB * executing;
struct PCB * init;
void List_init();
void List_shutdown();

void create_routine();
void fork_routine();
void kill_routine();
void exit_routine();
void quantum_routine();

void send_routine();
void receive_routine();
void reply_routine();

void new_semaphore_routine();
void semaphore_p_routine();
void semaphore_v_routine();

void procinfo_routine();
void totalinfo_routine();

//ask if we can exit even though there are pcoesses still bloceked?
//answer: print error. already did
//ask about multiple receives
//first recieve gets the first id, the second recieve gets the sencond send id
//as what happens when a process is unblocked, does it go to the end of the ready queue, or immediatly executs
//answer: sends to the end of ready queue
//when does the message start prints, when the process next gets to the execution state, or immediatly the process ends
//answer next time it executes the process will be displayed
//two sends to the same id what happens?
