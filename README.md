# Order-Management-System

A computer-aided system designed to take orders from customers, process orders chronologically and display the receipt for the processed order. 

Upon launching, the system loads the menu from the 'menu.txt' file. This file contains the item code (letter + number, e.g. L1, B2, D1, ...), the item name, and the cost (e.g. $1.99), all separated by a comma (,). 

To input an order, the user inputs the item codes in sequence, no separation, and no repetitions (e.g. "L1L3L4D1D3B1B2"). Then inputs the number of each item ordered, separated by comma (e.g. "1,1,2,1,1,4,2"). 

When processing an order, the system takes the oldest order stored in the system and converts it into a receipt that is shown in the console.

This project utilizes an ideal data structure (i.e. queue, because first-in-first-out (FIFO) order), and string (char*) properties and functions.
