import math

file=open("test.txt","w")
file.write ("My test)
file.close()

with open("test.txt", "r") as fh3:
            print(fh3.read()) #automatically closes file when use 'with'

try:
            open("randomfile.text")
except FileNotFoundError:
            print("file not existed")
else:
            print("it does exist")


angle = 60
numbers = [1,2,3,4,5]

number_input = input("type ")

if number_input == "":
    print("nothing")
else:
    numbers.append(int(number_input))
    print(str(numbers))

pet_names = {}
pet_names["cat"] = "kitty" #the index is 'cat'
pet_names[1]="fish" #defines 1 as fish

things = set()
things.add(1)
things.add(2)
print(str(1 in things))
things2=set()
things2.add(2)
things2.add(3)

things|things2 #combines the sets but without duplicates

print(things & things2) #prints common items in things and things2

print (" i HAE {} APPLES".format(4))
print("I have {4} apples")


#classes
#dictionaries
#sets
#open and close file
#try and except for errors



