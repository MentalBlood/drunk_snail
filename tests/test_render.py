import os
import pytest
from time import sleep

from drunk_snail import Template
from drunk_snail.syntax import default_keywords
from drunk_snail.sources import StringSource, FileSource



keywords = {
	'open_tag': '<!--',
	'close_tag': '-->',
	'param_operator': '(param)',
	'ref_operator': '(ref)'
}

approaches = ['comprehension', 'append', 'ragel']


@pytest.mark.parametrize('approach', approaches)
def test_basic(approach: str):
	
	result = Template(
		'test_render_basic', 
		StringSource('<!-- (param)x -->'), 
		keywords,
		approach=approach
	)({
		'x': 'lalala'
	})
	assert result == 'lalala' or result == 'lalala\n'

	result = Template(
		'test_render_basic', 
		StringSource('<!-- (param)x -->\n'),
		approach=approach
	)({
		'x': 'lalala'
	})
	assert result == 'lalala' or result == 'lalala\n'

	result = Template(
		'test_render_basic', 
		StringSource('lalala'),
		approach=approach
	)()
	assert result == 'lalala' or result == 'lalala\n'


@pytest.mark.parametrize('approach', approaches)
def test_empty_template(approach: str):
	assert Template('test_render_empty_template', StringSource(''), approach=approach)() == ''


def test_nonexistent_file():
	with pytest.raises(FileNotFoundError):
		Template('test_render_nonexistent_file', FileSource('lalala', watch=False))


@pytest.mark.parametrize('approach', approaches)
def test_list(approach: str):

	result = Template(
		'test_render_list', 
		StringSource('<!-- (param)some_param -->\n'), 
		keywords,
		approach=approach
	)({
		'some_param': ['1', '2', '3']
	})
	assert result == '1\n2\n3' or result == '1\n2\n3\n'

	result = Template(
		'test_render_list', 
		StringSource('<!-- (param)some_param -->'), 
		keywords,
		approach=approach
	)({
		'some_param': ['1', '2', '3']
	})
	assert result == '1\n2\n3' or result == '1\n2\n3\n'


@pytest.mark.parametrize('approach', approaches)
def test_ref(approach: str):

	Template(
		'addition', 
		StringSource('Nice to <!-- (param)action --> you'), 
		keywords,
		approach=approach
	)

	t = Template(
		'greeting', 
		StringSource('Hello, <!-- (param)name -->!\n<!-- (ref)addition -->!\n'), 
		keywords,
		approach=approach
	)
	print(t.compiled)
	
	result = t({
		'name': 'username',
		'addition': {
			'action': 'eat'
		}
	})
	print(result)
	assert result == 'Hello, username!\nNice to eat you!' or result == 'Hello, username!\nNice to eat you!\n'

	result = Template('greeting')({
		'name': 'username',
		'addition': [{
			'action': 'meet'
		}, {
			'action': 'eat'
		}]
	})
	assert result == 'Hello, username!\nNice to meet you!\nNice to eat you!' or result == 'Hello, username!\nNice to meet you!\nNice to eat you!\n'


@pytest.mark.parametrize('approach', approaches)
def test_buf_overflow(approach: str):

	t1 = Template(
		'test_buf_overflow_1', 
		StringSource(' ' * 1000),
		keywords,
		approach=approach
	)

	t2 = Template(
		'test_buf_overflow_2', 
		StringSource('<!-- (ref)test_buf_overflow_1 -->'),
		keywords,
		approach=approach
	)

	assert t2.compiled


@pytest.mark.parametrize('approach', approaches)
def test_consicutive_lines(approach: str):

	t1 = Template(
		'test_consicutive_lines_1',
		StringSource('a'),
		approach=approach
	)

	t2 = Template(
		'test_consicutive_lines_2',
		StringSource('b'),
		approach=approach
	)

	t3 = Template(
		'test_consicutive_lines_3',
		StringSource('''
\t<!-- (optional)(ref)test_consicutive_lines_1 -->
\t<!-- (optional)(ref)test_consicutive_lines_2 -->
'''),
		approach=approach
	)

	result = t3({
		'test_consicutive_lines_1': {},
		'test_consicutive_lines_2': {}
	})
	assert result == '\n\ta\n\tb' or result == '\n\ta\n\tb\n'


@pytest.mark.parametrize('approach', approaches)
def test_optional_param(approach: str):
	
	t = Template(
		'test_optional_param',
		StringSource('<!-- (optional)(param)a -->'),
		approach=approach
	)
	print(Template('test_optional_param', approach='comprehension').compiled)
	print(Template('test_optional_param', approach='ragel').compiled)
	
	assert t() == ''


@pytest.mark.parametrize('approach', approaches)
def test_optional_ref(approach: str):

	Template(
		'test_optional_ref_1',
		StringSource('lalala'),
		approach=approach
	)
	t = Template(
		'test_optional_ref_2',
		StringSource('<!-- (optional)(ref)test_optional_ref_1 -->'),
		approach=approach
	)
	
	assert t() == ''
	
	result = t({
		'test_optional_ref_1': [None]
	})
	assert result == 'lalala' or result == 'lalala\n'


@pytest.mark.parametrize('approach', ['append'])
def test_shift(approach: str):

	for letter in ['a', 'b', 'c', 'd']:
		Template(letter, FileSource(f'templates/{letter.upper()}.xml'), approach=approach)
	
	result = Template('a')({'B': {'C': [{}, {}]}})

	valid_tabs_number = 60
	assert result.count('\t') == valid_tabs_number


@pytest.mark.parametrize('approach', approaches)
def test_cyrillic(approach: str):

	t = Template('test_render_cyrillic', StringSource('ляляля'), approach=approach)

	assert t() == 'ляляля'


@pytest.mark.parametrize('approach', approaches)
def test_table(approach: str):

	row = Template('Row', FileSource('templates/Row.xml'), approach=approach)
	table = Template('Table', FileSource('templates/Table.xml'), approach=approach)

	print(table.compiled)
	# assert False
	
	args = {
		"Row": [
			{"cell": ["1.1", "2.1", "3.1"]},
			{"cell": ["1.2", "2.2", "3.2"]},
			{"cell": ["1.3", "2.3", "3.3"]}
		]
	}

	result = table(args)
	print(result)

	with open('tests/table_correct_result.xml', encoding='utf8') as f:
		correct_result = f.read()
	assert result == correct_result

test_table('ragel')


@pytest.mark.parametrize('approach', approaches)
def test_endpoint_template(approach: str):

	t = Template('test_compile_endpoint_template', FileSource('templates/endpoint_template.txt'), approach=approach)
	print(t.compiled)
	print(t({
		'route_to': 'route_to',
		'methods': 'methods',
		'handler_name': 'handler_name',
		'handler_args': ', '.join(['a', 'b'])
	}))

	assert t({
		'route_to': 'route_to',
		'methods': 'methods',
		'handler_name': 'handler_name',
		'handler_args': ', '.join(['a', 'b'])
	}) == """from ..common import *



@route(
	None, 
	'route_to', 
	methods=methods
)
def handler_name(
	a, b
):
	return Response(status=200)"""


# test_endpoint_template('ragel')
# print('ok')