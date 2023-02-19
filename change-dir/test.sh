#! /bin/bash
a="2432"
if [[ "$a" =~ ^[0-9]+$ ]]; then
    echo "is the digit"
else
    echo "not the digit"
fi
