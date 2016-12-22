#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>

sem_t cEmpty;   //semaphore für producer
sem_t cFull;    //semaphore für consumer
int counter = 0;        // Produktanzahl
int lagerGrosse = 10;   //lagergröße
int itemBuffer[10]; //Array für Lager


//Prototypen
void producer(void *ptr);
void consumer(void *ptr);
void insertItem(int item);
void removeItem(int *item); 

//unterprogramm producer
void producer(void *ptr)   
{
    while(1)
    {
        int event;  //variable für eingabe
        scanf("%d",&event); //einlesen des events
        if(event == 1)  //ausführen wenn event vorhanden
        {
            sem_wait(&cEmpty);  //semaphore cEmpty sperren wenn nicht 0 (bei eintritt wir cEmpty decrementiert)
            int item = rand()+1;    //Random Wert (+1 damit nicht 0 vorkommt)
            printf("\n Produktion gestartet\n");
            insertItem(item);   //insertItem aufruf
            printf("\n Produktion beendet\n");
            printf("\n produziertes item: #%d\n",item); //produziertes item anzeigen
            printf("-------------------------\n");

            sem_post(&cFull);   //cFull inkrementieren
        }  
    }   
}

//unterprogramm consumer    
void consumer(void *ptr)
{
    while(1)
    {
        sleep(2);   //alle 2 sekunden consumer ausführen
        printf("\n Konsumer wartet auf Eintritt in kritischen Bereich\n");
        sem_wait(&cFull);   ////semaphore cFull sperren wenn nicht 0 (bei eintritt wir cFull decrementiert)
        printf("\n Konsumer ist in kritischen Bereich eingetretten\n");
        int item;
        printf("\n Konsum gestartet\n");
        removeItem(&item);  //removeItem aufruf
        printf("\n Konsum beendet\n");
        printf("\n konsumiertes item: #%d\n",item); //konsumiertes item anzeigen
        printf("-------------------------\n"); 

        sem_post(&cEmpty);  //cEmpty inkrementieren
    } 
}

//unterprogramm insertItem
void insertItem(int item){
    if (counter < lagerGrosse){  //Im Lager befindet sich Platz für weiter Items
        itemBuffer[counter] = item; //item im lager(array) platzieren
        counter ++; //anzahl der vorhanden items inkrementieren
        printf("\n Item zum Lager hinzugefügt:  %d items im Lager\n",counter);  //anzahl der items im lager anzeigen
    }else{
        printf("\n Lager ist Voll\n");
    }
}

//unterprogramm removeItem
void removeItem(int *item){
    if (counter > 0){   //im lager bedindet sich mindestens ein item
        *item = itemBuffer[counter-1]; //item aus lager(array) entfernen
        counter --; //anzahl der vorhanden items dekrementieren
        printf("\n Item konsumiert:  %d items im Lager\n", counter);    //anzahl der items im lager anzeigen
    }else{
        printf("\n Lager ist leer\n");
        *item = 0;  //itemwert auf 0 setzen
    }
}

//hauptprogramm
int main(void)
{
    int i[2];   //array für thread id's
    pthread_t thread_1; //thread 1
    pthread_t thread_2; //thread 2
    int err;    //err Variable für error handling 

    sem_init(&cFull, 0, 0); //semaphore cFull initialisieren
    sem_init(&cEmpty, 0, 10);   //semaphore cEmpty initialisieren
    
    //producer thread erstellen
    err = pthread_create(&thread_1, NULL, (void *) &producer, (void *) &i[0]);
    if (err != 0)
    printf("\ncan't create thread :[%s]", strerror(err));
        
    //consumer thread erstellen     
    err = pthread_create(&thread_2, NULL, (void *) &consumer, (void *) &i[1]);
    if (err != 0)
    printf("\ncan't create thread :[%s]", strerror(err));

    //wartet auf terminierung der threads
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    return 0;
}