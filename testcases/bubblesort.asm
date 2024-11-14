/*
 * Name: PRIYANSHI AGRAWAL
 * Roll No.: 2301CS90
 * Course Code: CS2102  
 * Work: MiniProject
 * Course Instructor: Dr. Jimson Mathew
 * Declaration of Authorship:
 * I, Priyanshi Agrawal, declare that this code is my own work. I have not copied code from any other source, and I have not shared my code with anyone else.
 */

; Bubble Sort in SIMPLE Assembly
start:   ldc array
         ldc length
         call bubblesort
         HALT

bubblesort: adj -2           ; Create space for local variables
            stl 1            ; Store array base address in local variable 1
            stl 2            ; Store array length in local variable 2
outer:     ldl 2             ; Load length
            ldc 1
            sub
            stl 2
            brz done         ; If length is 1, sorting is done
            ldl 2            ; Load array length - 1
inner:     ldl 1             ; Load array base address
            adc 0            ; Add offset 0
            ldl 1
            adc 1
            add              ; Calculate address of the next element
            ldnl 0           ; Load the next element
            ldl 1            ; Load array base address again
            adc 0            ; Add offset 0
            ldnl 0           ; Load the current element
            sub
            brz skip
            ldl 1
            adc 0
            ldl 1
            adc 1
            add
            ldl 0
            stnl 0
            ldl 1
            adc 0
            ldnl 0
            stnl 1
skip:      ldl 1
            adc 1
            stl 1
            ldl 2
            adc -1
            brz outer
            br inner
done:      return

length:  data 5              ; Array length
array:   data 8, 3, 7, 4, 1  ; Array to be sorted
