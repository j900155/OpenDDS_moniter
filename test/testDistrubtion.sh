#! /bin/bash
#
# testDistrubtion.sh
# Copyright (C) 2019 zack <zack@zack>
#
# Distributed under terms of the MIT license.
#

allTopic=("A" "B" "C")
curl 0.0.0.0:5000/runTopic
echo ""
curl 0.0.0.0:5000/getNode
echo ""
for i in ${allTopic[@]}
do
	echo ${i}
	curl 0.0.0.0:5000/newTopic/${i}
	echo ""
done
curl 0.0.0.0:5000/runTopic
curl 0.0.0.0:5000/updateDistrubtion
