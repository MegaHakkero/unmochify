# unmochify
A MochiCrypt flash payload decryptor. I totally didn't waste my time on this :3

You can't feed the MochiCrypted flash file directly to this program, you need to extract the payload using something like JPEXS/FFDec. The payload is a BinaryData tag with character ID 7.

Works only on unix systems out of the box, also requires zlib installed.

It'll output a flash file ready to be played / edited using ffdec. I personally created and used this program to basically rip the soundtrack of a single flash game lmao

Run the program w/o any arguments to see how to use it, it's really simple ;3

Compile with `g++ -Wall -std=c++11 -lz -Iinclude -o unmochify`
