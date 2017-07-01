#################################################
# Author : Holly Buteau
# Assignment : Program 5 : Python Exploration 
# Date : November 20, 2016
# Purpose : Calling random, creating files and
# reading them out, summing intergers.
#################################################

#!/usr/bin/env python
import random

#this is just to make things look neat
print "\n"
alphabet = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']#this is the alphabet list that ran will reference to 
#add in to files

#creating of first file
f1 = open('file1.txt','w+')
for x in range(10):
	f1.write(random.choice(alphabet))
f1.write("\n")
f1.close()

#creation of second file
f2 = open('file2.txt', 'w+')
for x in range(10):
           f2.write(random.choice(alphabet))
f2.write("\n")
f2.close()

#creation of third file
f3 = open('file3.txt', 'w+')
for x in range(10):
          f3.write(random.choice(alphabet))
f3.write("\n")
f3.close()

#reading out the contents of file 1
print "The contents of file 1 are:"
f1new = open('file1.txt', 'r')
file_contents1 = f1new.read()
print(file_contents1)
f1new.close()

#reading out the contents of file 2
print "The contents of file 2 are:" 
f2new = open('file2.txt', 'r')
file_contents2 = f2new.read()
print(file_contents2)
f2new.close()

#reading out the contents of file 3
print "The contents of file 3 are:"
f3new = open('file3.txt', 'r')
file_contents3 = f3new.read()
print(file_contents3)
f3new.close()

#getting the two random integers
num1 = random.randint(1, 42)
print "The first number is: %s\n" % (num1)

num2 = random.randint(1,42)
print "The second number is: %s\n" % (num2)

#multiplying the two numbers and display the product
print "The product of the two numbers is: %s\n" % (num1 * num2)

