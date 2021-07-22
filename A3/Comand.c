//still need to check if id is unique when creating new id during int overflow
#include "Comand.h"
#include "list.h"
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
// ready queues
static List* ready_queue_1;
static List* ready_queue_2;
static List* ready_queue_3;
// queue of processes waiting on send
static List* send_op;
// a queue of processes waiting on a receive operation
static List* rev_op;
//coutner for id;
static int counter = 0;
static int num_process = 0;// count the number of all processes
static int next_queue = 0;// for schedule_next function

static void PCBFree(void* temp){
  struct PCB* node = (struct PCB*)temp;
  free(node->msg);
  free(node);
}
static bool PCDIDcomp(void* pItem, void* pComparisonArg){
  struct PCB * temp1 = (struct PCB *) pItem;
  int temp2 = *(int *) pComparisonArg;
  return (temp1->PID == temp2);
}
static bool PCDIDcomp_rev(void* pItem, void* pComparisonArg){//pComparator is the id of the receive process
  struct PCB * temp1 = (struct PCB *) pItem;
  int temp2 = *(int *) pComparisonArg;
  return (temp1->n_PID == temp2 && temp1->msg != NULL);
}

static void* pop_queue(List* pList){
  List_first(pList);
  return List_remove(pList);
}
//returns list* on sucess, and NULL on failure
static List* find_id (int id){//leaves current at location that matches id
  printf("Searching through ready_queue_1\n");
  List_first(ready_queue_1);
  struct PCB * temp = (struct PCB *) List_search(ready_queue_1, PCDIDcomp, (void*)&id);
  if (temp != NULL){
    printf("process found\n");
    return ready_queue_1;
  }
  printf("process not found.\n");
  printf("Searching through ready_queue_2\n");
  List_first(ready_queue_2);
  temp = (struct PCB *) List_search(ready_queue_2, PCDIDcomp, (void*)&id);
  if (temp != NULL){
    printf("process found\n");
    return ready_queue_2;
  }
  printf("process not found.\n");
  printf("Searching through ready_queue_3\n");
  List_first(ready_queue_3);
  temp = (struct PCB *) List_search(ready_queue_3, PCDIDcomp, (void*)&id);
  if (temp != NULL){
    printf("process found\n");
    return ready_queue_3;
  }
  printf("process not found.\n");
  printf("Searching through send_queue\n");
  List_first(send_op);
  temp = (struct PCB *) List_search(send_op, PCDIDcomp, (void*)&id);
  if (temp != NULL){
    printf("process found\n");
    return send_op;
  }
  printf("process not found.\n");
  printf("Searching through receive_queue\n");
  List_first(rev_op);
  temp = (struct PCB *) List_search(rev_op, PCDIDcomp, (void*)&id);
  if (temp != NULL){
    printf("process found\n");
    return rev_op;
  }
  for (int i = 0; i < 5; i++){
    if (semaphores[i] != NULL){
      printf("process not found.\n");
      printf("Searching through semaphore: %d\n",i);
      List_first((semaphores[i])->processes);
      temp = (struct PCB *) List_search((semaphores[i])->processes, PCDIDcomp, (void*)&id);
      if (temp != NULL){
        printf("process found\n");
        return (semaphores[i])->processes;
      }
    }
  }
  printf("No such process in system.\n");
  return NULL;
}

static void schedule_next(){//finds the next available and sets the executing as the next thing. if there are no ready, then just do nothng.
  if (List_count(ready_queue_1) == 0 && List_count(ready_queue_2) == 0 && List_count(ready_queue_3) == 0){
    executing = init;
    printf("Currently executing process: init\n");
    return;
  }
  while (1){
    if (next_queue == 0){
        next_queue = 1;
        if (List_count(ready_queue_1) != 0){
          executing = (struct PCB *)pop_queue(ready_queue_1);
          executing->process_state = running;
          printf("Currently executing process: %d\n",executing->PID);
          if (executing->msg != NULL){
            printf("message:%s\n", executing->msg);
            free(executing->msg);
            executing->msg = NULL;
          }
          return;
        }
    }
    if (next_queue == 1){
      next_queue = 2;
      if (List_count(ready_queue_2) != 0){
        executing = (struct PCB *)pop_queue(ready_queue_2);
        executing->process_state = running;
        printf("Currently executing process: %d\n",executing->PID);
        if (executing->msg != NULL){
          printf("message:%s\n", executing->msg);
          free(executing->msg);
          executing->msg = NULL;
        }
        return;
      }
    }
    if (next_queue == 2){
      next_queue = 0;
      if (List_count(ready_queue_3) != 0){
        executing = (struct PCB *)pop_queue(ready_queue_3);
        executing->process_state = running;
        printf("Currently executing process: %d\n",executing->PID);
        if (executing->msg != NULL){
          printf("message:%s\n", executing->msg);
          free(executing->msg);
          executing->msg = NULL;
        }
        return;
      }
    }
  }

}
static int insert_ready_queue (struct PCB * node, int priority){
  int result = 0;
  node->process_state = ready;
  if (priority == 0)
    result = List_append(ready_queue_1,(void*)node);
  if (priority == 1)
    result = List_append(ready_queue_2,(void*)node);
  if (priority == 2)
    result = List_append(ready_queue_3,(void*)node);
  return result;
}

