/*
 * Name: PRIYANSHI AGRAWAL
 * Roll No.: 2301CS90
 * Course Code: CS2102  
 * Work: MiniProject
 * Course Instructor: Dr. Jimson Mathew
 * Declaration of Authorship:
 * I, Priyanshi Agrawal, declare that this code is my own work. I have not copied code from any other source, and I have not shared my code with anyone else.
 */


; Compute P(x) = 3x^2 + 2x + 1
start:   ldc x
         ldnl 0
         ldc 3
         call poly
         HALT

poly:    adj -4
         stl 1
         stl 2
         stl 3
         stl 4
         ldc 1
         stl 0
         ldl 1
         ldc 2
         mul
         ldl 2
         add
         ldl 0
         ldl 1
         mul
         ldl 2
         add
         ldl 0
         ldc 3
         mul
         ldl 3
         add
         ldl 0
         ldl 4
         return

x:      data 2
result: data 0
