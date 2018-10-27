#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include "CacheHeadder.h"

int main ( char argc, char *argv[])
{
	FILE *Fptr;
	char address[100];
	char read_write[100];
	char *Binary;
	int argument_length;
	char Argument1[10];
	int cachesize=0;
	
	
	/*  1.  input Cache size 
		2.  input Associativity (Number of ways)
		3.  Cache line size
	*/
	
	/*total cache size*/
	argument_length = strlen(argv[1]);
	strncpy (Argument1,argv[1],argument_length-2);
	cachesize = atoi(Argument1);
	if(argv[1][argument_length-1] == 'B'){
		switch (argv[1][argument_length-2])
		{
			case 'K' : cachesize = cachesize * 1024;
						break;
			case 'M' : cachesize = cachesize * 1024 * 1024;
						break;
			case 'G' : cachesize = cachesize * 1024 * 1024 * 1024;
						break;
			default : printf("ERROR: invalid cache size\n");
						exit(0);
						
		}
	} else {
		printf("ERROR: invalid cache size\n");
		exit(0);
	}
	/* total cache size*/
	Cache_size = cachesize;	

		
	Assoc = atoi(argv[2]);   

	Line_size = atoi(argv[3]);	

	/*total Number of Sets*/
	Number_Of_Sets =(cachesize/Line_size)/Assoc;

	
	
	/*total number of cache line*/
	Number_Of_CacheLine = cachesize / Line_size;
	
	printf("\nCACHE PARAMETERS\n");
	printf("Number_Of_Sets = %d\n",Number_Of_Sets);
	printf("Associativity = %d\n",Assoc);
	printf("Cache line size = %d\n",Line_size);
	
	/* check if Byte select,set and associativity is power of 2 and also set the number of bits for byte_select and sets*/
	isPowerOf2_And_setbits();
	
	 Fptr = fopen(argv[4], "r");
	 
	  if (Fptr == NULL)
		  {
			  fprintf(stderr, "ERROR: cannot open input file. \n");
			  exit(0);
				
		  }		
		cache = Cache_Memory_allocation();

	while (fscanf(Fptr, "%d %s", &Write_OR_Read, address) != EOF)
	{	
		
		Address_length = Convert_To_Binary(address);	
		
		if(Write_OR_Read == 1)
		{
			
            write_to_cache(Address_length); 
			
        }
        else if(Write_OR_Read == 0)
		{
			
            read_from_cache(Address_length);
        }		
			
	}
	
	Cache_Hit_Ratio = (float) Cache_hits/ Cache_access *100 ;
	Cache_Miss_Ratio = (float)Cache_misses/Cache_access * 100;
	
	printf("\nCACHE STATISTICS\n");
	printf("Total number of cache accesses: %d\n", Cache_access);
	printf("Number of cache reads : %d\n", Cache_reads);
	printf("Number of cache writes: %d\n", Caxhe_writes);
	printf("Number of cache hits: %d\n",Cache_hits);
	printf("Number of cache misses: %d\n",Cache_misses);
	printf("Cache hit ratio :%.2f %%\n",Cache_Hit_Ratio);
	printf("Cache miss ratio: %.2f %%\n",Cache_Miss_Ratio);
	printf("Number of evictions: %d\n",Evection);
	printf("Number of write backs: %d\n",Write_back);


}


