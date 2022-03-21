num=$(cat text | tr '\n' ' ' | tr -cd '.' | wc -m)
if [[ $[$num - $num/6*6] == 0 ]]; then
	num=$[$num/6];
else
	num=$[$num/5];
fi
cat text | tr '\n' ' '| sed -r 's/(([^.!?]+[.!?]){'$num'})(\s*)/\1\n/g' > text1
