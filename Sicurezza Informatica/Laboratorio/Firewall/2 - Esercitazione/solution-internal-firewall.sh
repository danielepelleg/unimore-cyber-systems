#!/bin/bash

# Flush filter and nat tables, then zero their pkt and byte counters
iptables -F
iptables -t nat -F
iptables -Z
iptables -t nat -Z

# /etc/hosts Configuration:
# 192.168.2.1   local1
# 192.168.1.254 ext-firewall-int
# 192.168.1.1   www
# 192.168.1.2   ftp
# 192.168.2.1   local1
# 11.22.33.211  remote1

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
# Allow local1 to ssh to external firewall machine
iptables -A FORWARD -p tcp --dport ssh -i eth0 -o eth1 -s local1 -d ext-firewall-int -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ssh -i eth1 -o eth0 -s ext-firewall-int -d local1 -m state --state NEW,ESTABLISHED -j ACCEPT
# Allow LAN to access DMZ's www
iptables -A FORWARD -p tcp --dport www -i eth0 -o eth1 -s 192.168.2.0/24 -d www -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport www -i eth1 -o eth0 -s www -d 192.168.2.0/24 -m state --state ESTABLISHED -j ACCEPT
# Allow LAN to access DMZ's ftp
iptables -A FORWARD -p tcp --dport ftp -i eth0 -o eth1 -d ftp -s 192.168.2.0/24 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ftp -i eth1 -o eth0 -s ftp -d 192.168.2.0/24 -m state --state ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --dport ftp-data -i eth0 -o eth1 -d ftp -s 192.168.2.0/24 -m state --state ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport ftp-data -i eth1 -o eth0 -s ftp -d 192.168.2.0/24 -m state --state RELATED,ESTABLISHED -j ACCEPT
# ------
# Allow LAN to Access DMZ's 8080 port
iptables -A FORWARD -p tcp --dport 8080 -i eth0 -o eth1 -s 192.168.2.0/24 -d www -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A FORWARD -p tcp --sport 8080 -i eth1 -o eth0 -d 192.168.2.0/24 -s www -m state --state ESTABLISHED -j ACCEPT
# local1 : export HTTP_PROXY=http://192.168.1.1:8080/