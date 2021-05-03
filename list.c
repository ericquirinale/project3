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
char* set(list_t *ll, char *key, char *value);
char* get(list_t *ll, char *key);
char* del(list_t *ll, char *key);
void displayLinked(list_t *ptr);
char* itoa(int value, char* result, int base);
int countDigit(long long n);

void initLinked(list_t *ll){
  ll->key = NULL;
  ll->value = NULL;
  ll->next = NULL;
}

char* set(list_t *ll, char *key, char *value){ //insert key & value or changes keys value
  /*char *tmpKey = malloc(sizeof(char)*strlen(key)+1);
  strcpy(tmpKey, key);
  char *tmpValue = malloc(sizeof(char)*strlen(value)+1);
  strcpy(tmpValue, value);*/
  char tmp[4] = "OKS";
  char *oks = tmp;
  int isHead = 1; //head boolean

  while (ll->key!=NULL) {
    if (strcmp(ll->key, key)==0) { //set value if key is found
      ll->value = value;
      return oks;
    }
    else if(strcmp(ll->key, key)>0) { //insert alphabetically
      //create new node
      list_t *new = malloc(sizeof(list_t));
      initLinked(new);
      if (isHead==0) { //if not the head
        //new->key=tmpKey;
        //new->value=tmpValue;
        new->key=key;
        new->value=value;
        new->next = ll->next;
        ll->next = new;
      }
      else { //insert at front
        new->key=ll->key;
        new->value=ll->value;
        new->next = ll->next;
        ll->key=key;
        ll->value=value;
        ll->next = new;

      }
      return oks;
    }
    else if (ll->next==NULL) {//insert at the end
      list_t *new = malloc(sizeof(list_t));
      initLinked(new);
      new->key=key;
      new->value=value;
      new->next = ll->next;
      ll->next = new;
    }
    isHead = 0;
    ll=ll->next;
  }

  //list was empty (add info to list head)
  ll->key = key;
  ll->value = value;
  return oks;
}

char *get(list_t *ll, char *key){
  char okg[4] = "OKG";
  char knf[4] = "KNF";
  char *okgP = okg;
  char *knfP = knf;

  char *retValue = malloc(strlen(key)+15);

  while (ll->key!=NULL) {
    if (strcmp(ll->key, key)==0) { //if key found
      //create return string
      int len = strlen(ll->value);
      char slen[5];
      itoa(len, slen, 10);
      strcat(retValue, okgP);
      strcat(retValue, "\n");
      strcat(retValue, slen);
      strcat(retValue, "\n");
      strcat(retValue, ll->value);
      return retValue;
    }
    if (strcmp(ll->key, key)>0) { //not found
      return knfP;
    }
    ll=ll->next;
  }

  //if head was null
  return knfP;
}

char* del(list_t *ll, char *key){
  char tmp[4] = "OKD";
  char tmp2[4] = "KNF";
  char *okd = tmp;
  char *knf = tmp2;

  char *retValue = malloc(strlen(key)+15);

  list_t *prev = NULL;

  while (ll->key!=NULL) {
    if (strcmp(ll->key, key)==0) { //if key found
      //create return string
      int len = strlen(ll->value);
      char slen[5];
      itoa(len, slen, 10);
      strcat(retValue, okd);
      strcat(retValue, "\n");
      strcat(retValue, slen);
      strcat(retValue, "\n");
      strcat(retValue, ll->value);
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

/**
	 * C++ version 0.4 char* style "itoa":
	 * Written by Lukás Chmela
	 * Released under GPLv3.

	 */
char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

int countDigit(long long n)
{
    int count = 0;
    while (n != 0)
    {
        n = n / 10;
        ++count;
    }
    return count;
}
