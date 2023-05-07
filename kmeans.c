#include<stdio.h>
#include<stdlib.h>

// STAGE 1

char* dynamically_read_line(int* line_length){
    int counter = 0;
    int MORE_CHARS = 512;
    char c = 'a';
    size_t num_of_chars_left = MORE_CHARS;
    char* line;
    FILE* file = stdin;
 

     if (file == NULL)
  {
    printf("Error opening file.\n");
    exit(1);
  }

    line = (char*)malloc((MORE_CHARS)*(sizeof(char)));
    if (line == NULL){
        printf("failed to allocate memory :(\n");
        exit(1);
    }

    while((c != '\n' )){

        c = fgetc(file);

        if (ferror(file)){
            printf("there is an error in reading the file :(\n");
            exit(1);
        }
        

        if ( feof(file) ){
            // we have reached the end of the file and we didnt have an empty string
                line = realloc(line,*(line_length) + 1);
                line[*(line_length)] = '\0';
                return line;



        }

        if (c == '\n'){

            line = realloc(line,*(line_length) + 1);
                line[*(line_length)] = '\0';
            return line;

            

        }


        
        if (num_of_chars_left == 0){
            num_of_chars_left = MORE_CHARS*2;
            line = realloc(line,*(line_length) + MORE_CHARS*2);


        }



        line[*(line_length)] = c; 
        *(line_length) +=1;
        num_of_chars_left +=-1;  
        counter +=1;    
        

    }

    printf("fail\n");
    

    return line;
    

}


// STAGE 2

double* convert_line_string_to_double_array(char* line,int* length_line, int* num_of_double_elements,int extracting_row,int* row_dimenstion){
    char* pointer;
    double number;
    double * double_array = (double*)malloc(*(length_line)*sizeof(double));




    number = strtod(((const char*)line),&pointer);
    double_array[*(num_of_double_elements)] = number;

 
    while(*pointer != '\0'){
        *(num_of_double_elements) +=1;
        pointer +=1; // we jump after ',' to go to the next number !


        number = strtod( (const char*)(pointer),&pointer);
        double_array[*(num_of_double_elements)] = number;


    }

    *(num_of_double_elements) +=1;

 
    

    double_array = realloc(double_array,*(num_of_double_elements)*sizeof(double));

    if (extracting_row) {
        *(row_dimenstion) = *(num_of_double_elements);

    }

    return double_array;

}


// STAGE 3

double ** create_matrix(int* num_rows_in_matrix_ptr,int* dimension_ptr){

    // VARIABLE DECLERATIONS//

    //GENERAL
    int MORE_LINES = 1024;

    // FOR GEETING THE LINES
    int line_len = 0;
    int* line_len_ptr = &line_len;



    //FOR CONVERTING LINE TO DOUBLE ARRAY

    int num_double_elements = 0;
    int* num_double_element_ptr = & num_double_elements;
    double* double_line_ellements_array;
    char* LINE;


    // FOR MATRIX
    int number_of_rows_that_are_free = MORE_LINES;

    double** MATRIX;

    // LETS START

    MATRIX = (double**)malloc((MORE_LINES)*(sizeof(double*)));
    if (MATRIX == NULL){
        printf("failed to allocate memory :(\n");
        exit(1);
    }

    ////////////////////////////////////

    LINE = dynamically_read_line(&line_len);
     

    while( *(line_len_ptr) != 0 ){


        if (*(dimension_ptr) == 0 ){
        MATRIX[*(num_rows_in_matrix_ptr)] = convert_line_string_to_double_array(LINE,&line_len,&num_double_elements,1,dimension_ptr);

        }

        else{
            MATRIX[*(num_rows_in_matrix_ptr)] = convert_line_string_to_double_array(LINE,&line_len,&num_double_elements,0,dimension_ptr);
            
            }


        *(num_rows_in_matrix_ptr) +=1;
        number_of_rows_that_are_free += -1;
        *(line_len_ptr) = 0;
        *(num_double_element_ptr) = 0;

        if ( (number_of_rows_that_are_free)  == 0 ) {

            MATRIX = realloc(MATRIX,*(num_rows_in_matrix_ptr) + MORE_LINES );

            if (MATRIX == NULL) {
                printf("failed to REALLOCATE MEMORY");
                exit(1);

            }

            number_of_rows_that_are_free = MORE_LINES;

        }

    // FINISHED READING THIS LINE. NOW NEED TO FREE IT AND READ ANOTHERONE

        *(line_len_ptr) = 0;
        free(LINE);

        LINE = dynamically_read_line(&line_len);


    }

    //FINISHED TO GER ALL THE MATRIX ROWS. NOW REACLLOCATE IT TO THE PROPER AMOUNT OF ROWS THAT THERE ARE

    

    return MATRIX;

    }




