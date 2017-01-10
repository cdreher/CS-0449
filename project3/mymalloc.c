//Collin Dreher
#include "mymalloc.h"
#include <sys/mman.h> 
#include <stdio.h>

int MAX_MEM = 1 << 30;		
void *base = NULL;
struct node *freelist[26];		

struct node
{
	char head;		
	struct node *prev_node;
	struct node *next_node; 
};

void *my_buddy_malloc(int size)
{
	int i;
	int x;
	int y;
	
	//Allocates the initial base region of 1GB
	if(base == NULL)
	{
		base = mmap(NULL, MAX_MEM, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, 0, 0);
		freelist[25] = base;
		freelist[25]->prev_node = NULL;
		freelist[25]->next_node = NULL;
		freelist[25]->head = 30;
	}
	
	if(size < 32)	//min region size is 32 +1 for the one char bite
		size = 32;
	else
		size = size + 1;


	x = size - 4;
	while(x>>= 1)
	   	y = y+1;//find log base2
	int index = y;
	 
	
	for(i = index; i < 26; i++)	//find memmory to split
	{
		if(freelist[i] != NULL)
			break;	
	}
	
	if(i != 26)
	{
	  void *temp = split_mem(i,index);//call to split memmory
	  return temp;
	}
}

void my_free(void *ptr)
{
	if(ptr == NULL)
	{	
		printf("Pointer is NULL\n");		
		return;
	}		 
	
	ptr = ptr - 1;		//account for the one char bite
		
	if(*(char *)ptr & 128)	//checks if node is ocupied 
	{
		struct node *p = ptr;
		int index;
			
		p->head = p->head & 127; 	//set  occupied bit to 0
		index = (p->head & 127) - 5;
		p->next_node = freelist[index];
			
			
		if(freelist[index] != NULL)	//sets pointers 
			p->next_node->prev_node = p;
			
		p->prev_node = NULL;
			
		freelist[index] = p;
			
		//calculate address of  buddy
		struct node *buddy = ((void *)base) + (((long long )((void *)p - ((void *)base)))^(( long long)(1 << (p->head))));
		
	
		while((index < 26) && ((buddy->head & 128) == 0))		//makes sure  regions are occupied before  free
		{	
			if(p->prev_node == NULL)			//check if buddy is the first node in the freelist
				freelist[index] = p->next_node;
			else
				p->prev_node->next_node = p->next_node;	//reset pointer
					
			if(p->next_node!= NULL)
				p->next_node->prev_node = NULL;	//remove  previous pointer
					
			if(buddy->prev_node == NULL)			
				freelist[index] = buddy->next_node;
			else
				buddy->prev_node->next_node = buddy->next_node;	
						
			if(buddy->next_node != NULL)
				buddy->next_node->prev_node = buddy->prev_node;
		
			index = index + 1;	
			p->head = index+5;
			buddy->head = index+5;
				
			if(buddy < p)
				p = buddy;
				
			p->next_node = freelist[index];
				
			if(freelist[index] != NULL)
				p->next_node->prev_node = p;
			
			p->prev_node = NULL;
				
			freelist[index] = p;
					
				//recalculate buddy address
			buddy = ((void *)base) + (((long long)((void *)p - ((void *)base)))^((long long)(1 << (p->head))));

		}

	}
}

//recursivly splits buddy to size of requested space
void *split_mem(int i, int index)
{
        struct node *p = freelist[i];
        freelist[i] = p->next_node;

        if(p->next_node != NULL)
	{
		p->next_node->prev_node = NULL;
	}
        p->head = i + 5;          //set p to its new size
        i = i -1;

        p->next_node = ((void *)base) + (((long long)((void *)p - ((void *)base)))^((long long)(1 << (p->head))));

        p->next_node->prev_node = p;        //sets the double links between nodes
        p->next_node->head = p->head;	//sets the status of  buddy node

        p->next_node->next_node = freelist[i];  //having next node go to buddy

        if(freelist[i]!= NULL)
	{
        	freelist[i]->prev_node = p->next_node;  //reset pointers 
	}
        freelist[i] = p;    //Add p to  freelist
        if (i < index)
        {
        	struct node *p = freelist[i];               
                freelist[i] = p->next_node;

                if(p->next_node != NULL)
		{
                        p->next_node->prev_node = NULL;
		}
                p->head = p->head | 256;    //marks  node as occupied by setting bit to 1

                void *return_address = p;
                return_address++;              
		return return_address;
        }
        else
        split_mem(i,index);
}

void dump_heap()
{
    int i = 0;
    for (i; i <= 25; i++)
    {
        struct Block *current = freelist[i];
        printf("%d -> ", i+5);
        while(current != NULL)
        {
            printf("[%d : %d : %.0f] -> ", (current->head & 1),((char *)current - (char *)base),pow(2,(current->head >> 1)));
            current = current->next_node;
        }
        printf("NULL\n");
    }
}


