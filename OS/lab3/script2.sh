awk '{if (NR % 2 == 0) print($0)}' text | tr [лйцукенгшщзхъфывапроджэячсмитьбюa-z»] [ЛЙЦУКЕНГШЩЗХЪФЫВАПРОДЖЭЯЧСМИТЬБЮA-Z»] > text2
awk '{if (NR % 2 != 0) print($0)}' text | sed 's/!/!!/g' | tr -d '.' >> text2
awk '{if (NR % 2 != 0) print($0)}' text | sed 's/!/!!/g' | tr -cd '.' | wc -c >> text2
