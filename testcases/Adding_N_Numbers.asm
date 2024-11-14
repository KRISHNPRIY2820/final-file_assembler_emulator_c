/*
 * Name: PRIYANSHI AGRAWAL
 * Roll No.: 2301CS90
 * Course Code: CS2102  
 * Work: MiniProject
 * Course Instructor: Dr. Jimson Mathew
 * Declaration of Authorship:
 * I, Priyanshi Agrawal, declare that this code is my own work. I have not copied code from any other source, and I have not shared my code with anyone else.
 */


; Adding N Numbers
start:   ldc count
         call add_numbers
         HALT

add_numbers: adj -2
             ldc 0
             stl 0
             ldl 1
             stl 1
loop:       ldl 1
             brz end
             ldl 0
             ldl 1
             add
             stl 0
             ldl 1
             ldc 1
             sub
             stl 1
             br loop
end:        return

count: data 10
result: data 0
