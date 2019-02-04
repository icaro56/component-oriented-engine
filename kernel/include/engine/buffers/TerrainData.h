#ifndef TERRAIN_DATA_H
#define TERRAIN_DATA_H

#include "KernelPrerequisites.h"
#include "PTPrerequisites.h"

class KERNEL_VS_EXPORT TerrainData
{
public:
	TerrainData();
	~TerrainData();

	void setData(HEIGHTMAPTYPE* d);
	float* getData();

	float getDataAt(int idx);

	void setHeight(double h);
	double getHeight();

	void setWidth(double w);
	double getWidth();

	void setPageSize(unsigned ps);
	unsigned getPageSize();

	unsigned getTotalSize();

private:

	float* data;
	double height;
	double width;
	unsigned pageSize;
	unsigned totalSize;
};

#endif