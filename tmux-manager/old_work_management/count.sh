#! /bin/bash
file_id=1
line_counter=0
echo "-------------------Project files --------------------"
for cpp_file in *.cpp *.h */*.py *.sh
do
    lines=$(wc -l $cpp_file)
    lines=${lines%% *}
    echo "$file_id. $cpp_file : $lines"
    let line_counter+=lines
    let file_id+=1
done
echo "-> total lines are the: ${line_counter}"
echo "----------------------------------------"
