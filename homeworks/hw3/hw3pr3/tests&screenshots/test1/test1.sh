#Change the line below to that it points to your shell
#!./msh
echo TESTINGTESTING123 >testfile.txt
cat <testfile.txt >testfile2.txt
ls -l
#You should see testfile.out and testfile2.out both with size 18
exit 17
#After you invoke this script from the "real" shell, echo $? and
#verify the exit status is 17
