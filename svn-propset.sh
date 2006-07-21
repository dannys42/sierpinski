#!/bin/sh
FILES=`find . -iname "*.[ch]" -o -iname "*.cpp"`
svn ps svn:eol-style native ${FILES}
