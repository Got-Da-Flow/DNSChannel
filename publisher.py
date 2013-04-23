#!/usr/bin/env python
import sys
from scapy.all import *

if len(sys.argv) < 4:
	print "Usage: publisher.py dst_ip dst_port dns_server_ip"
	sys.exit(0)

# DA magic function! (spoof IP.src/UDP.sport)
def scapySend(c):
	send(IP(src=sys.argv[1], dst=sys.argv[3])/UDP(sport=int(sys.argv[2]))/DNS(id=c, rd=1, qd=DNSQR(qclass=1, qtype=2)))

print "*** DNSChannel 0.1 - Publisher ***"
print "You're the publisher, write your message :"
msg = sys.stdin.readline()
msg = "@" + msg.strip() + "@"
# Send begin split
scapySend(0x4000)
# Send ASCII code of chars
for c in msg:
	scapySend(ord(c))
# Send end split
scapySend(0x4000)
print "Message sent to " + sys.argv[1] + ":" + sys.argv[2] + " through " + sys.argv[3] + "!"