void write_to_cache(char address[])
{
	
	int index = Index_value(address, get_tag_length(address))* Assoc; //* Assoc;
	int i = 0;
	Caxhe_writes++;
	Cache_access ++;
	
	//Check if this line present in cache 
	for (i=index;i < index + Assoc; i++)
	{
		if(cache[i].valid_bit == 1)
		{

			
			if(strncmp(cache[i].Cache_Line, address, cache[i].tag_length) == 0)
			{
									
				Cache_hits++;				
				cache[i].tag_length = get_tag_length(address);
				cache[i].dirty_bit = 1;
				LRU(i);
				return;		
			}				
		}		
	}
	Cache_misses++;
	//if not present in cache,Check for any free position in any way number.

	for (i=index;i < index + Assoc; i++)
	{		
		if(cache[i].valid_bit == 0)
		{
			cache[i].valid_bit = 1;
            cache[i].tag_length = get_tag_length(address);
            strcpy(cache[i].Cache_Line, address);
            cache[i].dirty_bit = 1;
			LRU(i);
            return;
		}
	}
	
	//if all the ways are full then check which is Least recently used by checking LRUbit
	
		for (i=index;i < index + Assoc; i++)
		{
			if (cache[i].LRU_bit == 0)
			{
				if(cache[i].dirty_bit == 1)
				{
					Write_back++;
				}				
				Evection++;
				cache[i].tag_length = get_tag_length(address);
				strcpy(cache[i].Cache_Line, address);
				cache[i].valid_bit = 1;
				cache[i].dirty_bit = 1;
				
				//LRU(i, cache[i].Select_set);
				LRU(i);
				return;
			}			
		}
	//If all the LRU bits are set then clear all LRU bits 
	for (i=index;i < index + Assoc; i++)
		{
			cache[i].LRU_bit = 0;
		}
		
    // Update from left to right
	for (i=index;i < index + Assoc; i++)
		{
			if (cache[i].LRU_bit == 0)
			{
				if(cache[i].dirty_bit == 1)
				{
					Write_back++;
				}
				Evection++;
				cache[i].tag_length = get_tag_length(address);
				strcpy(cache[i].Cache_Line, address);				
				cache[i].valid_bit = 1;
				cache[i].dirty_bit = 1;
				LRU(i);
				return;
			}			
		}		
}
void read_from_cache(char address[])
{
	int index = Index_value(address, get_tag_length(address))* Assoc; //* Assoc;
	Cache_access ++;
	Cache_reads++;
	//Check if this line present in cache 
	for (int i=index;i < index + Assoc; i++)
	{
		if(cache[i].valid_bit == 1)
		{
			if(strncmp(cache[i].Cache_Line, address, cache[i].tag_length) == 0)
			{	

				Cache_hits++;
				cache[i].tag_length = get_tag_length(address);
				LRU(i);
				return;		
			}				
		}		
	}
	
	Cache_misses++;
	//if not present in cache,Check for any free position in any way number.	
	for (int i=index;i < index + Assoc; i++)
	{
		if(cache[i].valid_bit == 0)
		{			

			cache[i].valid_bit = 1;
			cache[i].dirty_bit = 0;
            cache[i].tag_length = get_tag_length(address);
            strcpy(cache[i].Cache_Line, address);
            LRU(i);
            return;
		}
	}
	//if all the ways are full then check which is Least recently used by checking LRUbit
	for (int i=index;i < index + Assoc; i++)
		{
			if (cache[i].LRU_bit == 0)
			{
				if(cache[i].dirty_bit == 1)
				{
					Write_back++;
				}
				Evection++;
				cache[i].tag_length = get_tag_length(address);
				strcpy(cache[i].Cache_Line, address);
				cache[i].valid_bit = 1;
				cache[i].dirty_bit = 0;
				LRU(i);
				return;
				
			}			
		}
	
		//If all the LRU bits are set then clear all LRU bits 
	for (int i=index;i < index + Assoc; i++)
		{
			cache[i].LRU_bit = 0;
		}
		
    // Update from left to right
	for (int i=index;i < index + Assoc; i++)
		{
			if (cache[i].LRU_bit == 0)
			{
				if(cache[i].dirty_bit == 1)
				{
					Write_back++;
				}
				Evection++;
				cache[i].tag_length = get_tag_length(address);
				strcpy(cache[i].Cache_Line, address);
				cache[i].valid_bit = 1;
				cache[i].dirty_bit = 0;
				LRU(i);
				return;
			}			
		}	
}

void LRU(int index)
{
	cache[index].LRU_bit =1;
}
/* 
	Index_value()
	start from index position length of 

	ex: 4 way = 2 bits associativity 64 cache line size = 6 bits 

	0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 30 31 32 
	1 0 1 1 0 0 1 1 0 0  1  1  0  0  1  1  0  0  1  1  0  0  1  1  0  0  1  1  0  0  1  1

	total length = 32 
	cache line address bits position= 26-32
	sets address bits position = 24 -25
	tag = 1-23 
	point to that index and read 2 bits from that index.
	Multiplier = 1,10,100,1000 
	

*/
	int Index_value(char address[], int tag_length)
	{
		int index = 0;
		int Multiplier = 1;
		int i = strlen(address) - 1 - Byte_select_bits; 
		for(; i >= tag_length; i--)
		{
			if(address[i] == '1')
			{
				index += Multiplier;
			}
			Multiplier = Multiplier<<1;
		}
		return index;
		
	}

/* get the tag length 
tag length = Total address bits - Set bits - Byte select bits 
*/
int get_tag_length(char address[])
{
   int tag_length =0;

   tag_length = (strlen(address) - Set_bits - Byte_select_bits);
   return tag_length;
}
/* 
	Convert_To_Binary(char address)
	Read the address string, convert each character to corresponding binary value and concatenate 
*/
	
