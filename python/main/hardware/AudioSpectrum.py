


bin_count = 3

class AudioSpectrum:
	def __init__(self):
		self.bins = [0.0] * bin_count
		self.levels = [0.0] * bin_count


	def set_bin(self,index, bin, level):
		self.bins[index] = bin
		self.levels[index] = level
	
 

