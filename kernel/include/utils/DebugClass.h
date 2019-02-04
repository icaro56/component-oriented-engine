#ifndef DEBUG_H
#define DEBUG_H

#include "KernelPrerequisites.h"
#include <map>
#include "math/Vector3.h"
#include "math/Color.h"

#define Debug	DebugClass::getInstance()

class LineDebug
{
public:
	LineDebug();
	Vector3 start;	
	Vector3 end	;
	Color color;	
	float duration;
	int id;
	bool updated;
	bool death;
	bool initialized;
};

class RayDebug
{
public:
	RayDebug();
	Vector3 start;	
	Vector3 dir	;
	Color color;	
	float duration;
	int id;
	bool updated;
	bool death;
	bool initialized;
};


class KERNEL_VS_EXPORT DebugClass
{

	public:
		static DebugClass* getInstance();
		DebugClass(void);
        virtual ~DebugClass(void);
		void log(const std::string &message);
		void logError(const std::string &message);
		void logWarning(const std::string &message);
		void pause();
		void clearDeveloperConsole();
		void drawLine(const Vector3 &start,const Vector3 &end,  const Color &color, float duration, int id, bool update);
		void drawRay(const Vector3 &start, const Vector3 &dir,  const Color &color, float duration, int id, bool update);
		void updateDrawing();
		bool haveUpdate();
		
	private:
		void createMesh();
		void updateLineIndex(int id,const Vector3 &vecOne,const Vector3 &vecTwo,const Color &color, float duration, bool update);
		void updateRayIndex(int id,const Vector3 &vecOne,const Vector3 &vecTwo,const Color &color, float duration, bool update);
		static DebugClass* instance;
		QList<LineDebug> lineList;
		QList<RayDebug> rayList;
		Ogre::ManualObject* mesh;
		bool lineUpdate;
		bool rayUpdate;
};
#endif