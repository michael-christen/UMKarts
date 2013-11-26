# Usage:
#     python makebin.py /path/to/sound/samples
# Creates:
#     flash.bin, sound_samples.h

import os, sys

header_start = """// Auto-generated with makebin.py
#ifndef SOUND_SAMPLES_H
#define SOUND_SAMPLES_H

"""

header_end = """
#endif /* SOUND_SAMPLES_H */
"""


header = open('sound_samples.h', 'w')
binout = open('flash.bin', 'wb')

header.write(header_start)

count = 0;
for file in os.listdir(sys.argv[1]):
	if file.endswith('.raw'):
		name = (os.path.splitext(file)[0]).upper()
		
		f = open(os.path.join(sys.argv[1], file), 'rb')
		data = f.read()
		header.write('#define ' + name + '_BEGIN ' + str(count) + '\n')
		count += len(data)
		header.write('#define ' + name + '_END ' + str(count) + '\n\n')
		
		binout.write(data)
	
header.write(header_end)
header.close()
binout.close()