static int create(int priority){// need a correct id created
  if (priority < 0 || priority >  2){
    printf("Error incorrect input for \n");
    return -1;
  }
  struct PCB* node = malloc (sizeof (struct PCB));
  node->PID = counter;
  node->priority = priority;
  node->process_state = ready;

  int result = insert_ready_queue (node, priority);
  if (result == 0){
    printf("Process creation scuessful, pid = %d\n", counter);
    counter++;
    num_process++;
  }

  if (result == -1){
    printf("Insert failed\n");
    free(node->msg);
    free(node);
  }

  return result;
}

void List_init(){
  init = malloc (sizeof (struct PCB));
  init->PID = -1;
  executing = init;

  for (int i = 0; i < 5; i++){
    semaphores[i] = NULL;
  }

  ready_queue_1 = List_create();
  ready_queue_2 = List_create();
  ready_queue_3 = List_create();

  send_op = List_create();
  rev_op = List_create();
}

void List_shutdown(){
  List_free(ready_queue_1, PCBFree);
  List_free(ready_queue_2, PCBFree);
  List_free(ready_queue_3, PCBFree);
  List_free(send_op, PCBFree);
  List_free(rev_op, PCBFree);
}

void create_routine(){
  if (num_process == (LIST_MAX_NUM_NODES - 1)){
    printf("Error, max number of processes reached\n");
    return;
  }
  int priority = -1;
  while (priority < 0 || priority >  2){
    char line[256];
    int isint;
    while (1) {
      printf("Enter priority:");
      fgets(line, sizeof line, stdin);
      isint = sscanf(line, "%d",&priority);
      if (isint)
        break;
      printf("You did not enter a number.\n Please inpute a number between 0 and 2\n");
    }
    if (priority < 0 || priority >  2)
      printf("Please inpute a number between 0 and 2\n");
  }
  int result = create(priority);
  if (result == -1){
    printf("Create failed.\n");
    return;
  }
}
void fork_routine (){
  if (num_process == (LIST_MAX_NUM_NODES - 1)){
    printf("Error, max number of processes reached\n");
    return;
  }
  if (executing == init){
    printf("Failure, forking init process\n");
    return;
  }
  struct PCB* node = malloc (sizeof (struct PCB));
  node->PID = counter;
  node->priority = executing->priority;
  node->process_state = ready;
  node->msg = executing->msg;
  int result = insert_ready_queue (node, executing->priority);

  if (result == 0){
    printf("Fork scuessful, pid = %d\n", counter);
    counter++;
    num_process++;
  }

  if (result == -1){
    printf("Fork failed\n");
    free(node->msg);
    free(node);
  }
  return;
}

