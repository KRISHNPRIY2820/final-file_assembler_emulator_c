/*
 * Name: PRIYANSHI AGRAWAL
 * Roll No.: 2301CS90
 * Course Code: CS2102  
 * Work: MiniProject
 * Course Instructor: Dr. Jimson Mathew
 * Declaration of Authorship:
 * I, Priyanshi Agrawal, declare that this code is my own work. I have not copied code from any other source, and I have not shared my code with anyone else.
 */


; Standard Deviation Calculation
start:   ldc array
         ldc length
         call std_dev
         HALT

std_dev: adj -2
         stl 1
         stl 2
         ldc 0
         stl 0
mean_loop:
         ldl 1
         ldl 2
         sub
         brz mean_end
         ldl 1
         ldnl 0
         ldl 0
         add
         stl 0
         ldl 1
         adc 1
         stl 1
         br mean_loop
mean_end:
         ldl 0
         ldl 2
         div
         stl 0
         ldc 0
         stl 1
var_loop:
         ldl 1
         ldl 2
         sub
         brz var_end
         ldl 1
         ldnl 0
         ldl 0
         sub
         mul
         ldl 1
         add
         stl 1
         ldl 1
         adc 1
         stl 1
         br var_loop
var_end:
         ldl 1
         ldl 2
         sub
         shr
         ldc 1
         adc
         ldl 2
         adc -1
         div
         stl 0
         return

length:  data 5
array:   data 8, 3, 7, 4, 1
mean:    data 0
result