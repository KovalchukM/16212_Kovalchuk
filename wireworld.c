#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPTY 0
#define HEAD 1
#define TAIL 2
#define WIRE 3

void read_field_size ( FILE *input, FILE *output, int *height, int *lenght )
{
    while ('\n' != fgetc(input));
    fseek(input, 3, SEEK_CUR);
    fscanf(input, "%d", lenght);
    fseek(input, 6, SEEK_CUR);
    fscanf(input, "%d", height);
    printf("lenght = %d\n", *lenght);
    printf("height = %d\n", *height);
    rewind(input);
    for (int i = 0; i < 2; i++)
    {
        char c = fgetc(input);
        fprintf(output, "%c", c);
        while ('\n' != c)
        {
            c = fgetc(input);
            fprintf(output, "%c", c);
        }
    }
}

int *RLE_decode(FILE *input, int *field, int lenght)
{
	char number = 0;
    int elem = 0;
    int cell = 0;
    char read = fgetc(input);
    while ('!' != read)
    {
        if ('\n' != read)
        {
            if(('0' <= read) && ('9' >= read))
            {
                number = number * 10 + (read - '0');
            }
            else
            {
                if ('$'!= read)
                {
                    if ('.' == read)
                        elem = EMPTY;
                    if ('A' == read)
                        elem = HEAD;
                    if ('B' == read)
                        elem = TAIL;
                    if ('C' == read)
                        elem = WIRE;
                    if (0 == number)
                        number = 1;
                    for (int i = 0; i < number; i++ )
                    {
                        field[cell] = elem;
                        cell++;
                    }
                    number = 0;
                }
                if ('$' == read)
                {
                    if ((0 != number) || (0 == (cell % lenght)))
                    {
                        while(number > 1)
                        {
                            for (int i = 0; i < lenght; i++)
                            {
                                field[cell] = 0;
                                cell++;
                            }
                            number--;
                        }
                    }
                    if (0 != (cell % lenght))
                    {
                        cell = cell - (cell % lenght) + lenght;
                    }
                    number = 0;
                }
            }
        }
    	read = fgetc(input);
    }
	return field;
}

void print_on_screen (int *field, int step, int lenght, int height)
{
	printf( "generation %d\n", step );
	for ( int i = 0; i < lenght * height; i++ )
	{
        if (0 == i % lenght)
            printf ( "\n" );
        if (EMPTY == field[i])
            printf ( "0" );
        if (WIRE == field[i])
            printf ( "=" );
        if (HEAD == field[i])
            printf ( "+" );
        if (TAIL == field[i])
            printf ( "-" );
    }
	printf ("\n");
   	getchar();
}

int *next_generation (int *field, int lenght, int height)
{
    int *next_field = (int*)calloc(lenght * height, sizeof(int));
    int head_counter = 0;
    memcpy(next_field, field, sizeof(int) * lenght * height);
    for (int i = 0; i < lenght * height; i++)
    {
        if (EMPTY == field[i])
            next_field[i] = EMPTY;
        if (HEAD == field[i])
            next_field[i] = TAIL;
        if (TAIL == field[i])
            next_field[i] = WIRE;
        if (WIRE == field[i])
        {
            head_counter = 0;
            for (int k = -1; k <= 1; k++)
            {
                if (HEAD == field[i - lenght + k])
                {
                    head_counter++;
                }
            }
            for ( int k = -1; k <= 1; k++ )
            {
                if (HEAD == field[i + lenght + k])
                {
                head_counter++;
                }
            }
            if (HEAD == field[i - 1])
                head_counter++;
            if (HEAD == field[i + 1])
            {
                head_counter++;
            }
            if ((1 == head_counter) || (2 == head_counter))
            {
                next_field[i] = HEAD;
            }
        }
    }
    memcpy(field, next_field, sizeof ( int ) * lenght * height);
    free(next_field);
}

void print_output (FILE *output, int *field, int height, int lenght )
{
    int string_nymeric = 0;
    int count = 0;
    int elem = -1;
    for ( int i = 0; i < (lenght * height); i++ )
    {
        if (field[i] != elem)
        {
            if (1 < count)
                fprintf(output, "%d", count);
            if (HEAD == elem)
                fprintf(output, "A");
            if (TAIL == elem)
                fprintf(output, "B");
            if (EMPTY == elem)
                fprintf(output, ".");
            if (WIRE == elem)
                fprintf(output, "C");
            elem = field[i];
            count = 0;
        }
        count++;
        if ((0 == i % lenght) && (height-1 > string_nymeric) && (0 != i))
        {
            printf("%d\n", i );
            int k = 0;
            for(k = i; k < (lenght * height)-1; k++)
            {
                if (field[k] != field[k+1])
                {
                    break;
                }
                if ( 0 == k % lenght)
                    string_nymeric++;
            }
            if (1 < string_nymeric)
                fprintf(output, "%d", string_nymeric );
            if (0 != string_nymeric)
                fprintf(output, "$");
            for (k = 1; k < string_nymeric; k++)
                i = i + lenght;
            string_nymeric == 0;
            i = i - (i % lenght);
        }
    }
    fprintf(output, "!");
}

int main( int argc, char *argv[])
{
    if (2 > argc)
    {
        printf("wrong nymder of arguments\n");
        return (1);
    }
    FILE *input = fopen(argv[1], "r");
    if (NULL == input)
    {
        printf ("Cannot open file\n");
        return (1);
    }
    int generation_nymber = -1;
    if(3 == argc)
    {
        generation_nymber = atoi(argv[2]);
        if (0 >= generation_nymber)
        {
            printf("wrong nymder of generations\n");
            return (1);
        }
    }
    FILE *output = fopen("wireworld.txt", "w+");
    int height = 0;
    int lenght = 0;
    int step = 1;
    read_field_size(input, output, &height, &lenght);
    int *field = (int*)calloc(height * lenght, sizeof(int));
    RLE_decode(input, field, lenght);
    print_on_screen ( field, 0, lenght, height);
    if (-1 == generation_nymber)
    {
        while (1)
        {
            next_generation(field, lenght, height);
            print_on_screen ( field, step, lenght, height);
            step++;
        }
    }
    while (generation_nymber >= step)
    {
        next_generation (field, lenght, height);
        print_on_screen (field, step, lenght, height);
        step++;
    }
    print_output(output, field, height, lenght);
    fclose(input);
    free(field);
    return 0;
}
