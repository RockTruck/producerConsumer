#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t tid[2];	// Array für Thread id's
pthread_mutex_t lock;	// Mutex Variable
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
            pthread_mutex_lock(&lock);
            int item = rand()+1;
            printf("\n Produktion gestartet\n");
            insertItem(item);
            printf("\n Produktion beendet\n");
            printf("\n produziertes item: #%d\n",item);
            printf("-------------------------\n");

            pthread_mutex_unlock(&lock);
        }  
    }   
}
    
void consumer(void *ptr)
{
    while(1)
    {
        sleep(2);
        //Mutex sperren
        pthread_mutex_lock(&lock);
        int item;
        printf("\n Konsum gestartet\n");
        removeItem(&item);
        printf("\n Konsum beendet\n");
        printf("\n konsumiertes item: #%d\n",item);
        printf("-------------------------\n"); 

        pthread_mutex_unlock(&lock); 
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

    pthread_mutex_init(&lock, NULL);// Mutex initialisieren
    
    err = pthread_create(&thread_1, NULL, (void *) &producer, (void *) &i[0]);
    if (err != 0)
    printf("\ncan't create thread :[%s]", strerror(err));
        
         
    err = pthread_create(&thread_2, NULL, (void *) &consumer, (void *) &i[1]);
    if (err != 0)
    printf("\ncan't create thread :[%s]", strerror(err));

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}