void kill_routine(){
  char line[256];
  int id;
  int isint;
  while (1) {
    printf("Enter PID:");
    fgets(line, sizeof line, stdin);
    isint = sscanf(line, "%d",&id);
    if (isint)
      break;
    printf("You did not enter a number.\n Please inpute a number.\n");
  }

  if (id == -1){//if init process
    if (num_process == 0){
      printf("Removing init processes\n");
      free(init);
      free(init->msg);
      executing = NULL;
      init = NULL;
      return;
    }
    else {//ask if we can exit even though there are pcoesses still blocekd
      printf("processes are still blocked cannot kill init\n");
      return;
    }
  }
  if (id == executing->PID){//
    printf("removing process.\n");
    struct PCB * temp = executing;
    schedule_next();//finds the next available and sets the executing as the next thing. if there are no ready, then just do nothng.
    free(temp->msg);
    free(temp);
    num_process--;
    return;
  }
  List* result = find_id (id);
  if (result == NULL){
    printf("kill Failed\n");
    return;
  }
  printf("removing process\n");
  struct PCB * temp2 = (struct PCB *) List_remove(result);
  free(temp2->msg);
  free(temp2);
  num_process--;
  printf("process removed\n");
  printf("kill successful\n");
  //missing search for the process
}
void exit_routine(){

  if (executing == init){
    if (num_process == 0){
      printf("removing init process\n");
      free(init->msg);
      free(init);
      executing = NULL;
      init = NULL;
      return;
    }
    else {//ask if we can exit even though there are pcoesses still blocekd
      printf("processes are still blocked cannot exit from init\n\n");
      return;
    }
  }
  printf("exiting process\n\n");
  struct PCB * temp = executing;
  schedule_next();//finds the next available and sets the executing as the next thing. if there are no ready, then just do nothng.
  free(temp->msg);
  free(temp);
  num_process--;
}
void quantum_routine(){
  printf("Executing Quantum\n");
  if (executing == init){
    schedule_next();//finds the next available and sets the executing as the next thing. if there are no ready, then just do nothng.
    return;
  }
  struct PCB * node = executing;
  schedule_next();//finds the next available and sets the executing as the next thing. if there are no ready, then just do nothng.
  int result = insert_ready_queue (node, node->priority);
  if (result == 0){
      printf("pid = %d inserted into queue.\n", node->PID);
      printf("Quantum Sucess\n");
  }
  if (result == -1){
    printf("quantum failed\n");
    free(node->msg);
    free(node);
  }
  return;
}
void send_routine(){
  if (executing == init){
    printf("Send Failed, init process can not be blocked.\n");
    return;
  }
  char line[256];
  int id;
  int isint;
  List* temp;
  while (1) {
    printf("Enter PID:");
    fgets(line, sizeof line, stdin);
    isint = sscanf(line, "%d",&id);
    if (isint)
      break;
    printf("You did not enter a number.\n Please inpute a number.\n");
  }
  if (executing->PID == id){
    printf("ID found\n");
  }
  else {
    temp = find_id (id);
    if (temp == NULL){
      printf("send failed\n");
      return;
    }
  }
  executing->n_PID = id;
  char* str = malloc(40*sizeof(char));
  printf("Enter message (max 40 characters): ");
  fgets(str, 40, stdin);
  executing->msg = str;
  executing->process_state = blocked;
  int result = 0;
  result = List_append(send_op,(void*)executing);
  if (result == 0){
    if (executing->PID != id){
      if (temp == rev_op){//check if the rev from has the same id as the send
        struct PCB * temp = (struct PCB *) List_remove(rev_op);
        temp->msg = str;
        temp->process_state = ready;
        executing->msg = NULL;
        int result2 = insert_ready_queue (temp, temp->priority);

        if (result2 == -1){
          printf("receive failed unblock\n");
          free(temp->msg);
          free(temp);
          num_process--;
        }
        else {
          printf("Receive unblocked.\n");
        }
        }
    }
    printf("Send successful.\n");
    schedule_next();
    return;
  }
  if (result == -1){
    printf("send failed\n");
    return;
  }
}

void receive_routine(){//can multiple recieve get the same send?
  if (executing == init && num_process == 0){
    printf("Receive Failed, init process can not be blocked.\n");
    return;
  }
  List_first(send_op);
  struct PCB * temp = (struct PCB *) List_search(send_op, PCDIDcomp_rev, (void*)&(executing->PID));

  if (temp != NULL){
    printf("message: %s\n",temp->msg);
    free(temp->msg);
    temp->msg = NULL;
    return;
  }
  else if (executing != init){
    executing->process_state = blocked;
    int result = List_append(rev_op,(void*)executing);
    if (result == 0){
      printf("Receive sucess\n\n");
      schedule_next();
      return;
    }
    else if (result == -1){
      printf("Receive Failed.\n");
      return;
    }
  }
  else{
    printf("Receive Failed, init process can not be blocked.\n");
    return;
  }
}

