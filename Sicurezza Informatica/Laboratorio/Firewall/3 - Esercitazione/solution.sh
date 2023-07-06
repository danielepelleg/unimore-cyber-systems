#!/bin/bash

# /etc/hosts Configuration:
# 11.22.33.33     www.local.it
# 11.22.33.34     ftp.local.it
# 155.185.2.21    www.unimore.it
# 11.11.11.11     remote1
# 192.168.100.11	local1
# 192.168.100.22	local2
# 11.22.33.17     this-firewall   

# Flush filter and nat tables, then zero their pkt and byte counters
iptables -F
iptables -t nat -F
iptables -Z
iptables -t nat -Z

#############
# nat table #
#############
# Allow LAN to access Internet sharing a Public IP
iptables -t nat -A POSTROUTING -o eth0 -s 192.168.100.0/24 -j MASQUERADE
# Allow remote1 to ssh local2 on port 2222 using it's Public IP
iptables -t nat -A PREROUTING -p tcp --dport 2222 -i eth0 -d this-firewall -j DNAT --to-destination 192.168.100.11:22

################
# filter table #
################
# Policies
iptables -P FORWARD DROP  
iptables -P INPUT DROP
iptables -P OUTPUT DROP
# Allow local2 to ssh this machine (firewall)
iptables -A INPUT -p tcp --dport ssh -i eth2 -s local1 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p tcp --sport ssh -o eth2 -d local1 -m state --state ESTABLISHED -j ACCEPT
# Allow Internet to connect to www.local.it
iptables -A FORWARD -p tcp --dport www -i eth0 -o eth1 -d www.local.it -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport www -o eth0 -i eth1 -s www.local.it -m state --state ESTABLISHED -j ACCEPT
# Allow local2 to connect to www.local.it
iptables -A FORWARD -p tcp --dport www -i eth2 -o eth1 -s local2 -d www.local.it -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport www -i eth1 -o eth2 -d local2 -s www.local.it -m state --state ESTABLISHED -j ACCEPT
# Allow Internet to connect to ftp.local.it
iptables -A FORWARD -p tcp --dport ftp -i eth0 -o eth1 -d ftp.local.it -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ftp -o eth0 -i eth1 -s ftp.local.it -m state --state ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --dport ftp-data -i eth0 -o eth1 -d ftp.local.it -m state --state ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ftp-data -o eth0 -i eth1 -s ftp.local.it -m state --state RELATED,ESTABLISHED -j ACCEPT
# Allow local2 to connect to ftp.local.it
iptables -A FORWARD -p tcp --dport ftp -i eth2 -o eth1 -s local2 -d ftp.local.it -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ftp -o eth2 -i eth1 -d local2 -s ftp.local.it -m state --state ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --dport ftp-data -i eth2 -o eth1 -s local2 -d ftp.local.it -m state --state ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ftp-data -o eth2 -i eth1 -d local2 -s ftp.local.it -m state --state RELATED,ESTABLISHED -j ACCEPT
# Allow LAN to access Internet
iptables -A FORWARD -i eth2 -o eth0 -s 192.168.100.0/24 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -o eth2 -i eth0 -d 192.168.100.0/24 -m state --state RELATED,ESTABLISHED -j ACCEPT
# Allow remote1 to ssh into local1
iptables -A FORWARD -p tcp --dport ssh -i eth0 -o eth2 -s remote1 -d local2 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ssh -o eth0 -i eth2 -d remote1 -s local2 -m state --state ESTABLISHED -j ACCEPT
