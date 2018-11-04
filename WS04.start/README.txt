Bosen Yang by570


####################################################################################################
compiling rules START
####################################################################################################

make: running all sources files, i.e. files for all questions

make sleep: Q1
make wave: Q2
make barrier: Q3
make wait: Q4

make clean: remove all files in bin/, src/ and lib/ for a fresh restart

####################################################################################################
compiling rules END
####################################################################################################



####################################################################################################
files description START
####################################################################################################

bin/sleep: executable file for Q1
bin/wave: executable file for Q2
bin/barrier: executable file for Q3
bin/wait: executable file for Q4

src/sleep: source file for Q1
src/wave: source file for Q2
src/barrier: source file for Q3
src/wait: source file for Q4

opf/sleep: output file for Q1
opf/wave: output file for Q2
opf/barrier: output file for Q3
opf/wait: output file for Q4

./makefile: contains the compilation instructions all questions

####################################################################################################
files description END
####################################################################################################





####################################################################################################
Q1 COMMENTS START
####################################################################################################

Confuseing question.

####################################################################################################
Q1 COMMENTS END
####################################################################################################





####################################################################################################
Q2 COMMENTS START
####################################################################################################

Easy.

####################################################################################################
Q2 COMMENTS END
####################################################################################################





####################################################################################################
Q3 COMMENTS START
####################################################################################################

Q3.1:	the minimum number of signal emissions needed is 4.
		The parent pauses for two signals from children after executing calc1,
		and the send one signal to each child telling them to stop pausing and tun calc2.
Q3.2:	No, we can't implement with SIGUSR1 only.
		In this program there's definitely one process that receives 2 signals, 
		each from one of the other two processes.
		If we only have SIGUSR1, because one signal can hide another,
		one out of two SIGUSR1's will be discarded and thus cause some information to be lost.

####################################################################################################
Q3 COMMENTS END
####################################################################################################





####################################################################################################
Q4 COMMENTS START
####################################################################################################



####################################################################################################
Q4 COMMENTS END
####################################################################################################

