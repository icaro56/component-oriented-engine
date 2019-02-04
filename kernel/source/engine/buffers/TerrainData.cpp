#include "engine/buffers/TerrainData.h"

TerrainData::TerrainData()
	: data(nullptr),
	height(0),
	width(0),
	pageSize(0),
	totalSize(0)
{

}

TerrainData::~TerrainData()
{
	delete [] data;
}

void TerrainData::setData(HEIGHTMAPTYPE* d)
{
	data = new float[totalSize];

	//data = d;

	float aux = 1.0f / 65535.0f;

	for(unsigned int x = 0; x < totalSize; ++x)
	{
		data[x] = d[x] * (aux);
	}
}

float* TerrainData::getData()
{
	return data;
}

float TerrainData::getDataAt(int idx)
{
	return data[idx];
}

void TerrainData::setHeight(double h)
{
	height = h;
}

double TerrainData::getHeight()
{
	return height;
}

void TerrainData::setWidth(double w)
{
	width = w;
}

double TerrainData::getWidth()
{
	return width;
}

void TerrainData::setPageSize(unsigned ps)
{
	pageSize = ps;
	totalSize = pageSize * pageSize;
}

unsigned TerrainData::getPageSize()
{
	return pageSize;
}

unsigned TerrainData::getTotalSize()
{
	return totalSize;
}