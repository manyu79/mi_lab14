#!/bin/bash
#check for flags:
for var in $@
do 

  if [ $var == '-h' ]; then
	echo ''
	echo 'How to Call:'
	echo './submitCode.sh <flags>'
	echo ''
	echo 'Script to submit code for 2680.  Flags:'
	echo '   -h: help'
	echo '   -tar_only: only tar the file, do not submit'
	echo ''
	echo 'Requirements: You must have an internet connection '
	echo 'to submit over scp. You must run this script within '
	echo 'a moos-ivp-extend directory.  Your directory must   '
	echo 'contain a missions folder, a src folder, src/CMakeLists.txt'
	echo 'and a build.sh script.  '
	echo ''
	echo 'Function: submitCode.py checks for correct structure, '
	echo 'then creats a tar.gz file without executables, temporary files, or logs.'
	echo 'It then submits the result to student@128.30.31.10: via scp'
	echo ''
  fi
done
python submitCode.py $var


