#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *ReadFromFile(char *fileName);
int CharacterCounter(char *arrayName);
int WordCounter(char *arrayName);
int LineCounter(char *arrayName);
void CreateRandomTextFile();
void HandleInput(int argc, char *argv[]);
void HandleOutput(char *fileName, int isLineCountWanted, int isWordCountWanted, int isCharCountWanted);

int main(int argc, char *argv[])
{
  HandleInput(argc, argv);
  return 0;
}

/**
 * @brief Counts the lines by counting '/n' sign
 * whith given array
 *
 * @param arrayName will be counted
 * @return int how much lines counted
 */

int LineCounter(char *arrayName)
{
  int lineCount = 0;
  int i = 0;

  while (1)
  {
    if (arrayName[i] == '\n')
    {
      lineCount++;
    }

    i++;
    if (arrayName[i] == '\0')
    {
      lineCount++;
      break;
    }
  }

  return lineCount;
}

/**
 * @brief counts the total words of given array
 *
 * @param arrayName will be counted
 * @return int how much lines counted
 */

int WordCounter(char *arrayName)
{
  int totalWordCount = 0;
  int isPreCharSpace = 1;
  int i = 0;

  // if counter space or new line or tab increase unless previous char is one of them
  while (1)
  {
    if (arrayName[i] == ' ' || arrayName[i] == '\n' || arrayName[i] == '\t')
    {
      if (isPreCharSpace == 0)
      {
        totalWordCount++;
      }
      isPreCharSpace = 1;
    }
    else
    {
      isPreCharSpace = 0;
    }

    if (arrayName[i] == '\0')
    {
      break;
    }
    i++;
  }
  return totalWordCount;
}

/**
 * @brief counts the total charactes in given array
 *
 * @param arrayName will be counted
 * @return int how mucj characters counted
 */

int CharacterCounter(char *arrayName)
{
  int sizeofArray;
  int i = 0;

  // increase with every character
  while (1)
  {
    i++;
    if (arrayName[i] == '\0')
    {
      sizeofArray = i;
      break;
    }
  }

  return sizeofArray;
}

/**
 * @brief Reads from a file and returns the file in char array form
 *
 * @param fileName going to be read
 * @return char* will the returned array with file's value
 */

char *ReadFromFile(char *fileName)
{

  char *readString;

  // open the file
  FILE *ptr;
  char ch;

  // Opening file in reading mode
  ptr = fopen(fileName, "r");

  if (NULL == ptr)
  {
    printf("file can't be opened \n");
    exit(0);
  }

  // Printing what is written in file
  // character by character using loop.
  int sizeofFile = 0;
  do
  {
    ch = fgetc(ptr);
    sizeofFile++;
    // Checking if character is not EOF.
    // If it is EOF stop eading.
  } while (ch != EOF);
  fclose(ptr);

  // reopening file in reading mode
  ptr = fopen(fileName, "r");

  if (NULL == ptr)
  {
    printf("file can't be opened \n");
  }

  readString = (char *)malloc(sizeofFile * sizeof(char));

  int i = 0;
  do
  {
    ch = fgetc(ptr);
    if (ch == EOF)
    {
      break;
    }
    readString[i] = ch;
    i++;
  } while (1);

  fclose(ptr);

  return readString;
}

/**
 * @brief Create a Random Text File object and names the file
 * with 'Random.txt'
 *
 */

void CreateRandomTextFile()
{

  FILE *ptr;
  char fileName[] = "Random.txt";
  char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
  char punctions[] = {'.', '?', '!'};

  ptr = fopen(fileName, "w");
  fclose(ptr);

  // creating a line count between 7.000-25.000
  srand(time(NULL));
  int sentenceCount = (rand() % 18000) + 7000;

  const int maxLineCounter = 60; // after 50 char go new line when counter space
  int curLineCounter = 0;

  int maxParagraphSentence = 50 - (rand() % 30);
  int curParagraphSentence = 0;

  char sentence[150];
  int sentenceIndex = 0;

  int i = 0, j = 0, k = 0;
  // creating the whole text
  for (i = 0; i < sentenceCount; i++)
  {
    // creating every sentence
    int wordCount = rand() % 12 + 1; // max length of a sentence is 12
    for (j = 0; j < wordCount; j++)
    {
      // creating every word
      int charCount = (rand() % 10) + 1;
      for (k = 0; k < charCount; k++)
      {
        int randCharIndex = rand() % 26;
        sentence[sentenceIndex] = alphabet[randCharIndex];
        sentenceIndex++;
        curLineCounter++;
        if (k == charCount - 1 && !(j == (wordCount - 1)))
        {
          sentence[sentenceIndex] = ' ';
          sentenceIndex++;
        }
      }

      if (j == (wordCount - 1))
      {
        sentence[sentenceIndex] = '.';
        sentenceIndex++;
        sentence[sentenceIndex] = ' ';
        sentenceIndex++;
      }

      if (curLineCounter > maxLineCounter)
      { // and current char is space
        sentence[sentenceIndex] = '\n';
        sentenceIndex++;
        curLineCounter = 0;
      }
    }
    curParagraphSentence++;
    if (curParagraphSentence == maxParagraphSentence)
    {
      sentence[sentenceIndex] = '\n';
      sentenceIndex++;
      sentence[sentenceIndex] = '\t';
      sentenceIndex++;
      curParagraphSentence = 0;
      maxParagraphSentence = 50 - (rand() % 30);
    }

    ptr = fopen(fileName, "a");

    for (int i = 0; i < sentenceIndex; i++)
    {
      fputc(sentence[i], ptr);
    }

    fclose(ptr);

    sentenceIndex = 0;
    memset(sentence, 0, sizeof(sentence));
  }

  fclose(ptr);
}

