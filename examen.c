#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void padre(int segnal){
printf("\n padre dice al hijo :  recibido tu mensaje hijo --> ");
}

void hijo(int segnal){
printf("\n hijo dice al padre :  te envio otro mensaje ");
}

int main(){
int pid, pid_hijo; //proceso hijo y padre
int tuberia[2];
int cont_mensaje = 0;
char buffer[30];

pipe(tuberia); //Se crea la tuberia
pid = getpid(); // pid padre
pid_hijo = fork(); //se crea el hijo


switch(pid_hijo){

  case -1: // error al crear el hijo
  printf("el hijo no se ha podido crear\n");
  exit(-1);
  break;

  case 0: // hijo

  while(cont_mensaje != 4){

    wait(NULL);
    signal(SIGUSR1, hijo); // envia señal al void hijo
    pause(); //Hijo espera
    sleep(1);
    kill(pid, SIGUSR1); // señal al padre
    close(tuberia[0]);
    write(tuberia[1],"hola daddy finger\n" , 30); // el hijo escribe en el pipe el mensaje
  }
   break;

  default:// padre

  while(cont_mensaje != 4){

      cont_mensaje++;
      signal(SIGUSR1, padre); // envia señal al void padre
      sleep(1);
      kill(pid_hijo, SIGUSR1); // señal al hijo
      pause(); //padre espera hasta que llegue una señal del hijo
      close(tuberia[1]);
      read(tuberia[0], buffer, sizeof(buffer)); // el padre lee el mensaje del pipe
      printf("mensaje %d enviado por el hijo: ", cont_mensaje); // se imprime el numero del contador
      printf("%s",buffer); // se imprime el mensaje que mando el hijo
  }
   break;
}
exit(0);
}
