#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    const struct person *person = get_person();
    printf("person size: %d\n", sizeof(*person));

//    printf("person name: %s age: %d\n", person->name, person->age);
 //   strcpy(person->name, "kelly");
//    person->age = 23;
 //   printf("person name: %s age: %d\n", person->name, person->age);

    return 0;
}