/**
 * @brief with which input combinations, and which arguments exist
 * orders the inputs in meaningfull sense.
 *
 * @param argc how many arguments is given
 * @param argv which arguments is given
 */

void HandleInput(int argc, char *argv[])
{
  // if agrument count is greater than 4 exit
  if (argc > 5)
  {
    printf("Used too much arguments. Exiting program.\n");
    exit(0);
  }

  // if 1 argument given -> use stdin
  if (argc == 1)
  {
    // use stdin (d_dickens)
    char *fileName = "c_dickens.txt";
    char *readFile = ReadFromFile(fileName);
    int lineCount = LineCounter(readFile);
    int wordCount = WordCounter(readFile);
    int charCount = CharacterCounter(readFile);
    printf("%s\t%d\t%d\t%d\n", "-stdin-", lineCount, wordCount, charCount);
    exit(1);
  }
  if (argc == 2)
  {
    // check if argument is random.txt
    if ((strcmp("random.txt", argv[1])) == 0)
    {
      CreateRandomTextFile();
      char *fileName = "random.txt";
      char *readFile = ReadFromFile(fileName);
      int lineCount = LineCounter(readFile);
      int wordCount = WordCounter(readFile);
      int charCount = CharacterCounter(readFile);
      printf("%s\t%d\t%d\t%d\n", fileName, lineCount, wordCount, charCount);
      exit(1);
    }
    else
    {
      // only file name
      char *readFile = ReadFromFile(argv[1]);
      int lineCount = LineCounter(readFile);
      int wordCount = WordCounter(readFile);
      int charCount = CharacterCounter(readFile);
      printf("%s\t%d\t%d\t%d\n", argv[1], lineCount, wordCount, charCount);
      exit(1);
    }
  }

  if (argc > 1)
  {
    // check if a txt file has given
    char *tmp = argv[1];
    char *ret;
    ret = strstr(tmp, ".txt");

    if (!ret)
    {
      printf("No .txt file found\n");
      exit(0);
    }
  }

  // gether every desired functions
  int isLineCountWanted = 0;
  int isWordCountWanted = 0;
  int isCharCountWanted = 0;

  int totalWanted = 0;

  if (argc == 3)
  {
    if (((strcmp("-l", argv[1])) == 0) || ((strcmp("-l", argv[2])) == 0))
    {
      isLineCountWanted = 1;
      totalWanted++;
    }
    if (((strcmp("-w", argv[1])) == 0) || ((strcmp("-w", argv[2])) == 0))
    {
      isWordCountWanted = 1;
      totalWanted++;
    }
    if (((strcmp("-c", argv[1])) == 0) || ((strcmp("-c", argv[2])) == 0))
    {
      isCharCountWanted = 1;
      totalWanted++;
    }
    if (totalWanted != 1)
    {
      printf("You entered invalid input! Exiting Program\n");
      exit(2);
    }
  }

  if (argc == 4)
  {
    if (((strcmp("-l", argv[1])) == 0) || ((strcmp("-l", argv[2])) == 0) || ((strcmp("-l", argv[3])) == 0))
    {
      isLineCountWanted = 1;
      totalWanted++;
    }
    if (((strcmp("-w", argv[1])) == 0) || ((strcmp("-w", argv[2])) == 0) || ((strcmp("-w", argv[3])) == 0))
    {
      isWordCountWanted = 1;
      totalWanted++;
    }
    if (((strcmp("-c", argv[1])) == 0) || ((strcmp("-c", argv[2])) == 0) || ((strcmp("-c", argv[3])) == 0))
    {
      isCharCountWanted = 1;
      totalWanted++;
    }

    if (totalWanted != 2)
    {
      printf("You entered invalid input! Exiting Program\n");
      exit(2);
    }
  }

  if (argc == 5)
  {
    if (((strcmp("-l", argv[1])) == 0) || ((strcmp("-l", argv[2])) == 0) || ((strcmp("-l", argv[3])) == 0) || ((strcmp("-l", argv[4])) == 0))
    {
      isLineCountWanted = 1;
      totalWanted++;
    }
    if (((strcmp("-w", argv[1])) == 0) || ((strcmp("-w", argv[2])) == 0) || ((strcmp("-w", argv[3])) == 0) || ((strcmp("-w", argv[4])) == 0))
    {
      isWordCountWanted = 1;
      totalWanted++;
    }
    if (((strcmp("-c", argv[1])) == 0) || ((strcmp("-c", argv[2])) == 0) || ((strcmp("-c", argv[3])) == 0) || ((strcmp("-c", argv[4])) == 0))
    {
      isCharCountWanted = 1;
      totalWanted++;
    }
    if (totalWanted != 3)
    {
      printf("You entered invalid input! Exiting Program\n");
      exit(2);
    }
  }

  HandleOutput(argv[1], isLineCountWanted, isWordCountWanted, isCharCountWanted);
}

/**
 * @brief writes down the files by eliminating the undesired ones
 *
 * @param fileName name of the file we are going to count
 * @param isLineCountWanted
 * @param isWordCountWanted
 * @param isCharCountWanted
 */

void HandleOutput(char *fileName, int isLineCountWanted, int isWordCountWanted, int isCharCountWanted)
{
  printf("%s\t", fileName);
  char *readFile = ReadFromFile(fileName);

  if (isLineCountWanted == 1)
  {
    int lineCount = LineCounter(readFile);
    printf("%d\t", lineCount);
  }

  if (isWordCountWanted == 1)
  {
    int wordCount = WordCounter(readFile);
    printf("%d\t", wordCount);
  }

  if (isCharCountWanted == 1)
  {
    int charCount = CharacterCounter(readFile);
    printf("%d\t", charCount);
  }

  printf("\n");
}
