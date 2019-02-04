
#include "AxisObject.h"
#include <string.h>
#include <OgreMeshManager.h>
#include <OgreManualObject.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>

AxisObject::AxisObject()
{

	createAxesMaterial();
	createAxesMesh();

}

AxisObject::~AxisObject()
{

}
 
void AxisObject::createAxesMesh()
{
	Ogre::String meshName = "GizmoAxesMesh.mesh";
    mAxesMeshPtr = Ogre::MeshManager::getSingleton().getByName(meshName);
    if (mAxesMeshPtr.isNull())
    {
        Ogre::ManualObject mo("tmp");
        mo.begin(mAxisMatPtr->getName());

        /* 3 axes, each made up of 2 of these (base plane = XY)
        *   .------------|\
        *   '------------|/
        */
        mo.estimateVertexCount(7 * 2 * 3);
        mo.estimateIndexCount(3 * 2 * 3);
        Ogre::Quaternion quat[6];
        Ogre::ColourValue col[3];
 
        // x-axis
        quat[0] = Ogre::Quaternion::IDENTITY;
        quat[1].FromAxes(Ogre::Vector3::UNIT_X, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y);
        col[0] = Ogre::ColourValue::Red;
        col[0].a = 0.3;
        // y-axis
        quat[2].FromAxes(Ogre::Vector3::UNIT_Y, Ogre::Vector3::NEGATIVE_UNIT_X, Ogre::Vector3::UNIT_Z);
        quat[3].FromAxes(Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_X);
        col[1] = Ogre::ColourValue::Green;
        col[1].a = 0.3;
        // z-axis
        quat[4].FromAxes(Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_Y, Ogre::Vector3::NEGATIVE_UNIT_X);
        quat[5].FromAxes(Ogre::Vector3::UNIT_Z, Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y);
        col[2] = Ogre::ColourValue::Blue;
        col[2].a = 0.3;
 
        Ogre::Vector3 basepos[7] =
        {
            // stalk
            Ogre::Vector3(0, 0.05, 0),
            Ogre::Vector3(0, -0.05, 0),
            Ogre::Vector3(0.7, -0.05, 0),
            Ogre::Vector3(0.7, 0.05, 0),
            // head
            Ogre::Vector3(0.7, -0.15, 0),
            Ogre::Vector3(1, 0, 0),
            Ogre::Vector3(0.7, 0.15, 0)
        };
 
 
        // vertices
        // 6 arrows
        for (size_t i = 0; i < 6; ++i)
        {
            // 7 points
            for (size_t p = 0; p < 7; ++p)
            {
                Ogre::Vector3 pos = quat[i] * basepos[p];
                mo.position(pos);
                mo.colour(col[i / 2]);
            }
        }
 
        // indices
        // 6 arrows
        for (size_t i = 0; i < 6; ++i)
        {
            size_t base = i * 7;
            mo.triangle(base + 0, base + 1, base + 2);
            mo.triangle(base + 0, base + 2, base + 3);
            mo.triangle(base + 4, base + 5, base + 6);
        }
 
        mo.end();
 
        mAxesMeshPtr = mo.convertToMesh(meshName, "General");
    }
}

void AxisObject::createAxesMaterial()
{
    Ogre::String matName = "SkeletonDebugAxesMat";
 
    mAxisMatPtr = Ogre::MaterialManager::getSingleton().getByName(matName);
    if (mAxisMatPtr.isNull())
    {
        mAxisMatPtr = Ogre::MaterialManager::getSingleton().create(matName, "General");
 
        // First pass for axes that are partially within the model (shows transparency)
        Ogre::Pass* p = mAxisMatPtr->getTechnique(0)->getPass(0);
        p->setLightingEnabled(false);
        p->setPolygonModeOverrideable(false);
        p->setVertexColourTracking(Ogre::TVC_AMBIENT);
        p->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
        p->setCullingMode(Ogre::CULL_NONE);
        p->setDepthWriteEnabled(false);
        p->setDepthCheckEnabled(false);
 
        // Second pass for the portion of the axis that is outside the model (solid colour)
        Ogre::Pass* p2 = mAxisMatPtr->getTechnique(0)->createPass();
        p2->setLightingEnabled(false);
        p2->setPolygonModeOverrideable(false);
        p2->setVertexColourTracking(Ogre::TVC_AMBIENT);
        p2->setCullingMode(Ogre::CULL_NONE);
        p2->setDepthWriteEnabled(false);
    }
}