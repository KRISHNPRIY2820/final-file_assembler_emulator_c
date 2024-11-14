/*
 * Name: PRIYANSHI AGRAWAL
 * Roll No.: 2301CS90
 * Course Code: CS2102  
 * Work: MiniProject
 * Course Instructor: Dr. Jimson Mathew
 * Declaration of Authorship:
 * I, Priyanshi Agrawal, declare that this code is my own work. I have not copied code from any other source, and I have not shared my code with anyone else.
 */


; Median Calculation (assuming array is sorted)
start:   ldc array
         ldc length
         call median
         HALT

median:  adj -2
         stl 1
         stl 2
         ldl 2
         adc -1
         shr
         adc 1
         ldl 1
         add
         ldnl 0
         stl 0
         return

length:  data 5
array:   data 1, 3, 4, 7, 8
result:  data 0
