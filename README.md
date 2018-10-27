# Cache-Simulator-

Single Level Cache simulater.

The simulator will take the provided input address trace as its input. It will implement the workings of the cache. Specifically, it will keep track of which blocks are brought into the cache and which blocks are being evicted. At the completion of the trace, the simulator will provide statistics about cache hit ratio, read traffic, write traffic etc.

The input trace contains information only about the addresses that are being accessed. Since there is no data information in the trace.

Input Trace :

The simulator will take an address trace as its input. This trace captures the
sequence of accesses that are being made to the cache. For each access, the trace provides the
type of access and the memory address that is being accessed.


Each line in the trace has the following format:
<Access_type Hex_Address>

Access_type signifies the type of the memory operation (read or a write). It is encoded as a binary digit. Access_type = 0 indicates a read and Access_type = 1 indicates a write. Hex_address is the address which is being accessed. The address is encoded in the hexadecimal (base-16) format. Each address is a 32-bit number (8 hexadecimal digits) and signifies the byte which is being accessed. 

example 1:  Let us assume that the first line in the address trace is as follow:
	0 00A53C00
	This represents a read request to the byte address 0x00A53C00
	
Example 2: Let us assume that the second line in the address trace is as follow:
1 3C44DB20
This represents a write request to the byte address 0x3C44DB20

configurable parameters:
	 Number of sets
	 Associativity (Number of ways)
	 Cache line size
	 each of the above three parameters will always be a power of 2. 
	 
Simulator output: After the last access in the trace has been simulated, the simulator will output the following statistics:

	Total number of cache accesses
	Number of cache reads
	Number of cache writes
	Number of cache hits
	Number of cache misses
	Cache hit ratio
	Cache miss ratio
	Number of evictions
	Number of writebacks
	
