# CS-300-DSA-Analysis-and-Design

Portfolio work for CS 300 DSA: Analysis and Design

## Reflection

### What was the problem you were solving in the projects for this course?

The main problem in these projects was creating an advising program for ABC University that could read course information from a file, store the courses, search for a specific course, and print the complete course list in alphanumeric order. The program also needed to display prerequisite information for each course.

### How did you approach the problem? Consider why data structures are important to understand.

I approached the problem by comparing three different data structures: a vector, a hash table, and a binary search tree. I looked at how each structure handled searching, inserting, storing, and printing course information. Understanding data structures is important because the structure you choose can affect how efficiently a program performs. For this project, I chose a binary search tree because it allowed the courses to stay organized by course number and made it possible to print them in order using an in-order traversal.

### How did you overcome any roadblocks you encountered while going through the activities or project?

One roadblock I encountered was making sure the course file was read and validated correctly, especially when working with prerequisites. I also had to make sure invalid input did not cause the program to fail. I worked through these issues by testing the program step by step, checking different menu options, testing invalid course numbers, and making sure the program handled errors correctly. Testing each part separately helped me find and correct problems more easily.

### How has your work on this project expanded your approach to designing software and developing programs?

This project helped me understand the importance of planning the structure of a program before writing the final code. In Project One, I compared different approaches using pseudocode and runtime analysis before deciding which data structure to use in Project Two. This showed me that choosing a data structure should be based on the requirements of the problem instead of just choosing the easiest option to code.

### How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

This project helped me focus more on organizing code into smaller functions and using clear variable and function names. I also added comments to explain what important parts of the program were doing. Breaking the program into separate functions for loading data, searching, printing, and displaying the menu made the code easier to understand and easier to change. I also learned that validating input and handling errors are important parts of writing programs that are reliable and maintainable.
