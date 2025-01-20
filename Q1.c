#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void print_to_file(int n, int **array, int format_flag){

        FILE *fp;
        if(format_flag==1){
                char filename[50];
                snprintf(filename, sizeof(filename), "array_%06d_bin.out",n);

                fp = fopen(filename,"wb");

                if(fp==NULL){
                        perror("Error opening file");
                        return;
                }
                for(int i =0 ;i<n;i++){
                        fwrite(array[i], sizeof(int), n, fp);
                }
                fclose(fp);
        }
        else if(format_flag==0){
                char filename[50];
                snprintf(filename, sizeof(filename), "array_%06d_asc.out",n);

                fp = fopen(filename,"w");

                if(fp!=NULL){
                        for(int i = 0; i<n; i++){
                                for(int j = 0; j<n; j ++){
                                        fprintf(fp,"%0.15e    ",(double)array[i][j]);
                                }
                                fprintf(fp,"\n");
                        }
                }
        }

        return;
}

int main(){
        //reading n from input file
        FILE *file_ptr = fopen("input.in","r");
        int n;
        fscanf(file_ptr,"%d",&n);
        int **array=(int **)malloc(n*sizeof(int *));
        for(int i=0; i<n; i++){
                array[i] = (int *)malloc(n*sizeof(int));
        }

        for(int i = 0; i<n; i++){
                for(int j = 0; j<n; j++){
                        array[i][j] = i+j;
                }
        }
        int format_flag;

        //taking input from user
        printf("Format flag(0(ascii) or 1(binary)):");
        scanf("%d",&format_flag);

        print_to_file(n,array,format_flag);

        for(int i = 0; i<n; i++){
                free(array[i]);
        }
        free(array);
        return 0;
}