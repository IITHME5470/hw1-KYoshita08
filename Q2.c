#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
#define epsilon 1e-6

//function to read data from input files
void read_matrix_vectors(int n, double **mat, double **vecs){

        char filename[200];
        snprintf(filename, sizeof(filename), "inputfiles/mat_%06d.in",n);
        FILE *matfile = fopen(filename,"r");

        // Read the matrix from the file
        char line[2048];
        for(int r = 0; r < n; r++){
                if(fgets(line, sizeof(line), matfile) == NULL){
                        fprintf(stderr, "Error reading matrix line %d\n",r);
                        fclose(matfile);
                        return;
                }

                char *token = strtok(line, ",");
                for(int c = 0; c < n; c++){
                        if(token == NULL){
                                fprintf(stderr, "Error reading matrix data at [%d][%d]\n", r, c);
                                fclose(matfile);
                                //break;
                                return;
                        }
                        mat[r][c] = atof(token);
                        token = strtok(NULL, ",");
                }
        }
        fclose(matfile);

        //printf("Printing the matrix for n = %d\n",n);
        //for(int i = 0; i<n; i++){
        //      for(int j = 0; j<n; j++){
        //              printf("%lf    ",mat[i][j]);
        //      }
        //      printf("\n");
        //}

        //reading vectors from the input files
        for(int i = 0; i < 4; i++){
                char fname[200];
                snprintf(fname, sizeof(fname), "inputfiles/vec_%06d_%06d.in",n,i+1);

                FILE *vecfile = fopen(fname, "r");

                if(fgets(line, sizeof(line), vecfile) == NULL) {
                        fprintf(stderr, "Error reading vector file %s\n", fname);
                        fclose(vecfile);
                        return;
                }

                char *token = strtok(line, ",");
                for(int j = 0; j < n; j++){
                        if(token == NULL){
                                fprintf(stderr, "Error reading vector data for vector %d at index %d\n", i+1, j);
                                fclose(vecfile);
                                return;
                        }
                        vecs[i][j] = atof(token);
                        token = strtok(NULL, ",");
                }
                fclose(vecfile);
                //printf("%d vector for n = %d:\n",i+1,n);
                //for(int k = 0; k<n ; k++){
                //      printf("%lf    ",vecs[i][k]);
                //}
                //printf("\n");
        }
        return;
}

//function to check if a vector is eigen or not
void check_eigen(int n, double **mat, double *vec, int vecnum){

        double multres[n];

        //multiplying the matrix and the vector
        for(int r = 0; r < n; r++){
                double sum = 0;
                for(int c = 0; c < n; c++){
                        sum = sum + mat[r][c] * vec[c];
                }
                multres[r] = sum;
        }

        //check if its eigen
        double ratio = 0;
        bool is_eigenvector = true;

        for(int i = 0; i < n; i++){
                if(fabs(vec[i]) > epsilon) {
                        ratio = multres[i] / vec[i];
                        break;
                }
        }
        for(int i = 0; i < n; i++){
                if(fabs(vec[i]) > epsilon){
                        double current_ratio = multres[i] / vec[i];
                        if(fabs(current_ratio - ratio) > epsilon){
                                is_eigenvector = false;
                                break;
                        }
                }
                else if(fabs(multres[i]) > epsilon){
                        //vec[i] is zero but multres[i] is not
                        is_eigenvector = false;
                        break;
                }
        }
        if(is_eigenvector){
                //an eigen vector
                printf("vec_%06d_%06d.in: Yes : %e\n", n, vecnum+1, ratio);
                char fname[200];
                snprintf(fname, sizeof(fname), "inputfiles/vec_%06d_%06d.in",n, vecnum+1);
                FILE *fileptr = fopen(fname, "a");
                fprintf(fileptr, "\nEigenvalue: %e\n", ratio);
                fclose(fileptr);
        }
        else{
                //the vector is not eigen
                printf("vec_%06d_%06d.in: Not an eigenvector\n",n ,vecnum+1);
        }

        return;

}

int main(){

        int n_values[4] = {3, 5, 50, 80};

        for(int i = 0; i < 4; i++){
                int n = n_values[i];

                //alloting space for matrix and vectors
                double **mat = (double **)malloc(n*sizeof(double *));
                for(int i = 0; i < n; i++){
                        mat[i] = (double *)malloc(n*sizeof(double));
                }
                double **vecs = (double **)malloc(4*sizeof(double *));
                for(int i = 0; i < 4; i++){
                        vecs[i] = (double *)malloc(n*sizeof(double));
                }

                printf("Reading the data from input files for n=%d\n",n);
                //populate the matrix and vectors from files
                read_matrix_vectors(n, mat, vecs);
                printf("Done reading data from the input files for n=%d\n",n);

                for(int i = 0; i < 4; i++){
                        //check if the ith vector is eigen vector
                        check_eigen(n, mat, vecs[i], i);
                }

                //free the alloted space for mat and vecs
                for(int i = 0; i < n; i++){
                        free(mat[i]);
                }
                free(mat);
                for(int i = 0; i < 4; i++){
                        free(vecs[i]);
                }
                free(vecs);
        }

        return 0;
}