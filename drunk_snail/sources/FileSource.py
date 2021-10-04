from .. import Source



class FileSource(Source):

	def __init__(self, path):
		self.path = path
	
	def get(self):
		with open(self.path) as f:
			return f.read()



import sys
sys.modules[__name__] = FileSource