#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct list_t{
    char* key;
    char* value;
    struct list_t *next;

} list_t;

void initLinked(list_t *ll);
char set(list_t *ll, char *key, char *value);
char get(list_t *ll, char *key);
char* del(list_t *ll, char *key);
void displayLinked(list_t *ptr);

void initLinked(list_t *ll){
  ll->key = NULL;
  ll->value = NULL;
  ll->next = NULL;
}

char set(list_t *ll, char *key, char *value){ //insert key & value or changes keys value
  char *tmpKey = malloc(strlen(key)+1);
  strcpy(tmpKey, key);
  char *tmpValue = malloc(strlen(value)+1);
  strcpy(tmpValue, value);
  char oks[4] = "OKS";
  int isHead = 1; //head boolean

  while (ll->key!=NULL) {
    if (strcmp(ll->key, key)==0) { //set value if key is found
      ll->value = tmpValue;
      return oks;
    }
    if (strcmp(ll->key, key)>0) { //insert alphabetically
      //create new node
      list_t *new = malloc(sizeof(list_t));
      initLinked(new);
      if (isHead==0) { //if not the head
        new->key=tmpKey;
        new->value=tmpValue;
        new->next = ll->next;
        ll->next = new;
      }
      else{ //insert at front
        new->key=ll->key;
        new->value=ll->value;
        new->next = ll->next;
        ll->key=tmpKey;
        ll->value=tmpValue;
        ll->next = new;
      }
      return oks;
    }
    isHead = 0;
  }

  //list was empty (add info to list head)
  ll->key = tmpKey;
  ll->value = tmpValue;
  return oks;
}