char * Convert_To_Binary(char address[])
{
	char *binary =((char *)malloc(sizeof(char) * 33));
	int i = 0;
	
	for(i=0; i< strlen(address);i++)
	{
		
		switch(address[i])
		{
			
            case '0': strcpy(&binary[(i)*4],"0000"); break;
            case '1': strcpy(&binary[(i)*4],"0001"); break;
            case '2': strcpy(&binary[(i)*4],"0010"); break;
            case '3': strcpy(&binary[(i)*4],"0011"); break;
            case '4': strcpy(&binary[(i)*4],"0100"); break;
            case '5': strcpy(&binary[(i)*4],"0101"); break;
            case '6': strcpy(&binary[(i)*4],"0110"); break;
            case '7': strcpy(&binary[(i)*4],"0111"); break;
            case '8': strcpy(&binary[(i)*4],"1000"); break;
            case '9': strcpy(&binary[(i)*4],"1001"); break;
            case 'a': strcpy(&binary[(i)*4],"1010"); break;
            case 'b': strcpy(&binary[(i)*4],"1011"); break;
            case 'c': strcpy(&binary[(i)*4],"1100"); break;
            case 'd': strcpy(&binary[(i)*4],"1101"); break;
            case 'e': strcpy(&binary[(i)*4],"1110"); break;
            case 'f': strcpy(&binary[(i)*4],"1111"); break;
			case 'A': strcpy(&binary[(i)*4],"1010"); break;
            case 'B': strcpy(&binary[(i)*4],"1011"); break;
            case 'C': strcpy(&binary[(i)*4],"1100"); break;
            case 'D': strcpy(&binary[(i)*4],"1101"); break;
            case 'E': strcpy(&binary[(i)*4],"1110"); break;
            case 'F': strcpy(&binary[(i)*4],"1111"); break;
			default : printf("Input trace is not a valid address : %s\n",address);
						exit(0);
						
						
		}	
	
	}

	 return binary;	
}	
/*
	isPowerOf2_And_setbits();
	This function
		1. check weather the line size, associativity and number of sets is power of 2 
		2. determine number of bits need to represent byte select and sets
*/
		
void isPowerOf2_And_setbits()
{

	int i = 1;
	int Associativity_check =0;
	/* selecting number of bits for byte select*/
	
		for(i =1;i < Line_size ;i=i*2)
		{
			Byte_select_bits++;
		}
		if(i != Line_size && Byte_select_bits !=0)
		{
			 printf("ERROR: Block size is not a power of 2. \n");
			exit(0);
		}
			
	/* selecting number of bits for sets*/
	
		for(i =1;i < Number_Of_Sets ;i=i*2)
		{
			Set_bits++;
		}
		if(i != Number_Of_Sets && Set_bits !=0)
		{
			 printf("ERROR:set size is not a power of 2. \n");
			exit(0);
		}
	/* determining associativity is power of 2 */
	
		for(i =1;i < Assoc ;i=i*2)
		{
			Associativity_check++;
		}
		if(i != Assoc && Associativity_check !=0)
		{
			 printf("ERROR:number of ways is not power of 2. \n");
			 exit(0);
		}
		
}
/*
	Cache_Memory_allocation()
	Allocating the dynamic memory for cache,
	cache line parameters along with meta dat 
							1. VALIB_BIT
							2. SET_BIT
							3. LRU_BIT
							4. DIRTY_BIT
							5. TAG_LENGTH
							6. CACHE_LINE

   All the block in any set is present abject to each other 
	ex:  4 way 16 cache line 
   set 0 lines = 0,1,2,3
   set 1 lines = 4,5,6,7
   set 2 lines = 8,9,10,11
   set 4 line = 12,13,14,15
   
*/
CacheLine *Cache_Memory_allocation()
{
	 CacheLine *cache;
	 cache = malloc(sizeof(CacheLine) * Number_Of_CacheLine);
	  int i = 0;
	 
		for(i = 0;i < Number_Of_CacheLine; i++)
		{
			cache[i].valid_bit = 0;
			cache[i].LRU_bit = 0;
		    cache[i].dirty_bit = 0;		  
		    cache[i].tag_length = 1 ;
			cache[i].Cache_Line = (char *)malloc(sizeof(char) * 33);
			 
		}
    return cache;
	 
}


        
	