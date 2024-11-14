/*
 * Name: PRIYANSHI AGRAWAL
 * Roll No.: 2301CS90
 * Course Code: CS2102  
 * Work: MiniProject
 * Course Instructor: Dr. Jimson Mathew
 * Declaration of Authorship:
 * I, Priyanshi Agrawal, declare that this code is my own work. I have not copied code from any other source, and I have not shared my code with anyone else.
 */



; Mean Calculation
start:   ldc array
         ldc length
         call mean
         HALT

mean:    adj -2
         stl 1
         stl 2
         ldc 0
         stl 0
loop:    ldl 1
         ldl 2
         sub
         brz end
         ldl 1
         ldnl 0
         ldl 0
         add
         stl 0
         ldl 1
         adc 1
         stl 1
         br loop
end:     ldl 0
         ldl 2
         div
         stl 0
         return

length:  data 58
array:   data 8, 3, 7, 4, 1
result:  data 0
