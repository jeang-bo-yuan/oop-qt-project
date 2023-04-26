#!/usr/bin/sh

case $1 in
	"--dot")
		# for Include_Path.svg
		cinclude2dot --src=.. --quotetypes=quote > Include_Path.dot
		dot Include_Path.dot -Tsvg -o Include_Path.svg  # dot is from graphviz
		;;
	"-h"|"--help")
		echo "Usage:"
		echo "	./DocGen.sh [--dot]"
		exit 0
		;;
esac

# for .chm
[ -d html ] && rm -r html
doxygen
hhc html/index.hhp # hhc is from HTML Help Workshop
mv html/index.chm Document.chm
