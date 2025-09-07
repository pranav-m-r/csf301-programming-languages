1. Can you hide the ("private") details of a data structure from its programmer consumers? Which programming languages can you demonstrate this in? Hint: Study the FILE type in stdio.h in C.
2. Can you name the functions and variables in a QuickSort implementation (in any programming language of your choice) such that just by reading your program (or just the API calls syntax) the reader can infer that
   1. It does in-place sorting
   2. It does (or it does not) perform a stable sort. (Note: There can be stable implementations of QuickSort.)
3. Implement QuickSort in Python. Can you make it do a stable sort?
4. Can you untangle the following to make a program without gotos?
   L1: x=10;
   L2: if(x>0)
   L3: if(x--) goto L1;
   L4: else if(x++ <= 10) goto L2;
   L5: else if(x--) goto L4;
   L6: else x++;
