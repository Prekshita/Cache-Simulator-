#ifndef CacheHeadder
#define CacheHeadder


typedef struct CacheLine{
    int valid_bit ;
    int LRU_bit;
    int dirty_bit;
    int tag_length;
	char *Cache_Line;
} CacheLine;

int Cache_hits = 0;
int Cache_misses = 0;
int Cache_reads = 0;
int Caxhe_writes = 0;
int Number_Of_Sets = 0;
int Number_Of_CacheLine =0;
int Cache_size = 0;
int Line_size = 0;
int Assoc = 0;
int Cache_access =0;
float  Cache_Hit_Ratio = 0;
float Cache_Miss_Ratio = 0;

int Evection =0;
int Write_back =0;
int Byte_select_bits =0;
int Set_bits  =0;
int Write_OR_Read = 0;
char* Address_length =0;


CacheLine* cache;

void isPowerOf2_And_setbits();
CacheLine *Cache_Memory_allocation();
void write_to_cache(char address[]);
void read_from_cache(char address[]);
char * Convert_To_Binary(char address[]);
int get_tag_length(char address[]);
int Index_value(char address[], int Tag_length);
void LRU(int index);
#endif

