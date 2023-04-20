#!/usr/bin/sh

# for .chm
[ -d html ] && rm -r html
doxygen
hhc html/index.hhp # hhc is from HTML Help Workshop
mv html/index.chm Document.chm

# for Include_Path.svg
cinclude2dot --src=.. --quotetypes=quote > Include_Path.dot
dot Include_Path.dot -Tsvg -o Include_Path.svg  # dot is from graphviz
