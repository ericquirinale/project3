#ifndef LIST_H_
#define LIST_H_

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

#endif
