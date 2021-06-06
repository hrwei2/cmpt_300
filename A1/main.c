#include "list.h"
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
static void my_list_printf_function(List* list){

  struct Node_s * current = list->head;
  int count = 0;
  printf("\n");
  while(current != NULL){
    count++;
    printf("%d -> ", *(int *)current->item);
    current = current->next;
  }
  current = list->tail;
  printf("\n");
  while(current != NULL){
    printf("%d -> ", *(int *)current->item);
    current = current->prev;
  }
  printf("\n\n");
  printf("number of elements in the list:%d\n", list->count);
  printf("actuall num of elements in the list: %d\n", count);
  if (list->head == NULL && list->tail == NULL)
    printf("list is empty\n");
  if (list->head == NULL && list->tail != NULL)
    printf("just head is empty\n");
  if (list->head != NULL && list->tail == NULL)
    printf("just tail is empty\n");
  if (list->head != NULL && list->tail != NULL){
    printf("head:%d\n", *(int *)list->head->item);
    printf("tail:%d\n", *(int *)list->tail->item);
  }
  if (list->current == NULL){
    if (list->state_curret == LIST_OOB_START)
      printf("current: out of bounds start\n");
    else
      printf("current: out of bounds end\n");
  }
  else
    printf("current:%d\n", *(int *)List_curr(list));
}
static void myFree(void* item){
  if (item != NULL)
    free(item);
}
static bool mycomp(void* pItem, void* pComparisonArg){
  return (*(int *)pItem == *(int *)pComparisonArg);
}


