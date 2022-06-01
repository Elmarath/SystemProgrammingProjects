#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

struct Chef
{
  //0 = S, 1 = U, 2 = F, 3 = G
  int infiniteIngredients[4];
  int currentIngredients[4];
  int chefNo;
};

struct Deliverer{
  int currentIngredients[4];
  int isFree;
};
struct Chef chefs[6];
struct Deliverer deliverer;
pthread_mutex_t lock;

void Log(const char output[]);
void delay(int number_of_seconds);
void InitializeChefs(struct Chef chefs[6]);
void InitializeDeliverer(struct Deliverer deliverer);
void PrintChefs(struct Chef chefs[6]);
void* MakeBaklava(void* arg);
void* Deliver(void* arg);

int main()
{
  pthread_t chefThreads[6];
  InitializeChefs(chefs);
  InitializeDeliverer(deliverer);
  if(pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("\n mutex init failed\n");
    return 1;
  }

  // start the threads for the chefs
  for (int i = 0; i < 6; i++)
  {
    int chefIndex = i;
    pthread_create(&chefThreads[i], NULL, &MakeBaklava, &chefIndex);
    pthread_join(chefThreads[i], NULL);
  }
  // start the thread for the deliverer
  pthread_t delivererThread;
  pthread_create(&delivererThread, NULL, &MakeBaklava, NULL);
  pthread_join(delivererThread, NULL);  


  PrintChefs(chefs);
  return 0;
}

void *MakeBaklava(void* arg) {
  while (1)
  {
    int chefIndex = *(int*)arg;
    int lackOfIngredientsIndex[4] = {-1};
    // check if all the ingredients are exist
    int isReady = 1;
    for (int i = 0; i < 4; i++)
    {
      if(chefs[chefIndex].currentIngredients[i] == 0){
        isReady = 0;
        lackOfIngredientsIndex[i] = 1;
      }
    }
    if(isReady)
    {
      pthread_mutex_lock(&lock);
      // ready to make baklava
      //printf("Chef %d is making baklava!\n", chefIndex);
      pthread_mutex_unlock(&lock);
    } 
    else{
      // not ready to make baklava
      printf("Not ready to make baklava!\n");
      for(int i = 0; i < 4; i++)
      {
        if(lackOfIngredientsIndex[i] == 1)
        {
          printf("Chef %d Waiting for ingredient %d\n", chefIndex, i);
        }
      }
    }
  }
}

void* Deliver(void* arg)
{
  int currentVisitedChef = 0;
  delay(1);
  while (1)
  {
    
    // take the ingredients from the chefs's infinite ingredients
    for(int i = 0; i < 4; i++)
    {
      if(chefs[currentVisitedChef].infiniteIngredients[i] > 0)
      {
        deliverer.currentIngredients[i] = chefs[currentVisitedChef].infiniteIngredients[i];
      }
    }
    pthread_mutex_lock(&lock);
    // give the ingredients to the chef's current ingredients
    for(int i = 0; i < 4; i++)
    {
      if(deliverer.currentIngredients[i] > chefs[currentVisitedChef].currentIngredients[i])
      {
        chefs[currentVisitedChef].currentIngredients[i] = deliverer.currentIngredients[i];
      }
    }
    pthread_mutex_unlock(&lock);

    currentVisitedChef++;

    if(currentVisitedChef == 6)
    {
      currentVisitedChef = 0;
    }
  }
}

void InitializeDeliverer(struct Deliverer deliverer){
    deliverer.isFree = 1;  
    for(int j = 0; j < 4; j++)
    {
      deliverer.currentIngredients[j] = 0;
    }
}


void InitializeChefs(struct Chef chefs[6])
{
  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      chefs[i].infiniteIngredients[j] = 0;
    }
  }
  
  chefs[0].infiniteIngredients[0] = 1;
  chefs[0].infiniteIngredients[1] = 1;
  chefs[1].infiniteIngredients[0] = 1;
  chefs[1].infiniteIngredients[2] = 1;
  chefs[2].infiniteIngredients[0] = 1;
  chefs[2].infiniteIngredients[3] = 1;
  chefs[3].infiniteIngredients[1] = 1;
  chefs[3].infiniteIngredients[2] = 1;
  chefs[4].infiniteIngredients[1] = 1;
  chefs[4].infiniteIngredients[3] = 1;
  chefs[5].infiniteIngredients[2] = 1;
  chefs[5].infiniteIngredients[3] = 1;  

  for (int i = 0; i < 6; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      chefs[i].currentIngredients[j] = chefs[i].infiniteIngredients[j];
    }
    chefs[i].chefNo = i;
  }
  
}

void Log(const char output[])
{
  int i = 0;
  while (output[i] != '\0')
  {
    printf("%c", output[i]);
    i++;
  }
  printf("\n");
}

void PrintChefs(struct Chef chefs[6]){
  for (int i = 0; i < 6; i++)
  {
    printf("chef[%d]: \n", i);
    for (int j = 0; j < 4; j++)
    {
      printf("infiniteIngredients[%d]: %d \n", i, chefs[i].infiniteIngredients[j]);
      printf("currentIngredients[%d]:  %d \n", i, chefs[i].currentIngredients[j]);
    }
  }
}

void delay(int number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;
  
    // Storing start time
    clock_t start_time = clock();
  
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}
 