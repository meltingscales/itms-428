alphalow = "abcdefghijklmnopqrstuvwxyz"
alphacap = alphalow.upper()
alpha = alphalow + alphacap
secret_code = "-squish"

try:
	input = raw_input #Py2 compat
except NameError:
	pass

def transform_word(word):
	return word+secret_code

print("INSECURE HACKED message-transformation program.")
print(";)")

message = input("Enter message:\n > ")

splitmsg = message.split(' ')

output = ""

for word in splitmsg:
	if word[0] in alpha:
		output += transform_word(word) + " "
	else:
		output += word
		
print(output)