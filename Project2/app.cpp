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
};
struct Chef chefs[6];
struct Deliverer deliverer;
pthread_mutex_t lock;

void InitializeChefs(struct Chef chefs[6]);
void InitializeDeliverer(struct Deliverer deliverer);
void PrintChefs(struct Chef chefs[6]);
void PrintDeliverer(struct Deliverer deliverer);
void PrintChef(struct Chef chef);
void WriteCurrentChefToFile(int chefNo);
void* CookBaklava(void* arg);
void* Deliver(void* arg);
struct Chef UpdateChefFromFile(int chefNo);

int main(){
  pthread_t chefsThreads[6];
  pthread_t delivererThread;
  if(pthread_mutex_init(&lock, NULL) != 0)
  {
    printf("\n mutex init failed\n");
    return 1;
  }
  InitializeChefs(chefs);
  InitializeDeliverer(deliverer);

  struct Chef returnedChef = UpdateChefFromFile(0);

  for (int i = 0; i < 6; i++)
  {
    WriteCurrentChefToFile(i);
  }

  pthread_create(&delivererThread, NULL, &Deliver, NULL);
  for(int i = 0; i < 6; i++){
    pthread_create(&chefsThreads[i], NULL, &CookBaklava, &chefs[i].chefNo);
  }
  pthread_join(delivererThread, NULL);
  for (int i = 0; i < 6; i++)
  {
    pthread_join(chefsThreads[i], NULL);
  }
  
  return 0;
}
/**
 * @brief Updates the chef's current ingredients from the file
 * 
 * @param chefNo 
 * @return Chef 
 */

Chef UpdateChefFromFile(int chefNo){
  int line = chefNo*2 + 1;
  int currentLine = 0;

  struct Chef returnedChef;
  for (int i = 0; i < 4; i++)
  {
    returnedChef.currentIngredients[i] = 0;
  }

  FILE* ptr;
  char ch;

  // Opening file in reading mode
  ptr = fopen("baklava.txt", "r");

  if (NULL == ptr) {
      printf("file can't be opened \n");
  }
  // Printing what is written in file
  // character by character using loop.
  do {
      ch = fgetc(ptr);
      if(ch == '\n'){
        currentLine++;
      }
      if(currentLine == line){
        if(ch == 'S'){
          returnedChef.currentIngredients[0] = 1;
        } else if(ch == 'U'){
          returnedChef.currentIngredients[1] = 1;
        } else if(ch == 'F'){
          returnedChef.currentIngredients[2] = 1;
        } else if(ch == 'G'){
          returnedChef.currentIngredients[3] = 1;
        }
      }

      // Checking if character is not EOF.
      // If it is EOF stop eading.
  } while (ch != EOF);
  // Closing the file
  fclose(ptr);

  return returnedChef;
}

/**
 * @brief writes the chef's current ingredients to the file
 * 
 * @param chefNo 
 */

void WriteCurrentChefToFile(int chefNo){
  FILE *fp;
   char buff[255];

  fp = fopen("baklava.txt", "a");
  if (fp == NULL)
  {
      printf("Error while appending file!\n");
      fp = fopen("baklava.txt", "w");
      if (fp == NULL)
      {
          printf("Error while creating file!\n");
          _exit(1);
      }
  }

  fprintf(fp, "ChefNo: %d\n", chefNo);
  for (int i = 0; i < 4; i++)
  {
    if(i == 0){
      fprintf(fp, "%c", chefs[chefNo].currentIngredients[i] ? 'S' : ' ');
    }
    else if(i == 1)
    {
      fprintf(fp, "%c", chefs[chefNo].currentIngredients[i] ? 'U' : ' ');
    }
    else if(i == 2)
    {
      fprintf(fp, "%c", chefs[chefNo].currentIngredients[i] ? 'F' : ' ');
    }
    else if(i == 3)
    {
      fprintf(fp, "%c", chefs[chefNo].currentIngredients[i] ? 'G' : ' ');
    }
    else{
      fprintf(fp, "\n");
    }
  }
  fprintf(fp, "\n");
  fclose(fp);
}

