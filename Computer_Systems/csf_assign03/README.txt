TODO: names of team members and their contributions to the project
NOTE: This is only stating what we generally did, we both helped in all functions and help test code
and made changes accordingly to the code itself. 

Bella:
    Create the Structs:
        Cache, Set, Block
    Main Cache Functions:
        Reading In the Files
        Handling Load and Store
        Handling Hit/Miss
    Seperating the Files

Kyle:
    Direct Cache Functions:
        Direct Load and Store
        Adding New Blocks To Sets
    Eviction Functions:
        Update FIFO and lru
        Evicting the block accordingly
    Cache Report

Both:
    Generic Testing
    Memory Handling Testing
    Cycle Tracker


TODO (for MS3): best cache report

Direct-Mapped Cache:
    FIFO
        "Normal" Number of Sets:
        ./csim 128 1 64 write-allocate write-back fifo < gcc.trace
            Load hits: 304442
            Load misses: 13755
            Store hits: 192540
            Store misses: 4946
            Total cycles: 39711349

        Decrease Number of Sets and Increase Number of Bytes Per Block:
        ./csim 64 1 128 write-allocate write-back fifo < gcc.trace
            Load hits: 298003
            Load misses: 20194
            Store hits: 191918
            Store misses: 5568
            Total cycles: 104114851

        Increase Number of Sets and Decrease Number of Bytes Per Block:
        ./csim 256 1 32 write-allocate write-back fifo < gcc.trace
            Load hits: 307356
            Load misses: 10841
            Store hits: 190546
            Store misses: 6940
            Total cycles: 21181023

    LRU
        "Normal" Number of Sets:
        ./csim 128 1 64 write-allocate write-back lru < gcc.trace
            Load hits: 304442
            Load misses: 13755
            Store hits: 192540
            Store misses: 4946
            Total cycles: 39711349

        Decrease Number of Sets and Increase Number of Bytes Per Block:
        ./csim 64 1 128 write-allocate write-back lru < gcc.trace
            Load hits: 298003
            Load misses: 20194
            Store hits: 191918
            Store misses: 5568
            Total cycles: 104114851

        Increase Number of Sets and Decrease Number of Bytes Per Block:
        ./csim 256 1 32 write-allocate write-back lru < gcc.trace
            Load hits: 307356
            Load misses: 10841
            Store hits: 190546
            Store misses: 6940
            Total cycles: 21181023

Fully Associative Cache:
    FIFO:  
        "Normal" Number of Sets:
        ./csim 1 128 64 write-allocate write-back fifo < gcc.trace
            Load hits: 314454
            Load misses: 3743
            Store hits: 194478
            Store misses: 3008
            Total cycles: 17815173
        
        Decrease Number of Blocks Per Set and Increase Number of Bytes Per Block:
        ./csim 1 64 128 write-allocate write-back fifo < gcc.trace
            Load hits: 313974
            Load misses: 4223
            Store hits: 195545
            Store misses: 1941
            Total cycles: 30189883
        
        Increase Number of Blocks Per Set and Decrease Number of Bytes Per Block:
        ./csim 1 256 32 write-allocate write-back fifo < gcc.trace
            Load hits: 314103
            Load misses: 4094
            Store hits: 192254
            Store misses: 5232
            Total cycles: 13268583

    LRU:
        "Normal" Number of Sets:
        ./csim 1 128 64 write-allocate write-back lru < gcc.trace
            Load hits: 315687
            Load misses: 2510
            Store hits: 194686
            Store misses: 2800
            Total cycles: 14607143
                    
        Decrease Number of Blocks Per Set and Increase Number of Bytes Per Block:
        ./csim 1 64 128 write-allocate write-back lru < gcc.trace
            Load hits: 315532
            Load misses: 2665
            Store hits: 195791
            Store misses: 1695
            Total cycles: 22004563
        
        Increase Number of Blocks Per Set and Decrease Number of Bytes Per Block:
       ./csim 1 256 32 write-allocate write-back lru < gcc.trace
            Load hits: 315140
            Load misses: 3057
            Store hits: 192496
            Store misses: 4990
            Total cycles: 11733413

