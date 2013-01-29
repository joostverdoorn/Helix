import serial
ser = serial.Serial('/dev/ttyACM0', 9600)
while True:
	print ser.readline()
	#'''
	'''byte = ser.read()
	if byte == '<':
		print '<',
		while True:
			byte = ser.read()
			if byte == '>':
				break
			if byte not in { 'O', 'F', 'A', 'X', 'Y' }:
				print ord(byte),
			else:
				print byte,
		print '>' 
	#'''
