

Suduku Solver using Sat Solve sat4j

submitters:
Gilad Eini, 	giladeini@gmail.com		,id 034744920
Vadim Khakham, 	vadim.khakham@gmail.com	,id 311890156



1.to load your own board: put the board in the same folder as the soduku.exe folder 
and type the name of your file into the custom field in DBoptions group box.

2.validation board: some of the DB boards has a validation file.
its purpose is to compare a valid result to the result of our program.
to see that we don't use it for the solution, change a any digit in the 
validation, and see that its not a PERFECT MATCH.
this file is optional and ment to save time in the check of our programs solution.

3.we made some optimizations depending on the board given.
in the text box in the GUI you can see the explanation.
for the board you supplied us we made 7425 clause.

4.we didn't check the validity of the the input.
it's the user's responsibility to supply a valid board in a text file.

5.don't remove anything from sodukoExe folder. all files there are needed for the 
program.(except of the board txt files. the program don't need it.)

6.added an empty board (board4.txt) to illustrate that without givens the 
clauses number will be 11745 just like the article, and to show sat solver will 
return the first solution it will find(regardless to how many exist).