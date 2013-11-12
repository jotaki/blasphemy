#! /bin/bash

tmpfile=$(mktemp)

cat <<EOF > "$tmpfile"
<html><body bgcolor="#${1:-000000}"></body></html>
EOF

${browser:-firefox} "$tmpfile"
