#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t lock;	// Mutex Variable
int counter = 0;        // Produktanzahl
int lagerGrosse = 10;   // Lagergröße
int itemBuffer[10];     // Array für Lager

//Prototypen
void producer(void *ptr);
void consumer(void *ptr);
void insertItem(int item);
void removeItem(int *item); 

//Producer Unterprogramm
void producer(void *tid)   
{
    while(1)   
    {
        int event;  //Variable für Eingabe
        scanf("%d",&event);     //Einlesen des Events
        if(event == 1)  //Ausführen wenn Event vorhanden
        {
            pthread_mutex_lock(&lock);  //Mutex sperren
            int item = rand()+1;    //Random Wert (+1 damit nicht 0 vorkommt)
            printf("\n Produktion gestartet\n");
            insertItem(item);   //Aufruf von insertItem
            printf("\n Produktion beendet\n");
            printf("\n produziertes item: #%d\n",item); //Produziertes Item anzeigen
            printf("-------------------------\n");

            pthread_mutex_unlock(&lock);    //Mutex freigeben
        }  
    }   
}

//Consumer Unterprogramm  
void consumer(void *tid)
{
    while(1)
    {
        sleep(2);   //Alle 2 Sekunden cosumer ausführen
        pthread_mutex_lock(&lock);  //Mutex sperren
        int item;
        printf("\n Konsum gestartet\n");
        removeItem(&item);  //Aufruf von removeItem
        printf("\n Konsum beendet\n");
        printf("\n konsumiertes item: #%d\n",item); //konsumiertes Item anzeigen
        printf("-------------------------\n"); 

        pthread_mutex_unlock(&lock);    //Mutex freigeben
    } 
}

//insertItem Unterprogramm
void insertItem(int item){
    if (counter < lagerGrosse){  //Im Lager befindet sich Platz für weiter Items
        itemBuffer[counter] = item; //Item  in Lager(Array) platzieren
        counter ++; //Anzahl der vorhandenen Items inkrementieren
        printf("\n Item zum Lager hinzugefügt:  %d items im Lager\n",counter); //Azahl der Items im Lager anzeigen
    }else{
        printf("\n Lager ist Voll\n");
    }
}

//removeItem Unterprogramm
void removeItem(int *item){
    if (counter > 0){   //Im Lager ist mindestens ein Item vorhanden
        *item = itemBuffer[counter-1];  //Item aus Lager(Array) entfernen
        counter --; //Anzahl der vorhanden Items dekrementieren
        printf("\n Item konsumiert:  %d items im Lager\n", counter);    //Anzahl der Items im Lager anzeigen
    }else{
        printf("\n Lager ist leer\n");
        *item = 0;  //Itemwert auf 0 setzen 
    }
}

//Hauptprogramm
int main(void)
{
    int id[2];  //Array für thread id's
    pthread_t thread_1; //thread 1
    pthread_t thread_2; //thread 2
    int err;    //err Variable für error handling 

    pthread_mutex_init(&lock, NULL);// Mutex initialisieren
    
    //Producer thread erstellen
    err = pthread_create(&thread_1, NULL, (void *) &producer, (void *) &id[0]);
    if (err != 0)
    printf("\ncan't create thread :[%s]", strerror(err));
        
    //Consumer thread erstellen     
    err = pthread_create(&thread_2, NULL, (void *) &consumer, (void *) &id[1]);
    if (err != 0)
    printf("\ncan't create thread :[%s]", strerror(err));

    // Wartet auf terminierung der threads
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    pthread_mutex_destroy(&lock);   //Mutex auflösen
    return 0;
}