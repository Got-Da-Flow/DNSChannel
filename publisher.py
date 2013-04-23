#!/usr/bin/env python
import sys
from scapy.all import *
print "*** DNSChannel 0.1 ***"
print "You're the publisher, write your message :"
msg = sys.stdin.readline()
msg = "@" + msg.strip() + "@"
send(IP(src=sys.argv[1], dst=sys.argv[3])/UDP(sport=int(sys.argv[2]))/DNS(id=0x4000, rd=1, qd=DNSQR(qclass=1, qtype=2)))
for c in msg:
	send(IP(src=sys.argv[1], dst=sys.argv[3])/UDP(sport=int(sys.argv[2]))/DNS(id=ord(c), rd=1, qd=DNSQR(qclass=1, qtype=2)))
send(IP(src=sys.argv[1], dst=sys.argv[3])/UDP(sport=int(sys.argv[2]))/DNS(id=0x4000, rd=1, qd=DNSQR(qclass=1, qtype=2)))
print "Message sent!"
