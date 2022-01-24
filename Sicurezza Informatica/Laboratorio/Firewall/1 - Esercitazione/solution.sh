#!/bin/bash

# Flush filter and nat tables, then zero their pkt and byte counters
iptables -F
iptables -t nat -F
iptables -Z
iptables -t nat -Z

# /etc/hosts Configuration:
# 192.168.1.1	local1
# 192.168.1.2	local2
# 155.185.1.1	www.fake.com
# 155.185.1.2	ftp.fake.com
# 11.22.33.211	remote1

#############
# nat table #
#############
# Allow LAN to access Internet with Public IP
iptables -t nat -A POSTROUTING -o eth2 -s 192.168.1.0/24 -j MASQUERADE
# Allow remote1 to ssh into local1
iptables -t nat -A PREROUTING -p tcp --dport ssh -i eth2 -s remote1 -d 155.185.1.9 -j DNAT --to-destination 192.168.1.1 

################
# filter table #
################
# Policies
iptables -P FORWARD DROP  
iptables -P INPUT DROP
# Allow local2 to ssh to this machine
iptables -A INPUT -p tcp --dport ssh -i eth1 -s local2 -m state --state NEW,ESTABLISHED -j ACCEPT
# Allow Internet to access www.fake.com
iptables -A FORWARD -p tcp --dport www -i eth2 -o eth0 -d www.fake.com -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport www -i eth0 -o eth2 -s www.fake.com -m state --state ESTABLISHED -j ACCEPT
# Allow local machines to access www.fake.com
iptables -A FORWARD -p tcp --dport www -i eth1 -o eth0 -d www.fake.com -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport www -i eth0 -o eth1 -s www.fake.com -m state --state ESTABLISHED -j ACCEPT
# Allow Internet to access ftp.fake.com
iptables -A FORWARD -p tcp --dport ftp -i eth2 -o eth0 -d ftp.fake.com -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ftp -i eth0 -o eth2 -s ftp.fake.com -m state --state ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --dport ftp-data -i eth2 -o eth0 -d ftp.fake.com -m state --state ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ftp-data -i eth0 -o eth2 -s ftp.fake.com -m state --state RELATED,ESTABLISHED -j ACCEPT
# Allow local machines to access ftp.fake.com
iptables -A FORWARD -p tcp --dport ftp -i eth1 -o eth0 -d ftp.fake.com -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ftp -i eth0 -o eth1 -s ftp.fake.com -m state --state ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --dport ftp-data -i eth1 -o eth0 -d ftp.fake.com -m state --state ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ftp-data -i eth0 -o eth1 -s ftp.fake.com -m state --state RELATED,ESTABLISHED -j ACCEPT
# Allow LAN to access Internet
iptables -A FORWARD -i eth1 -o eth2 -s 192.168.1.0/24 -j ACCEPT
iptables -A FORWARD -i eth2 -o eth1 -d 192.168.1.0/24 -m state --state ESTABLISHED,RELATED -j ACCEPT
# Allow remote1 to ssh into local2
iptables -A FORWARD -p tcp --dport ssh -i eth2 -o eth1 -s remote1 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --dport ssh -i eth1 -o eth2 -d remote1 -m state --state ESTABLISHED -j ACCEPT