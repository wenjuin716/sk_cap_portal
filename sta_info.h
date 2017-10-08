#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum{
	WIRED=0,
	WIRELESS
}dev_type; 

struct sta_info {
	dev_type type;
        char mac[20];
};

