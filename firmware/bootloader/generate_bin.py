#/usr/bin/env python
import sys

def create(sz = 51200):
    i =0
    with open("MOCKUP.bin", "wb") as f:
        f.write(i.to_bytes(4, "little")) # first 4 bytes is startup code, set to zero
        while sz > 0:
            sz -= 1
            f.write(i.to_bytes(1, "little"))
            i += 1
            if (i > 0x10):
                i = 0
        print("generated MOCKUP.bin")
                
                
                
if (__name__ == "__main__"):
    if (len(sys.argv) == 2):
       try:
            sz = int(sys.argv[1])
            create(sz)
       except:
            create()
    else:
        create()
