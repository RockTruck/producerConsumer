#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>

pthread_t tid[2];	// Array für Thread id's
sem_t cEmpty;
sem_t cFull;
int counter = 0;        // Produktanzahl
int lagerGrosse = 10;
int itemBuffer[10];


//Prototypen
void producer(void *ptr);
void consumer(void *ptr);
void insertItem(int item);
void removeItem(int *item); 

void producer(void *ptr)   // Produzieren od. konsummieren je nach Eingabe
{
    while(1)
    {
        int event;
        scanf("%d",&event);
        if(event == 1)
        {
            //Mutex sperren
            sem_wait(&cEmpty);
            int item = rand()+1;
            printf("\n Produktion gestartet\n");
            insertItem(item);
            printf("\n Produktion beendet\n");
            printf("\n produziertes item: #%d\n",item);
            printf("-------------------------\n");

            sem_post(&cFull);
        }  
    }   
}
    
void consumer(void *ptr)
{
    while(1)
    {
        sleep(2);
        printf("\n Konsumer wartet auf Eintritt in kritischen Bereich\n");
        //Mutex sperren
        sem_wait(&cFull);
        printf("\n Konsumer ist in kritischen Bereich eingetretten\n");
        int item;
        printf("\n Konsum gestartet\n");
        removeItem(&item);
        printf("\n Konsum beendet\n");
        printf("\n konsumiertes item: #%d\n",item);
        printf("-------------------------\n"); 

        sem_post(&cEmpty);
    } 
}

void insertItem(int item){
    if (counter < lagerGrosse){  //Im Lager befindet sich Platz für weiter Items
        itemBuffer[counter] = item;
        counter ++;
        printf("\n Item zum Lager hinzugefügt:  %d items im Lager\n",counter);
    }else{
        printf("\n Lager ist Voll\n");
    }
}

void removeItem(int *item){
    if (counter > 0){
        *item = itemBuffer[counter-1];
        counter --;
        printf("\n Item konsumiert:  %d items im Lager\n", counter);
    }else{
        printf("\n Lager ist leer\n");
        *item = 0;
    }
}

int main(void)
{
    int i[2];
    pthread_t thread_1;
    pthread_t thread_2;
    int err;

    sem_init(&cFull, 0, 0);
    sem_init(&cEmpty, 0, 10);
    
    err = pthread_create(&thread_1, NULL, (void *) &producer, (void *) &i[0]);
    if (err != 0)
    printf("\ncan't create thread :[%s]", strerror(err));
        
         
    err = pthread_create(&thread_2, NULL, (void *) &consumer, (void *) &i[1]);
    if (err != 0)
    printf("\ncan't create thread :[%s]", strerror(err));

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    return 0;
}