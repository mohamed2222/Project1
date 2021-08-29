#include <stdio.h>
//#include <stdlib.h>
#include <math.h>



 
 int passByReference(int *a){
	 int i;
	*a=(*a)*3;
		//*a = (*a)*(*a);//squares value at that address

	
	 return (*a);
 }



int main(){
	
	//char *s = "abcdefghijklmnopqrstuvwxyz";
    int array[9] = {0,1,2,3,4,5,6,7,8};
	//int *arraypointer = &array[9];
	
	FILE *file_name  = fopen("file1.txt", "r"); // read only 
	char c;
			   
			  
			  
	/* if (file_name == NULL) 
		{   
				  printf("Error! Could not open file\n"); 
				  exit(-1); // must include stdlib.h 
		} 
			 
	else{ */
			  while (c!=EOF){
				  
					c=fgetc(file_name);
			  
					
	 
					//fprintf(stdout, "%c",c); // write to screen 
					printf("%c",c); // write to screen					
					
											}
     /*    } */
	fclose(file_name);

	
	int b =9;
	int i;
	
	for (i=0;i<b;i++){
		passByReference(&array[i]);
		//*a = (*a)*(*a);//squares value at that address
	 }
	 for (i=0;i<b;i++){
		 printf("%d\n",array[i]);
		//passByReference(&array[i]);
		//*a = (*a)*(*a);//squares value at that address
	 }
	 
	
	return 0;
 
 }


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 // test for files not existing
   
			  // write to file vs write to screen
 
 
 //printf("this is a test %d\n",5); // write to screen
 
 
 
 
 
 
 
 
 
 //fprintf("%c", fgetc(file_name)); // reads file 
					
					//fprintf(stdout, "Print some data to stdout.\n\n");