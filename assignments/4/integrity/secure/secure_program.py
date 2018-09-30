alphalow = "abcdefghijklmnopqrstuvwxyz"
alphacap = alphalow.upper()
alpha = alphalow + alphacap
secret_code = "ay"

try:
	input = raw_input #Py2 compat
except NameError:
	pass

def transform_word(word):
	return word[1:]+word[0]+"-"+secret_code

print("Secure message-transformation program.")

message = input("Enter message:\n > ")

splitmsg = message.split(' ')

output = ""

for word in splitmsg:
	if word[0] in alpha:
		output += transform_word(word) + " "
	else:
		output += word
		
print(output)