int main(){
List* L1 = List_create();
my_list_printf_function(L1);
int* ptr;
for (int i = 0; i < 5; i++){
  ptr = (int *)malloc(sizeof(int));
  *ptr = i;
  printf("adding %d to list\n",i);
  List_add(L1, ptr);
  my_list_printf_function(L1);

}
my_list_printf_function(L1);
printf("moving current to begining of list\n");
List_first(L1);
ptr = (int *)malloc(sizeof(int));
*ptr = 5;
printf("adding %d to list\n",5);
List_add(L1, ptr);
my_list_printf_function(L1);
printf("moving current to next in list\n");
List_next(L1);
ptr = (int *)malloc(sizeof(int));
*ptr = 6;
printf("adding %d to list\n",6);
List_add(L1, ptr);
printf("moving current to begining of list\n");
List_first(L1);
printf("moving current to prev in list\n");
List_prev(L1);
ptr = (int *)malloc(sizeof(int));
*ptr = 7;
printf("adding %d to list\n",7);
List_add(L1, ptr);
my_list_printf_function(L1);
printf("moving current to end of list\n");
List_last(L1);
printf("moving current to next in list\n");
List_next(L1);
ptr = (int *)malloc(sizeof(int));
*ptr = 8;
printf("adding %d to list\n",8);
List_add(L1, ptr);
my_list_printf_function(L1);
List_free(L1, myFree);
printf("\n\n\n------------end of add fuction test----------\n\n\n");
L1 = List_create();
for (int i = 0; i < 5; i++){
  ptr = (int *)malloc(sizeof(int));
  *ptr = i;
  printf("adding %d to list\n",i);
  List_insert(L1, ptr);
  my_list_printf_function(L1);

}
my_list_printf_function(L1);
printf("moving current to begining of list\n");
List_first(L1);
ptr = (int *)malloc(sizeof(int));
*ptr = 5;
printf("inserting %d to list\n",5);
List_insert(L1, ptr);
my_list_printf_function(L1);
printf("moving current to next in list\n");
List_next(L1);
ptr = (int *)malloc(sizeof(int));
*ptr = 6;
printf("inserting %d to list\n",6);
List_insert(L1, ptr);
printf("moving current to begining of list\n");
List_first(L1);
printf("moving current to prev in list\n");
List_prev(L1);
ptr = (int *)malloc(sizeof(int));
*ptr = 7;
printf("inserting %d to list\n",7);
List_insert(L1, ptr);
my_list_printf_function(L1);
printf("moving current to end of list\n");
List_last(L1);
printf("moving current to next in list\n");
List_next(L1);
ptr = (int *)malloc(sizeof(int));
*ptr = 8;
printf("inserting %d to list\n",8);
List_insert(L1, ptr);
my_list_printf_function(L1);
printf("moving current to end in list\n");
List_last(L1);
ptr = (int *)malloc(sizeof(int));
*ptr = 9;
printf("inserting %d to list\n",9);
List_insert(L1, ptr);
my_list_printf_function(L1);
printf("test\n");
List_free(L1, myFree);
printf("\n\n\n------------end of insert fuction test----------\n\n\n");
L1 = List_create();
for (int i = 0; i < 20; i++){
  ptr = (int *)malloc(sizeof(int));
  *ptr = i;
  List_append(L1, ptr);
  if (i < 5){
    printf("appending %d to list\n",i);
    my_list_printf_function(L1);
  }

}
printf("appending %d nodes to list\n",20-5);

printf("\n\n\n------------end of appending fuction test----------\n\n\n");
List_free(L1, myFree);
printf("trimming %d nodes to list\n",20-5);
for (int i = 20; i > 0; i--){
  free(List_trim(L1));
  if (i < 5){
    printf("trimming list\n");
    my_list_printf_function(L1);
  }
}
printf("\n\n\n------------end of trimming fuction test----------\n\n\n");
L1 = List_create();
for (int i = 0; i < 10; i++){
  ptr = (int *)malloc(sizeof(int));
  *ptr = i;
  List_prepend(L1, ptr);
  if (i < 5){
    printf("prepending %d to list\n",i);
    my_list_printf_function(L1);
  }
}
printf("prepending %d nodes to list\n",20-5);
printf("\n\n\n------------end of prepending fuction test----------\n\n\n");
for (int i = 0; i < 5; i++){
  free(List_remove(L1));
  if (i > 5){
    printf("removing from list\n");
    my_list_printf_function(L1);
  }
}
printf("moving to begining of list\n");
List_first(L1);
printf("removing from list\n");
free(List_remove(L1));
my_list_printf_function(L1);
printf("moving 2 to the left\n");
List_next(L1);
List_next(L1);
printf("current:%d\n", *(int *)List_curr(L1));
printf("removing from list\n");
free(List_remove(L1));
my_list_printf_function(L1);
printf("moving to end of list\n");
printf("removing from list\n");
free(List_remove(L1));
my_list_printf_function(L1);
printf("remove 2 more\n");
free(List_remove(L1));
free(List_remove(L1));
my_list_printf_function(L1);
printf("try to remove once more\n");
if (List_remove(L1) == NULL){
  printf("No more in list returns NULL\n");
}
else{
  printf("error\n");
}
List_free(L1,myFree);
printf("\n\n\n------------end of removing fuction test----------\n\n\n");
L1 = List_create();
for (int i = 0; i < 10; i++){
  ptr = (int *)malloc(sizeof(int));
  *ptr = i;
  List_append(L1, ptr);
}
printf("appending 20 elements to list\n");
printf("L1:\n");
my_list_printf_function(L1);
printf("creating L2\n");
List* L2 = List_create();
printf("concat(L1,L2)\n");
List_concat(L1, L2);
printf("L1:\n");
my_list_printf_function(L1);
printf("creating L2\n");
L2 = List_create();

printf("concat(L2,L1)\n");
List_concat(L2, L1);
printf("L2:\n");
my_list_printf_function(L2);


printf("creating L1\n");
L1 = List_create();
for (int i = 10; i < 15; i++){
  ptr = (int *)malloc(sizeof(int));
  *ptr = i;
  List_append(L1, ptr);
}
printf("L1:\n");
my_list_printf_function(L1);
printf("concat(L2,L1)\n");
List_concat(L2, L1);
printf("L2:\n");
my_list_printf_function(L2);
printf("\n\n\n------------end of concat fuction test----------\n\n\n");
printf("creating empty list\n");
L1 = List_create();
printf("searching empty list\n");
if (List_search(L1, mycomp, NULL) == NULL)
  printf("returns NULL since empty lists\n");
printf("search this list:\n");
my_list_printf_function(L2);
printf("setting current to begining of list\n");
List_first(L2);
printf("search for: %d\n", *(int *)List_last(L2));
if (List_search(L2, mycomp, List_last(L2)) == NULL)
  printf("returns NULL element not in list\n");
else
  printf("element is found\n");
printf("setting current to begining of list\n");
List_first(L2);
ptr = (int *)malloc(sizeof(int));
*ptr = 100;
printf("search for: %d\n", 100);
if (List_search(L2, mycomp, ptr) == NULL)
  printf("returns NULL element not in list\n");
else
  printf("element is found\n");
printf("setting current to start out of bounds of list\n");
List_first(L2);
List_prev(L2);
printf("search for: %d\n", *(int *)List_last(L2));
if (List_search(L2, mycomp, List_last(L2)) == NULL)
  printf("returns NULL element not in list\n");
else
  printf("element is found\n");
printf("setting current to end out of bounds of list\n");
List_last(L2);
List_next(L2);
printf("search for: %d\n", *(int *)List_last(L2));
if (List_search(L2, mycomp, List_last(L2)) == NULL)
  printf("returns NULL element not in list\n");
else
  printf("element is found\n");
printf("setting current to begining of list\n");
List_first(L2);
printf("search for: %d\n", *(int *)List_first(L2));
if (List_search(L2, mycomp, List_first(L2)) == NULL)
  printf("returns NULL element not in list\n");
else
  printf("element is found\n");
free(ptr);
printf("\n\n\n------------end of search fuction test----------\n\n\n");
List_free(L1,myFree);
List_free(L2,myFree);
printf("end of program\n");
}