void reply_routine(){
  char line[256];
  int id;
  int isint;
  while (1) {
    printf("Enter PID:");
    fgets(line, sizeof line, stdin);
    isint = sscanf(line, "%d",&id);
    if (isint)
      break;
    printf("You did not enter a number.\n Please inpute a number.\n");
  }
  List_first(send_op);
  struct PCB * temp = (struct PCB *) List_search(send_op, PCDIDcomp, (void*)&id);

  if (temp != NULL){
    temp = List_remove(send_op);
    free(temp->msg);
    char* str = malloc(40*sizeof(char));
    printf("Enter message (max 40 characters): ");
    fgets(str, 40, stdin);
    temp->msg = str;
    int result = insert_ready_queue (temp , temp->priority);
    if (result == 0){
      printf("Reply sucess\n");
      printf("Send unblocked\n");
    }
    else{
      printf("Reply Failed\n");
    }

  }
  else {
    printf("Reply Failed, PID not found\n");
  }
}
void new_semaphore_routine(){
  int id = -1;
  char line[256];
  int isint;
  while (id < 0 || id >  4){
    while (1) {
      printf("Enter ID:");
      fgets(line, sizeof line, stdin);
      isint = sscanf(line, "%d",&id);
      if (isint)
        break;
      printf("You did not enter a number.\n Please inpute a number between 0 and 4\n");
    }
    if (id < 0 || id >  4)
      printf("Please inpute a number between 0 and 4\n");
  }
  if (semaphores[id] != NULL){
    printf("Error ID already in use\n");
    return;
  }
  semaphores[id] = malloc(sizeof(struct Semaphore));
  (semaphores[id])->processes = List_create();
  if ((semaphores[id])->processes == NULL){
    printf("Error, too many lists\n");
    free(semaphores[id]);
    return;
  }
  int init;
  while (1) {
    printf("Enter initial value (0 or higher):");
    fgets(line, sizeof line, stdin);
    isint = sscanf(line, "%d",&init);
    if (isint && init >= 0)
      break;
    printf("Error please input a integer 0 or higher.\n");
  }
  semaphores[id]->value = init;
  printf("success\n");
}

void semaphore_p_routine(){
  int id = -1;
  char line[256];
  int isint;
  while (id < 0 || id >  4){
    while (1) {
      printf("Enter ID:");
      fgets(line, sizeof line, stdin);
      isint = sscanf(line, "%d",&id);
      if (isint)
        break;
      printf("You did not enter a number.\n Please inpute a number between 0 and 4\n");
    }
    if (id < 0 || id >  4)
      printf("Please inpute a number between 0 and 4\n");
  }
  if (semaphores[id] == NULL){
    printf("Error ID does not exist\n");
    return;
  }
  if ((semaphores[id])->value == 0){
    executing->process_state = blocked;
    int result = List_append((semaphores[id])->processes,(void*)executing);
    if (result == 0){
      printf("block success\n");
      schedule_next();
      return;
    }
    else if (result == -1){
      printf("block Failed.\n");
      executing->process_state = running;
      return;
    }
  }
  else{
    printf("decormenting the value of semanphore\n");
    (semaphores[id])->value--;
  }
  printf("success\n");
}
void semaphore_v_routine(){//When semaphore is unblocked does it go to the back of the queue or become execution
  int id = -1;
  char line[256];
  int isint;
  while (id < 0 || id >  4){
    while (1) {
      printf("Enter ID:");
      fgets(line, sizeof line, stdin);
      isint = sscanf(line, "%d",&id);
      if (isint)
        break;
      printf("You did not enter a number.\n Please inpute a number between 0 and 2\n");
    }
    if (id < 0 || id >  4)
      printf("Please inpute a number between 0 and 4\n");
  }
  if (semaphores[id] == NULL){
    printf("Error ID does not exist\n");
    return;
  }
  if ((semaphores[id])->value == 0 && List_count((semaphores[id])->processes) != 0){
    struct PCB * node = pop_queue((semaphores[id])->processes);
    if (node == NULL){
      printf("List is empty Failure\n");
      return;
    }
    else{
      int result = insert_ready_queue (node, node->priority);
      if (result == -1){
        printf("Failure to insert into queue\n");
        return;
      }
      else{
        printf("Unblock Sucess.\n");
        return;
      }
    }
  }
  else{
    printf("incramenting the value of semanphore\n");
    (semaphores[id])->value++;
  }
  printf("success\n");
}

const char* getprocess_state(enum state process_state) {
   switch (process_state){
      case running: return "running";
      case ready: return "ready";
      case blocked: return "blocked";
   }
}

