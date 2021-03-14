ALL: 
	@gcc -o pager.out pager.c
	@gcc -o wordsearch.out wordsearch.c
	@gcc -o wordgen.out wordgen.c
	gcc -o launcher.out launcher.c

.PHONY:debug,clean
debug:
	@gcc -o pagerdebug  pager.c -g
	@gcc -o wordsearchdebug  wordsearch.c -g
	@gcc -o wordgendebug  wordgen.c -g
	@gcc -o launcherdebug launcher.c -g


clean:
	rm -f *.dSYM
	rm wordgen.out
	rm wordsearch.out
	rm pager.out
	rm launcher.out
