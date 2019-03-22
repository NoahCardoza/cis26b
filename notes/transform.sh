#!/usr/bin/env bash

echo FS Event '[UPDATED]' `basename $1`

sed 's/```java/```c/' $1 > $1.grip
