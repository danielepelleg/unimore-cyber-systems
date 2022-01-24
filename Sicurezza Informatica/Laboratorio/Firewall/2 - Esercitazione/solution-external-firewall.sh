#!/bin/bash

# Flush filter and nat tables, then zero their pkt and byte counters
iptables -F
iptables -t nat -F
iptables -Z
iptables -t nat -Z

# /etc/hosts Configuration:
# 192.168.2.1   local1 
# 155.185.1.1   ext-firewall
# 192.168.1.1   www 
# 192.168.1.2   ftp   

#############
# nat table #
#############
# Allow Internet to access DMZ's www with Public IP
iptables -t nat -A PREROUTING -p tcp --dport www -i eth1 -d ext-firewall -j DNAT --to-destination 192.168.1.1
# Allow Internet to access DMZ's ftp with Public IP
iptables -t nat -A PREROUTING -p tcp --dport ftp -i eth1 -d ext-firewall -j DNAT --to-destination 192.168.1.2
# Allow DMZ's www to access Internet with Public IP
iptables -t nat -A POSTROUTING -p tcp --dport www -o eth1 -s www -j MASQUERADE

################
# filter table #
################
# Policies
iptables -P FORWARD DROP  
iptables -P INPUT DROP
iptables -P OUTPUT DROP
# Allow local1 to ssh to this machine
iptables -A INPUT -p tcp --dport ssh -i eth0 -s local1 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p tcp --sport ssh -o eth0 -d local1 -m state --state ESTABLISHED -j ACCEPT
# Allow Internet to Access www
iptables -A FORWARD -p tcp --dport www -i eth1 -o eth0 -d www -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport www -i eth0 -o eth1 -s www -m state --state ESTABLISHED -j ACCEPT
# Allow Internet to Access ftp
iptables -A FORWARD -p tcp --dport ftp -i eth1 -o eth0 -d ftp -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ftp -i eth0 -o eth1 -s ftp -m state --state ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --dport ftp-data -i eth1 -o eth0 -d ftp -m state --state ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ftp-data -i eth0 -o eth1 -s ftp -m state --state RELATED,ESTABLISHED -j ACCEPT
# ------
# Allow DMZ's www to access Internet
iptables -A FORWARD -p tcp --dport www -i eth0 -o eth1 -s www -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport www -i eth1 -o eth0 -d www -m state --state ESTABLISHED -j ACCEPT