// LENGTH FUNCTION

double LENGTH(double* V1, double* V2,int dimension){

    double RESULT = 0;
    double SUBTRACTION = 0;
    for(int i = 0; i < dimension ; i++){
        SUBTRACTION = V1[i] - V2[i];
        RESULT +=  ( (SUBTRACTION)*(SUBTRACTION) );
    }


    return RESULT;

}


// CLASSIFY FUNCTION

int CLASSIFY_CENTROID(double* X_i,int K, int dimension,double CENTROIDS[K][dimension]){


    int RESULT = -1;
    double MINIMUM = __DBL_MAX__;
    double MINIMUM_CONTENDER;

    for(int j = 0; j < K ; j++) {

        MINIMUM_CONTENDER = LENGTH(X_i,CENTROIDS[j],dimension);

        if (MINIMUM_CONTENDER < MINIMUM) {
            MINIMUM = MINIMUM_CONTENDER;
            RESULT = j;
        }
        
    }

    
    return RESULT;

}


// UPDATE CENTROID FUNCTION

int UPDATE_CENTROIDS(double** matrix,int k,int dimension,int num_rows, double centroids[k][dimension] ){
    int enough_precision = 0;

    long double EPSILON = 0.000001;

    // INITIALIZE NUMBERS IN EACH CLUSTER IN 0

    int numbers_of_elements_in_each_kluster[k];
    for (int i = 0; i < k ; i++){
        numbers_of_elements_in_each_kluster[i] = 0;
    }

    double new_centroids[k][dimension];

    for(int a = 0 ; a < k ; a++){
            for (int b = 0; b < dimension ; b ++){
                new_centroids[a][b] = 0;
            }
    }

    int index;

    for(int i = 0 ; i < num_rows ; i++ ){
        index = CLASSIFY_CENTROID(matrix[i],k,dimension,centroids);
        numbers_of_elements_in_each_kluster[index] +=1;

        for (int j = 0; j < dimension ; j++){
            new_centroids[index][j] += matrix[i][j];

        }

    }

    for (int i = 0 ; i < k ; i++){
        for ( int j =0 ; j < dimension ; j++){
            new_centroids[i][j] = ( (new_centroids[i][j])*((double) 1 / (double) ( numbers_of_elements_in_each_kluster[i])) );

        }
    }

        for(int i = 0; i < k ; i++){
            if (LENGTH(centroids[i],new_centroids[i],dimension) >= EPSILON) {
                enough_precision = 1;

            }
        }
    
    // SET NEW CENTROIDS

    for (int i = 0 ; i < k ; i++){
        for ( int j =0 ; j < dimension ; j++){
            centroids[i][j] = ((new_centroids[i][j]));

        }

    }

return enough_precision;

}







int main(int argc, char* argv[]){

    int k = 3;
    printf(" k is :  %d\n",k);
    int num_of_rows = 0;
    int* num_of_rows_ptr = & num_of_rows;
    int dimension = 0;
    int* dimension_ptr = & dimension;

    /////////////////////////////////////

    double** MATRIX = create_matrix(&num_of_rows, &dimension);

    // INITIALIZE CENTROIDS
    double  centroids[k][*(dimension_ptr)];
    printf("\n\ninizializing centroids \n\n");

    for( int i = 0 ; i < k ; i++){

        for (int j = 0 ; j < *(dimension_ptr) ; j++) {
            centroids[i][j] = MATRIX[i][j];
        }


    }

    /////////////////////////////////////////////
    
    // ALGORITHM

    int i = 0;
    int precision = 1;
    while ( i < 200 && precision == 1){
        precision = UPDATE_CENTROIDS( MATRIX ,k,*(dimension_ptr),*(num_of_rows_ptr),centroids);
        i +=1;
    
    }



    for( int a = 0 ; a < k ; a++){
        //printf("adress of centroid number %d is %p \n",a, & centroids[a]);

        for (int j = 0 ; j < *(dimension_ptr) ; j++) {
            printf("%.4lf ",centroids[a][j]);
        }
        printf("\n");
    }
    printf("\n");
    
    // FREE ALL THE ROWS

    for(int i = 0; i < *(num_of_rows_ptr) ; i++) {
    free(MATRIX[i]);
    }

    printf("free everybody\n\n\n");
    free(MATRIX);

    return 0;
}