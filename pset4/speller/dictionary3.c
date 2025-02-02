#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"

/**
 * Returns true if word is in dictionary else false.
 */

int hashvalue(const char* a);

int word_count=0;
char word[LENGTH+1];
typedef struct node
{
    char* word;
    struct node* next;
}
node;

//each element in htable will contain an address to a node
node* htable[HT_SIZE];

bool check(const char* word)
{
    int hv = hashvalue(word);
    node* ptr = malloc(sizeof(node));
    ptr = htable[hv]->next;
    while(ptr!=NULL)
    {
        //if ( (int)toupper(ptr->word[1])<(int)toupper(word[1]))
        //    return false;
        if (strcasecmp(ptr->word,word)==0)
        {
            return true;
        }

        else
        {
            ptr = ptr->next;
        }
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Open the given file location
    FILE* inptr = fopen(dictionary,"r");
    // create a keeper for the word which is a character array of length Large
    for (int i = 0; i < HT_SIZE; i++)
    {
        htable[i] = malloc(sizeof(node));
    }

    if (inptr == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        return false;
    }


    while (fscanf(inptr,"%s\n",word) != EOF)
    {

        node* newword = malloc(sizeof(node));
        newword->word = malloc(strlen(word)+1);
        strcpy(newword->word,word);
        newword->next = NULL;
        int hv = hashvalue(word);
        word_count++;
        if (htable[hv]==NULL)
        {
            htable[hv]->next = newword;
        }
        else
        {
            node* nodenext = malloc(sizeof(node));
            nodenext = htable[hv]->next;
            htable[hv]->next = newword;
            newword->next = nodenext;
        }
    }
    return true;

}

/**
 Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return word_count;
    return 0;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    node* pnode;
    node* ptr;
    for(int i=0;i<HT_SIZE;i++)
    {
       ptr = htable[i];
       while(ptr!=NULL)
        {
            pnode=ptr;
            ptr = ptr->next;
            free(pnode);
        }
    }
    free(ptr);
    return true;
}


int hashvalue(const char* a)
{
    int x =  a[0];
    int y =  a[1];
    int hash = 0 ;

    if (x > 64 && x < 91 )
    {
        x = x + 32;
    }
    if (y > 64 && y < 91 )
    {
        y =y+32;

    }
    hash = (x*y)%(HT_SIZE);
    return hash;
}


#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dictionary.h"


// size of hashtable
#define SIZE 1000000

// create nodes for linked list
typedef struct node
{
    char word[LENGTH+1];
    struct node* next;
}
node;

// create hashtable
node* hashtable[SIZE] = {NULL};

// create hash function
int hash (const char* word)
{
    int hash = 0;
    int n;
    for (int i = 0; word[i] != '\0'; i++)
    {
        // alphabet case
        if(isalpha(word[i]))
            n = word [i] - 'a' + 1;

        // comma case
        else
            n = 27;

        hash = ((hash << 3) + n) % SIZE;
    }
    return hash;
}

// create global variable to count size
int dictionarySize = 0;

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // TODO
    // opens dictionary
    FILE* file = fopen(dictionary, "r");
    if (file == NULL)
        return false;

    // create an array for word to be stored in
    char word[LENGTH+1];

    // scan through the file, loading each word into the hash table
    while (fscanf(file, "%s\n", word)!= EOF)
    {
        // increment dictionary size
        dictionarySize++;

        // allocate memory for new word
        node* newWord = malloc(sizeof(node));

        // put word in the new node
        strcpy(newWord->word, word);

        // find what index of the array the word should go in
        int index = hash(word);

        // if hashtable is empty at index, insert
        if (hashtable[index] == NULL)
        {
            hashtable[index] = newWord;
            newWord->next = NULL;
        }

        // if hashtable is not empty at index, append
        else
        {
            newWord->next = hashtable[index];
            hashtable[index] = newWord;
        }
    }

    // close file
    fclose(file);

    // return true if successful
    return true;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // TODO
    // creates a temp variable that stores a lower-cased version of the word
    char temp[LENGTH + 1];
    int len = strlen(word);
    for(int i = 0; i < len; i++)
        temp[i] = tolower(word[i]);
    temp[len] = '\0';

    // find what index of the array the word should be in
    int index = hash(temp);

    // if hashtable is empty at index, return false
    if (hashtable[index] == NULL)
    {
        return false;
    }

    // create cursor to compare to word
    node* cursor = hashtable[index];

    // if hashtable is not empty at index, iterate through words and compare
    while (cursor != NULL)
    {
        if (strcmp(temp, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    // if you don't find the word, return false
    return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    // if dictionary is loaded, return number of words
    if (dictionarySize > 0)
    {
        return dictionarySize;
    }

    // if dictionary hasn't been loaded, return 0
    else
        return 0;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    // create a variable to go through index
    int index = 0;

    // iterate through entire hashtable array
    while (index < SIZE)
    {
        // if hashtable is empty at index, go to next index
        if (hashtable[index] == NULL)
        {
            index++;
        }

        // if hashtable is not empty, iterate through nodes and start freeing
        else
        {
            while(hashtable[index] != NULL)
            {
                node* cursor = hashtable[index];
                hashtable[index] = cursor->next;
                free(cursor);
            }

            // once hashtable is empty at index, go to next index
            index++;
        }
    }

    // return true if successful
    return true;
}
//hash function
 int hash = 0;
    int n;
    for (int i = 0; word[i] != '\0'; i++)
    {
        // alphabet case
        if(isalpha(word[i]))
            n = word [i] - 'a' + 1;

        // comma case
        else
            n = 27;

        hash = ((hash << 3) + n) % N;
    }
    return hash;