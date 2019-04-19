#!/bin/bash
for filename in `find . | egrep '\.gcno'`;
do
  sed -i 's/\/Projects\/CMake\/Build\/include\/MDP/\/Messenger\/Include/g' $filename;
done