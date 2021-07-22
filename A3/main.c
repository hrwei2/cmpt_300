#include "Comand.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  List_init();
  while (init != NULL){
        char e;
        int ch;
        printf("Mode: \n");
        scanf(" %c", &e);
        if ((ch = getchar()) != '\n'){
            printf("invalid input!\n");
            continue;
        }
        if (e == 'C' || e == 'c')
          create_routine();
        else if (e == 'F' || e == 'f')
          fork_routine();
        else if (e == 'K' || e == 'k')
          kill_routine();
        else if (e == 'E' || e == 'e')
          exit_routine();
        else if (e == 'Q' || e == 'q')
          quantum_routine();
        else if (e == 'S' || e == 's')
          send_routine();
        else if (e == 'R' || e == 'r')
          receive_routine();
        else if (e == 'Y' || e == 'y')
          reply_routine();
        else if (e == 'N' || e == 'n')
          new_semaphore_routine();
        else if (e == 'P' || e == 'p')
          semaphore_p_routine();
        else if (e == 'V' || e == 'v')
          semaphore_v_routine();
        else if (e == 'I' || e == 'i')
          procinfo_routine();
        else if (e == 'T' || e == 't')
          totalinfo_routine();

        else {
          printf("Invalid input!\n");
          continue;
        }
    }

  List_shutdown();
  return 0;
}
