#include "../Component.h"
#include "math/Vector3.h"
#include "math/Color.h"


namespace Ogre
{
	class Mesh;
}

enum RenderType
{
	OT_POINT_LIST = Ogre::RenderOperation :: OT_POINT_LIST,				//|	A list of points  | 1 vertex per point |
	OT_LINE_LIST = Ogre::RenderOperation :: OT_LINE_LIST,				//|	A list of lines	  | 2 vertices per line |
	OT_LINE_STRIP = Ogre::RenderOperation :: OT_LINE_STRIP,				//|	A strip of connected lines	| 1 start vertex and 1 vertex per line |
	OT_TRIANGLE_LIST = Ogre::RenderOperation :: OT_TRIANGLE_LIST,		//|	A list of triangles	  | 3 vertices per triangle |
	OT_TRIANGLE_STRIP = Ogre::RenderOperation :: OT_TRIANGLE_STRIP,		//|	A strip of triangles   | 3 vertices for the first triangle and 1 per triangle after that |
	OT_TRIANGLE_FAN = Ogre::RenderOperation :: OT_TRIANGLE_FAN			//|	A fan of triangles	 | 3 vertices for the first triangle and 1 per triangle after that |
};

class KERNEL_VS_EXPORT ManualObject : public Component
{
public:
	enum manualObjectDesc
	{
		MO_Enabled,
		MO_RenderOperation,
		MO_Size
	};

	ManualObject(void);
	virtual ~ManualObject(void);
	ManualObject(const ComponentDescMap& d);

	virtual void prerequisites(){}
	virtual void nextInit(){}

	virtual void setEnabled(bool enable);
	virtual bool isEnabled();

	virtual void init(const std::string& name);

	void makeObject(QList<Vector3> lPoints,RenderType type);
	void makeObject(QList<Vector3> lPoints,QList<Color> lColors,QList<int> lIndex,RenderType type);
	void makeObject(QList<Vector3> lPoints,QList<Color> lColors,RenderType type);
	void makeObject(QList<Vector3> lPoints,QList<int> lIndex,RenderType type);
	void makeObject(QList<Vector3> lPoints,QList<Vector3> ltriangles,RenderType type);
	void makeObject(QList<Vector3> lPoints,QList<Vector3> ltriangles,QList<Vector3> lnormals,QList<Vector2> ltextureCoord,RenderType type);

	Ogre::ManualObject *getManual();
	QList<Vector3> getPoints();
	QList<Color> getColors();
	QList<int> getIndexs();

	void setPoints(QList<Vector3> Points);
	void setTriangles(QList<Vector3> Triangles);
	void setColors(QList<Color> Colors);
	void setIndex(QList<int> Index);
	void setRenderOperation(RenderType ro);

private:

	Ogre::ManualObject* mesh;
	QList<Vector3> points;
	QList<Vector3> triangles;
	QList<Vector3> normals;
	QList<Vector2> textureCoord;
	QList<Color> colors;
	QList<int> index;
	Ogre::String ManualObjectName;
	RenderType renderOperation;
	ComponentDescMap modesc;
};
