all files must be in same directory (or at least each .c file needs its own copy of my .h, since they both #include it, and its own copy of candidates.txt, since they both read it).

to compile & run server (on Cayenne):
gcc -o serv serv.c -lpthread
./serv

to compile & run client (on Cinnamon):
gcc -o cli cli.c
./cli

sites I used for code references:
http://www.tutorialspoint.com/unix_sockets/socket_server_example.htm
http://www.tenouk.com/Module40c.html
http://beej.us/guide/bgnet/output/html/multipage/gethostbynameman.html
http://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
