# Postgresql-10.20 with Extended Functions

![build status](https://img.shields.io/badge/build-passing-66c2a5.svg)
[![psql version](https://img.shields.io/badge/PostgreSQL-10.2-fc8d62.svg)](https://www.postgresql.org/)
[![Apache License](https://img.shields.io/badge/license-Apache2.0-8da0cb.svg)](http://www.apache.org/licenses/)

## Description

## Mannual

### How to install
Run the following commands at the root directory of the PostgreSQL package:
```commandline
./configure --enable-depend --enable-cassert --enable-debug CFLAGS="-O0" --prefix=/usr/local/pgsql
make
su
make install
adduser postgres
mkdir /usr/local/pgsql/data
chown postgres /usr/local/pgsql/data
su - postgres
/usr/local/pgsql/bin/initdb -D /usr/local/pgsql/data --locale=C
/usr/local/pgsql/bin/postgres -D /usr/local/pgsql/data -l logfile start
```

### How to Load Dataset
You can load the dataset by the following command, replacing `<PATH>` with the exact path of the dataset:
```commandline
/usr/local/pgsql/bin/psql -f <PATH>
```

### How to Enter PostgreSQL

```commandline
/usr/local/pgsql/bin/psql
```

### How to Enable Timing

Run the following command after you've enter PostgreSQL
```commandline
\timing
```
