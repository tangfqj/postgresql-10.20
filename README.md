# PostgreSQL-10.20 with Extended Functions

![build status](https://img.shields.io/badge/build-passing-66c2a5.svg)
[![psql version](https://img.shields.io/badge/PostgreSQL-10.2-fc8d62.svg)](https://www.postgresql.org/)
[![Apache License](https://img.shields.io/badge/license-Apache2.0-8da0cb.svg)](http://www.apache.org/licenses/)

## Motivation & Description
For example, say we have two relations A and B and we want to joint them on the address field. We have address in A like ```123 Durant Avenue Berkeley, CA``` and address in B like ```123 Durant Ave. Berkeley, CA```. In the original postgres implementation, the two addresses will not match in join oepration. However, if the records could be join if the addresses were "similar" enough, that would be useful.

Similarity joins use similarity calculations to determin whether records should be matched. I mainly implement two similarity metrics: the Levenshtein distance and the Jaccard index.

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