/**
 * @brief delivers the ingredients to the chefs
 * 
 * @param arg 
 * @return void* 
 */

void* Deliver(void* arg)
{
  int currentVisitedChef = 0;
  while (1)
  {
    sleep(1);
    // take the ingredients from the chefs's infinite ingredients
    pthread_mutex_lock(&lock);
    for(int i = 0; i < 4; i++)
    {
      if(chefs[currentVisitedChef].infiniteIngredients[i] > 0 && deliverer.currentIngredients[i] <= 0)
      {
        deliverer.currentIngredients[i] = chefs[currentVisitedChef].infiniteIngredients[i];
        printf("Deliverer took %d from Chef %d\n", i, currentVisitedChef);
        PrintChef(chefs[currentVisitedChef]);
        PrintDeliverer(deliverer);
      }
    }
    pthread_mutex_unlock(&lock);
    // give the ingredients to the chef's current ingredients
    pthread_mutex_lock(&lock);
    for(int i = 0; i < 4; i++)
    {
      if(deliverer.currentIngredients[i] > chefs[currentVisitedChef].currentIngredients[i])
      {
        chefs[currentVisitedChef].currentIngredients[i] = deliverer.currentIngredients[i];
        deliverer.currentIngredients[i] = 0;
        printf("Deliverer gave %d to Chef %d\n", i, currentVisitedChef);
        PrintDeliverer(deliverer);
        PrintChef(chefs[currentVisitedChef]);
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

/**
 * @brief Cooks baklava by using the chef's current ingredients
 * 
 * @param arg 
 * @return void* 
 */

void* CookBaklava(void* arg){
  int chefNo = *(int*)arg;

  while (1)
  {
    sleep(1);
    int chefNo = *(int*)arg;
    pthread_mutex_lock(&lock);
    int isReady = 1;
    for (int i = 0; i < 4; i++)
    {
      if(chefs[chefNo].currentIngredients[i] == 0){
        isReady = 0;
      }
    }
    pthread_mutex_unlock(&lock);
    pthread_mutex_lock(&lock);
    if(isReady == 1)
    {
      printf("Chef %d is ready to make baklava\n", chefNo);
      sleep(1);
      printf("Chef %d has cooked baklava\n", chefNo);
    }
    else
    {
      printf("Chef %d is not ready to make baklava\n", chefNo);
    }
    pthread_mutex_unlock(&lock);
  }
}

/**
 * @brief initializes the deliverer's current ingredients
 * 
 * @param deliverer to be initialized
 */

void InitializeDeliverer(struct Deliverer deliverer){ 
    for(int j = 0; j < 4; j++)
    {
      deliverer.currentIngredients[j] = 0;
    }
}

/**
 * @brief initializes all the chefs's current ingredients
 * 
 * @param chef to be initialized
 */

void PrintChefs(struct Chef chefs[6]){
  for (int i = 0; i < 6; i++)
  {
    printf("chef[%d]: \n", i);
    for (int j = 0; j < 4; j++)
    {
      printf("infiniteIngredients[%d]: %d \n", i, chefs[i].infiniteIngredients[j]);
    }
  }
}

/**
 * @brief prints the deliverer's current ingredients
 * 
 * @param deliverer to be printed
 */

void PrintDeliverer(struct Deliverer deliverer){
  for (int i = 0; i < 4; i++)
  {
    printf("deliverer[%d]: %d \n", i, deliverer.currentIngredients[i]);
  }
}

/**
 * @brief initializes the chefs
 * 
 * @param chefs that will be printed
 */

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

/**
 * @brief prints the specified chef's current ingredients
 * 
 * @param chef 
 */

void PrintChef(struct Chef chef){
  printf("chef[%d]: \n", chef.chefNo);
  for (int i = 0; i < 4; i++)
  {
    printf("currentIngredients[%d]: %d \n", i, chef.currentIngredients[i]);
  }
}