static void print_node(struct PCB * node){
  if (node -> msg != NULL)
    printf("\n%d \t\t %d \t %d \t\t %s \t %s",node->PID,node->n_PID,node->priority,getprocess_state(node->process_state),node->msg);
  else
    printf("\n%d \t\t %d \t %d \t\t %s \t NULL",node->PID,node->n_PID,node->priority,getprocess_state(node->process_state));
}
void procinfo_routine(){
  char line[256];
  int id;
  int isint;
  while (1) {
    printf("Enter PID:");
    fgets(line, sizeof line, stdin);
    isint = sscanf(line, "%d",&id);
    if (isint)
      break;
    printf("You did not enter a number.\n Please inpute a number.\n");
  }
  if (id == -1){
    printf("init process\n");
    return;
  }
  List* result = find_id(id);
  printf("\n");
  if (result == NULL){
    printf("PID does not exist in system\n");
    return;
  }
  if (result == ready_queue_1)
    printf("PID in ready_queue_1\n");
  else if (result == ready_queue_2)
    printf("PID in ready_queue_2\n");
  else if (result == ready_queue_3)
    printf("PID in ready_queue_3\n");
  else if (result == send_op)
    printf("PID in sending_queue\n");
  else if (result == rev_op)
    printf("PID in receive_queue\n");
  else{
    for (int i = 0; i < 5; i++){
      if (semaphores[i] != NULL && (semaphores[i])->processes == result){
        printf("PID in Semaphore: %d\n",i);
      }
    }
  }
  struct PCB * node = List_curr(result);
  printf("\nPID\t\t n_PID \t priority \t process_state \tmessage");
  printf("\n----------------------------------------------------------------");
  print_node(node);
  printf("\n\n");
  //print the node's info
}
void totalinfo_routine(){
  void * temp;
  printf("Currently executing:\n");
  printf("\nPID\t\t n_PID \t priority \t process_state \tmessage");
  printf("\n----------------------------------------------------------------");
  print_node(executing);
  printf("\n\n");
  printf("ready_queue_1:\n");
  printf("\nPID\t\t n_PID \t priority \t process_state \tmessage");
  printf("\n----------------------------------------------------------------");
  List_first(ready_queue_1);
  if (List_curr(ready_queue_1) != NULL){
    print_node(List_curr(ready_queue_1));
    temp = List_next(ready_queue_1);
    while (temp != NULL){
      print_node((struct PCB *)temp);
      temp = List_next(ready_queue_1);
    }
  }
  printf("\n\n");
  printf("ready_queue_2:\n");
  printf("\nPID\t\t n_PID \t priority \t process_state \tmessage");
  printf("\n----------------------------------------------------------------");
  List_first(ready_queue_2);
  if (List_curr(ready_queue_2) != NULL){
    print_node(List_curr(ready_queue_2));
    void * temp = List_next(ready_queue_2);
    while (temp != NULL){
      print_node((struct PCB *)temp);
      temp = List_next(ready_queue_2);
    }
  }
  printf("\n\n");
  printf("ready_queue_3:\n");
  printf("\nPID\t\t n_PID \t priority \t process_state \tmessage");
  printf("\n----------------------------------------------------------------");
  List_first(ready_queue_3);
  if (List_curr(ready_queue_3) != NULL){
    print_node(List_curr(ready_queue_3));
    void * temp = List_next(ready_queue_3);
    while (temp != NULL){
      print_node((struct PCB *)temp);
      temp = List_next(ready_queue_3);
    }
  }
  printf("\n\n");
  printf("rev_op:\n");
  printf("\nPID\t\t n_PID \t priority \t process_state \tmessage");
  printf("\n----------------------------------------------------------------");
  List_first(rev_op);
  if (List_curr(rev_op) != NULL){
    print_node(List_curr(rev_op));
    void * temp = List_next(rev_op);
    while (temp != NULL){
      print_node((struct PCB *)temp);
      temp = List_next(rev_op);
    }
  }
  printf("\n\n");
  printf("send_op:\n");
  printf("\nPID\t\t n_PID \t priority \t process_state \tmessage");
  printf("\n----------------------------------------------------------------");
  List_first(send_op);
  if (List_curr(send_op) != NULL){
    print_node(List_curr(send_op));
    void * temp = List_next(send_op);
    while (temp != NULL){
      print_node((struct PCB *)temp);
      temp = List_next(send_op);
    }
  }
  printf("\n\n");
  for (int i = 0; i < 5; i++){
    if (semaphores[i] != NULL){
      printf("Semaphore: %d\n", i);
      printf("Current value: %d\n", (semaphores[i])->value);
      printf("\nPID\t\t n_PID \t priority \t process_state \tmessage");
      printf("\n----------------------------------------------------------------");
      List_first((semaphores[i])->processes);
      if (List_curr((semaphores[i])->processes) != NULL){
        print_node(List_curr((semaphores[i])->processes));
        void * temp = List_next((semaphores[i])->processes);
        while (temp != NULL){
          print_node((struct PCB *)temp);
          temp = List_next((semaphores[i])->processes);
        }
      }
      printf("\n\n");
    }
  }
}
