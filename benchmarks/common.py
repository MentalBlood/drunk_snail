import pytest
import typing
import pathlib



@pytest.fixture
def table_width() -> int:
	return 100


@pytest.fixture
def table_height() -> int:
	return 100


@pytest.fixture
def templates() -> pathlib.Path:
	return pathlib.Path(__file__).parent / 'templates'


@pytest.fixture
def cell_value(table_width, table_height) -> typing.Callable[[int, int], str]:
	return lambda x, y: str(x + y * table_width)