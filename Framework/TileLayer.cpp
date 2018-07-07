#include "TileLayer.h"

TileLayer::TileLayer()
{
	_Width = 0;
	_Height = 0;
	_Data = NULL;
}

TileLayer::~TileLayer()
{
	for (unsigned int i = 0; i < _Height; i++) {
		delete[] _Data[i];
	}
	delete[] _Data;
}

TileLayer::TileLayer(const TileLayer &TileLayer)
{
	_LayerName = TileLayer._LayerName;
	_Width = TileLayer._Width;
	_Height = TileLayer._Height;

	//copy data
	_Data = new unsigned int*[_Height];
	for (unsigned int row = 0; row < _Height; row++)
	{
		_Data[row] = new unsigned int[_Width];

		for (unsigned int column = 0; column < _Width; column++)
		{
			_Data[row][column] = TileLayer._Data[row][column];

		}
	}

}
TileLayer& TileLayer::operator=(const TileLayer &TileLayer)
{
	_LayerName = TileLayer._LayerName;
	_Width = TileLayer._Width;
	_Height = TileLayer._Height;

	//copy data
	_Data = new unsigned int*[_Height];
	for (unsigned int row = 0; row < _Height; row++)
	{
		_Data[row] = new unsigned int[_Width];

		for (unsigned int column = 0; column < _Width; column++)
		{
			_Data[row][column] = TileLayer._Data[row][column];

		}
	}

	return *this;
}


void TileLayer::SetAttributes(const std::string& name, unsigned int width, unsigned int height)
{
	_LayerName = name;
	_Width = width;
	_Height = height;

}

void  TileLayer::TileLayer::SetData(char* matrixCSV)
{
	_Data = new unsigned int*[_Height];

	for (unsigned int i = 0; i < _Height; i++)
	{
		_Data[i] = new unsigned int[_Width];
	}

	char* context = NULL;
	const char *separators = " \t,\n\r";

	//get data (convert matrixCSV to int matrix)
	char* item = strtok_s(matrixCSV, separators, &context);

	for (unsigned int row = 0; row < _Height; row++)
	{
		for (unsigned int column = 0; column < _Width; column++)
		{
			_Data[row][column] = atoi(item);
			item = strtok_s(0, separators, &context);
		}
	}

}



const std::string& TileLayer::GetName() const
{
	return _LayerName;
}


unsigned int TileLayer::GetWidth() const
{
	return _Width;
}


unsigned int TileLayer::GetHeight() const
{
	return _Height;
}

unsigned int** TileLayer::GetData() const
{
	return _Data;
}