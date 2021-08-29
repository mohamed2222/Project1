#include <stdlib.h>
#include <stdio.h>

int main(){
int *x = NULL;
int y = 10;
x = &y;
printf("%d %d\n", *x, y);

return EXIT_SUCCESS;
}