Set Associative Cache:
    FIFO:
        "Normal" Number of All Variables:
        ./csim 32 16 16 write-allocate write-back fifo < gcc.trace
            Load hits: 313151
            Load misses: 5046
            Store hits: 187761
            Store misses: 9725
            Total cycles: 11201053

        Increased Number of Sets and Decrease Number of Blocks:
        ./csim 64 8 16 write-allocate write-back fifo < gcc.trace
            Load hits: 313096
            Load misses: 5101
            Store hits: 187747
            Store misses: 9739
            Total cycles: 11236323
        
        Increased Number of Sets and Decrease Number of Bytes:
        ./csim 64 16 8 write-allocate write-back fifo < gcc.trace
            Load hits: 311592
            Load misses: 6605
            Store hits: 178884
            Store misses: 18602
            Total cycles: 10233373

        Increased Number of Blocks Per Set and Decrease Number of Sets:
        ./csim 16 32 16 write-allocate write-back fifo < gcc.trace
            Load hits: 313208
            Load misses: 4989
            Store hits: 187743
            Store misses: 9743
            Total cycles: 11183883

        Increased Number of Blocks Per Set and Decrease Number of Bytes:
        ./csim 32 32 8 write-allocate write-back fifo < gcc.trace
            Load hits: 311654
            Load misses: 6543
            Store hits: 178883
            Store misses: 18603
            Total cycles: 10215943
        
        Increased Bytes per Block and Decrease Number of Sets:
        ./csim 16 16 32 write-allocate write-back fifo < gcc.trace
            Load hits: 314008
            Load misses: 4189
            Store hits: 192229
            Store misses: 5257
            Total cycles: 13404183

        Increased Bytes per Block and Decrease Number of Blocks:
        ./csim 32 8 32 write-allocate write-back fifo < gcc.trace
            Load hits: 313929
            Load misses: 4268
            Store hits: 192244
            Store misses: 5242
            Total cycles: 13462903


    LRU:
        "Normal" Number of All Variables:
        ./csim 32 16 16 write-allocate write-back lru < gcc.trace
            Load hits: 314233
            Load misses: 3964
            Store hits: 188076
            Store misses: 9410
            Total cycles: 10301143
        
        Increased Number of Sets and Decrease Number of Blocks:
        ./csim 64 8 16 write-allocate write-back lru < gcc.trace
            Load hits: 314221
            Load misses: 3976
            Store hits: 188067
            Store misses: 9419
            Total cycles: 10313773
        
        Increased Number of Sets and Decrease Number of Bytes:
        ./csim 64 16 8 write-allocate write-back lru < gcc.trace
            Load hits: 312816
            Load misses: 5381
            Store hits: 179431
            Store misses: 18055
            Total cycles: 9616043

        Increased Number of Blocks Per Set and Decrease Number of Sets:
        ./csim 16 32 16 write-allocate write-back fifo < gcc.trace
            Load hits: 314260
            Load misses: 3937
            Store hits: 188072
            Store misses: 9414
            Total cycles: 10293253

        Increased Number of Blocks Per Set and Decrease Number of Bytes:
        ./csim 32 32 8 write-allocate write-back lru < gcc.trace
            Load hits: 312842
            Load misses: 5355
            Store hits: 179438
            Store misses: 18048
            Total cycles: 9606853

        Increased Bytes per Block and Decrease Number of Sets:
        ./csim 16 16 32 write-allocate write-back lru < gcc.trace
            Load hits: 315083
            Load misses: 3114
            Store hits: 192482
            Store misses: 5004
            Total cycles: 11819543

        Increased Bytes per Block and Decrease Number of Blocks:
        ./csim 32 8 32 write-allocate write-back lru < gcc.trace
            Load hits: 315006
            Load misses: 3191
            Store hits: 192487
            Store misses: 4999
            Total cycles: 11883303
        


"Best" Currently:
    Set Associative Cache:
        LRU:
            Increased Number of Blocks Per Set and Decrease Number of Bytes:
            ./csim 32 32 8 write-allocate write-back lru < gcc.trace
                Load hits: 312842
                Load misses: 5355
                Store hits: 179438
                Store misses: 18048
                Total cycles: 9606853

Now we test between the three options between the types of writing:
    ./csim 32 32 8 write-allocate write-through lru < gcc.trace
        Load hits: 312842
        Load misses: 5355
        Store hits: 179438
        Store misses: 18048
        Total cycles: 25142369
    
    ./csim 32 32 8 no-write-allocate write-through lru < gcc.trace
        Load hits: 308868
        Load misses: 9329
        Store hits: 160748
        Store misses: 36738
        Total cycles: 22254093


From this, we have determined that the best writing orientation is write-allocate and write-back.

LRU:
    Increased Number of Blocks Per Set and Decrease Number of Bytes:
    ./csim 32 32 8 write-allocate write-back lru < gcc.trace
        Load hits: 312842
        Load misses: 5355
        Store hits: 179438
        Store misses: 18048
        Total cycles: 9606853

Following this logic, we will keep decreasing the number of bytes and increasing the number of blocks:
LRU:
    ./csim 32 64 4 write-allocate write-back lru < gcc.trace
        Load hits: 312047
        Load misses: 6150
        Store hits: 169453
        Store misses: 28033
        Total cycles: 7717333

We also notice from above that if we decrease number of sets for increasing number of blocks, then the 
total miss decreases along with the total cycles, so we went to a direct caching with lots of blocks:
./csim 1 2048 4 write-allocate write-back lru < gcc.trace
    Total loads: 318197
    Total stores: 197486
    Load hits: 312374
    Load misses: 5823
    Store hits: 169600
    Store misses: 27886
    Total cycles: 7635213

For part (b), you’ll use the memory traces as well as your simulator to determine which cache configuration has
the best overall effectiveness. You should take a variety of properties into account: hit rates, miss penalties,
total cache size (including overhead), etc. In your README.txt, describe in detail what experiments you ran (and 
why!), what results you got (and how!), and what, in your opinion, is the best cache configuration of them all.

This has the lowest amount of total cycles, which indicates that it has the fewest misses, highest hits, and therefore the most 
optimal as it for the same byte range. My approach to finding the best cache was to divide and conquerer. We tested all the types of caches
that they are and by modifying each variable slightly, we can find the best cache while keeping the memory overall the same. From 
there, we had a rough idea of where the best cache could be. From this, we determined that it is within LRU and also that increasing
the number of bytes, while decreasing the othe variables is the best way to reduce misses and cycles. Then we tested the different 
types of writing of the hits and misses and found out that write-allocate write-back was the very best. Then, from there, we use logic
to set it to the extreme conditions finding them to be more favorable. This is also the cache that had the most total hits and lowest
misses out of all the ones that we tested, which means that out of the ones that we tested, this one is the best for this given number of
bytes. This indicates that fully Associative caches are the best in our simulation (which makes logical sense in the theoritical realm).
Also note, that I did test other byte configurations and they returned the same results, but if the blocks is at the maximun it can be, 
then increasing sets is more effective than increasing the number of bytes per block. In conclusion, the best cache configuration
that we got for 2^13 bytes of parameters, was a fully Associative cache with number of bytes set to the lowest.