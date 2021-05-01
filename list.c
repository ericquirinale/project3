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
char* get(list_t *ll, char *key);
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
  char oks[4] = "OKS\n";
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
    ll=ll->next;
  }

  //list was empty (add info to list head)
  ll->key = tmpKey;
  ll->value = tmpValue;
  return oks;
}

char *get(list_t *ll, char *key){
  char okg[4] = "OKG\n";
  char knf[4] = "KNF\n";
  char *retValue = malloc(len+15);
  strcat(retValue, okg);

  while (ll->key!=NULL) {
    if (strcmp(ll->key, key)==0) { //if key found
      //create return string
      int len = strlen(ll->value);
      strcat(retValue, itoa(len));
      strcat(retValue, "\n");
      strcat(retValue, ll->value);
      strcat(retValue, "\n");
      return retValue;
    }
    if (strcmp(ll->key, key)>0) { //not found
      return knf;
    }
    ll=ll->next;
  }

  //if head was null
  return knf;
}

char* del(list_t *ll, char *key){
  char okd[4] = "OKD\n";
  char knf[4] = "KNF\n";
  char *retValue = malloc(len+15);
  strcat(retValue, okg);

  list_t *prev = NULL;

  while (ll->key!=NULL) {
    if (strcmp(ll->key, key)==0) { //if key found
      //create return string
      int len = strlen(ll->value);
      strcat(retValue, itoa(len));
      strcat(retValue, "\n");
      strcat(retValue, ll->value);
      strcat(retValue, "\n");

      //delete key
      if (prev!=NULL) { //not head
        prev->next = ll->next;
      }
      else{ //delete from head
        if (ll->next->key!=NULL) {//if more than one key in list
          ll->key = ll->next->key;
          ll->value = ll->next->value;
          ll->next = ll->next->next;
        }
        else{ //if only key, set all values to null
          initLinked(ll);
        }
      }
      return retValue;
    }
    if (strcmp(ll->key, key)>0) { //not found
      return knf;
    }
    prev = ll;
    ll=ll->next;
  }

  //if head was null
  return knf;
}

void displayLinked(list_t *ptr){
  for (; ptr != NULL; ptr = ptr->next) {
        printf("%s%s\t%s%s\n", "Key: ", ptr->key, "Value: ", ptr->value);
    }
}
