#!/bin/bash

while
read file
do
	iconv --from-code=ISO_8859-15  --to-code=UTF-8 $file > $file.new
	mv $file.new $file
done < /tmp/list
