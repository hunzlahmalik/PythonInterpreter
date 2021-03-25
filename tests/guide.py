# don't use the function name as the parameters

x=lambda a:1 if a>10 else 0

a=10
b=30

add=lambda x, y: x+y #don't do this. Beacuse the variable 'x' is function
add=lambda a, b: a+b #can do this. Beacuse the variable 'a' and 'b' aren't function

c=add(3,1)


