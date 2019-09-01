'''
Created on 26.07.2019

@author: localo
'''
from struct import *
import ice
from dpkt.pcap import Reader
from dpkt.ethernet import Ethernet

def to_bin(a):
    return " ".join(map(lambda x: "".join([str((x>>(7-i))&1) for i in range(8)]),a))

def bit_shift_array_right(a,n):
    if n%8==0:
        return a
    r = []
    l = len(a)
    idx = n
    dw = 0
    for i in range(l):
        b=a[i]
        for j in range(8):
            if idx %8==0:
                r.append(dw)
                dw =0
                idx = 0
            v = (b>>(7-j))&1
            dw=dw|(v<<(7-idx))
            idx+=1
    return r

def consume(p,key,cmp):
    data = ice.decrypt(p, key)
    for i,c in enumerate(cmp):
        n = data.find(c)
        if n>0:
            result = "".join([chr(x) for x in bit_shift_array_right(data,8-i)][n:])
            print((result.split("}",1)[0]+'}'))
            return True
    return False
            
    
def main(f,k):
    print("[*] generating ICE key for build: %d"%(k))
    key = b'CSGO'
    key += bytes([((k)&0xff)])
    key += bytes([((k>>8)&0xff)])
    key += bytes([((k>>16)&0xff)])
    key += bytes([((k>>24)&0xff)])
    key += bytes([((k>>2)&0xff)])
    key += bytes([((k>>10)&0xff)])
    key += bytes([((k>>18)&0xff)])
    key += bytes([((k>>26)&0xff)])
    key += bytes([((k>>4)&0xff)])
    key += bytes([((k>>12)&0xff)])
    key += bytes([((k>>20)&0xff)])
    key += bytes([((k>>28)&0xff)])
    print("[*] done")
        
    print("[*] generating pattern")
    cmp = []
    test = [ord(x) for x in 'ALLES{']
    for _ in range(8):
        cmp.append(bytes(test[1:-1]))
        test = bit_shift_array_right(test, 1)
    print("[*] reading packets")
    with open(f,"rb") as f_:
        for _, pkt in Reader(f_):
            eth = Ethernet(pkt)
            ip = eth.data
            udp = ip.data
            payload = udp.data
            if consume(payload, key, cmp):
                break
    print("[*] done")

if __name__ == '__main__':
    #version can be guessed by looking at the newest version and decrementing or by using steam depots
    #this is the simplest way I know to solve this challenge as it does not require to implement any protocol layers
    main('31_07_2019.pcap',13707)
