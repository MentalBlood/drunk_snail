from sharpener_lite import Benchmark

from mako.template import Template



class table(Benchmark):

	def prepare(self, width, height):

		if not hasattr(self, 'table'):

			with open('templates/mako_template.xml', 'r', encoding='utf8') as f:
				text = f.read()

			self.table = Template(text)

			self.args = {
				'rows': [
					[
						str(x)
						for x in range(0, width)
					]
					for y in range(0, height)
				]
			}
	
	def run(self, **kwargs):
		self.table.render(**self.args)