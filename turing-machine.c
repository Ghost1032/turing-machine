/*
Author: Ghost1032
Date: 2021/10/11
Fork from: https://github.com/yedhukrishnan/turing-machine
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUTHORITY "XDU"
#define MAX_OF_STATE_SYMBOL 5 //状态码最大字符
#define MAX_OF_STATE_COUNT 1000 //最大状态数
#define MAX_DATA_LENGTH 100 //最大数据长度
#define MAX_DATASET_NUMBER 100 //最大数据个数
#define STATE_START "S" //启动状态码
#define STATE_END "E" //结束状态码
#define SYMBOL_BLANK '*' //空白符
#define MOVE_LEFT 'L' //左移
#define MOVE_RIGHT 'R' //右移
#define MOVE_STOP '#' //停止
#define END_OF_CMDSET "#" //命令读取停止符

//void print_status();
void print_usage();
void update(char new_ch, char move);
void read_commands();
void read_data();
void execute();

char current_state[MAX_OF_STATE_COUNT][MAX_OF_STATE_SYMBOL],
    new_state[MAX_OF_STATE_COUNT][MAX_OF_STATE_SYMBOL],
    dataset[MAX_DATASET_NUMBER][MAX_DATA_LENGTH];

char current_char[MAX_OF_STATE_COUNT],
    new_char[MAX_OF_STATE_COUNT],
    movement[MAX_OF_STATE_COUNT];

int head, status, cmd_num = 0, dataset_num = 0, data_length;
/*
head:pointer
status:Flag current status of the program(0,1,2)
*/

char state[MAX_OF_STATE_SYMBOL], FILEPATH[100], data[MAX_DATA_LENGTH];

FILE *fin, *fout, *fin1;

int main(int argc, char *argv[])
{

    printf(AUTHORITY);
    if (argc > 2)
    {
        fin = fopen(argv[1], "r");
        if (fin == NULL)
        {
            printf("Target commands file not found: %s\n", argv[1]);
            exit(0);
        }
        read_commands();

        fin1 = fopen(argv[2], "r");
        if (fin1 == NULL)
        {
            printf("Target dataset file not found: %s\n", argv[2]);
            exit(0);
        }
        read_data(fin1);
    }
    else
    {
        print_usage();
        exit(0);
    }
    for (int i = 0; i < dataset_num; i++)
    {
        printf("Executing Case#%d,raw data:%s\n", i, dataset[i]);
        strcpy(data, dataset[i]);
        execute(data, i);
    }
    return 0;
}

void read_commands()
{
    for (int i = 0; i < MAX_OF_STATE_COUNT; i++)
    {
        fscanf(fin, "%s", &current_state[i]);
        if (!strcmp(current_state[i], END_OF_CMDSET))
            break;
        cmd_num++;
        fscanf(fin, " %c %c %c %s\n", &current_char[i], &new_char[i], &movement[i], &new_state[i]);
    }
}

void read_data(FILE *fin1)
{
    fscanf(fin1, "%d", &dataset_num);
    for (int i = 0; i < dataset_num; i++)
    {
        fscanf(fin1, "%s", &dataset[i]);
        printf("Dataset Case%d: %s\n", i, dataset[i]);
    }
}

void execute(char data[MAX_DATA_LENGTH], int case_num)
{
    head = 0;
    data_length = strlen(data);
    strcpy(state, STATE_START);
    while (1)
    {
        status = 0;
        for (int i = 0; i < cmd_num; i++)
        {
            if (!strcmp(state, current_state[i]) && data[head] == current_char[i])
            { //Select command by current state and data
                printf("State: %s Changing from raw data No%d %c to new data %c\n", state, head, data[head], new_char[i]);
                update(new_char[i], movement[i]);
                strcpy(state, new_state[i]);
                //print_status()
                if (!strcmp(state, STATE_END))
                { //Program runs at END state,halting
                    status = 2;
                }
                else
                {
                    status = 1; //Not END state, continue
                }
                break;
            }
        }

        if (status == 0)
        {
            printf("No matching command for current_char %c number %d at state %s. Check your commands set!\n", data[head], head, state);
            break;
        }
        else if (status == 2)
        {
            printf("Success!\n");
            printf("Result for Case#%d: %s\n\n", case_num, data);
            break;
        }
    }
}

void update(char new_ch, char move)
{
    data[head] = new_ch;
    if (move == MOVE_RIGHT)
    {
        head++;
    }
    else if (move == MOVE_LEFT)
    {
        head--;
    }
    if (head == -1)
    {
        for (int i = data_length; i > 0; i--)
        {
            data[i] = data[i - 1];
        }
        data[0] = SYMBOL_BLANK;
        data_length++;
        data[data_length] = '\0';
        head = 0; //Left overflow,copy the data and expand length
    }
    else if (head == data_length)
    {
        data[data_length] = SYMBOL_BLANK;
        data_length++; //Right overflow,set it to BLANK and expand length
        data[data_length] = '\0';
    }
}

void print_usage()
{
    printf("Usage:main <commandset> <dataset>\n");
    printf("To run the program, two files are required:\n");
    printf("One for dataset and another for commands\n\n");
    printf("For commands format:\n");
    printf("Standard format: STATE1 rawdata newdata movement STATE2\n");
    printf("Use \'%s\' as initial state, \'%s\' as end state\n", STATE_START, STATE_END);
    printf("Use \'%c\' as moving left, \'%c\' as moving right, \'%c\' as movement for END state\n", MOVE_LEFT, MOVE_RIGHT, MOVE_STOP);
    printf("Example command:\nS1 1 0 R S2\n");
    printf("Finally,use \'%s\' at last line to tell the program to stop \n\n", END_OF_CMDSET);
    printf("For dataset:\n");
    printf("Write down the total number of datas at first line\n");
    printf("Example:\n");
    printf("2\n");
    printf("10010110\n");
    printf("01001010\n");
}
