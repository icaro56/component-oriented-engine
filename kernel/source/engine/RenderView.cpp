#include "engine/RenderView.h"

#include "engine/Engine.h"
#include "engine/RenderingEngine.h"
#include "components/rendering/Camera.h"

#include <OgreViewport.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
//#include <MyGUI_OgrePlatform.h>

#include <QEvent>
#include <QFocusEvent>
#include "../MainWindow.h"

#include "utils/KernelConverter.h"

#include "moc_RenderView.cpp"


RenderView::RenderView(const std::string &name, QWidget* parent)
    : QWidget(parent, Qt::MSWindowsOwnDC), 
	mCameraComponent(nullptr),
	mOgreWindow(nullptr), 
	haveMyGUI(false), 
	onRender(false), 
	mIsInitialised(false),
	mRenderStats(nullptr),
	mOverlayEnabled(true),
	autoAspectRatio(true)
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_PaintOnScreen);

    setMinimumSize(320,240);
    setGeometry(0, 0, parent->width(), parent->height());


    setFocusPolicy(Qt::StrongFocus);
    setNameId(name);
}

void RenderView::initialise(bool allowPerfHUD)
{
	mRenderStats = new RenderStats;

	///Criando OgreWindow
    // parameters list
    Ogre::NameValuePairList opts;
    opts["externalWindowHandle"] =
        Convert::toString((size_t)((HWND)this->winId()));

    // vsync
	Ogre::ConfigOptionMap map = Ogre::Root::getSingletonPtr()->getRenderSystem()->getConfigOptions();

    opts["vsync"] = map["VSync"].currentValue == "Yes" ? "true" : "false";
    opts["FSAA"] = map["FSAA"].currentValue;
	//opts["useNVPerfHUD"] = map["Allow NVPerfHUD"].currentValue == "Yes" ? "true" : "false";
	opts["useNVPerfHUD"] = allowPerfHUD ? "true" : "false";

    // create window
	mOgreWindow = Ogre::Root::getSingleton().createRenderWindow(mName.toStdString(),
                                            width(), height(),
                                             false, &opts);

    mOgreWindow->setActive(true);

	Rendering->addRenderView(this);

	mIsInitialised = true;
}

//---------------------------------------------------------
RenderView::~RenderView()
{
	//funfando
	Rendering->removeRenderView(this);

	if (mOgreWindow)
	{
		mOgreWindow->removeListener(this);
		mOgreWindow->removeAllListeners();
		mOgreWindow->removeAllViewports();

		Ogre::Root* r = Ogre::Root::getSingletonPtr();
		if (r)
		{
			//chamando esse comando sozinho está dando certo também
			r->getRenderSystem()->destroyRenderWindow(mOgreWindow->getName());
		}
	}

	mOgreWindow = nullptr;

	mCameraComponent = nullptr;

	if (mRenderStats)
	{
		delete mRenderStats;
		mRenderStats = nullptr;
	}
}

//---------------------------------------------------------
void RenderView::setRenderWindow(Ogre::RenderWindow *window)
{
    mOgreWindow = window;
}

//---------------------------------------------------------
Ogre::RenderWindow *RenderView::getRenderWindow()
{
    return mOgreWindow;
}

//---------------------------------------------------------
void RenderView::setCamera(Camera *cam, int ZOrder)
{
    Ogre::Viewport* mViewport = addViewport(cam, ZOrder);

	cam->setViewport(mViewport);

    mCameraComponent = cam;
}

Ogre::Viewport* RenderView::addViewport(Camera* cam, int ZOrder, float left, float top, float width, float height)
{
	return mOgreWindow->addViewport(cam->getOgreCamera(), ZOrder, left, top, width, height);
}

//---------------------------------------------------------
Camera * RenderView::getCamera()
{
    return mCameraComponent;
}

//---------------------------------------------------------
void RenderView::setNameId(const std::string & n)
{
    mName = QString(n.c_str());
}

//-----------------------------------------------------------
const std::string RenderView::getNameId() const
{
    return mName.toStdString();
}

//---------------------------------------------------------
void RenderView::resizeWindow(int w, int h)
{
	if (mIsInitialised && mOgreWindow)
	{
		//Parece que só precisa ser chamado em Linux.
		//Quando ativo causa bug do printscreen
		//quando janela é extendida para 2 monitores causa bug
		//mOgreWindow->resize(w, h);
		mOgreWindow->windowMovedOrResized();

		// ajust camera aspect ratio
		if (autoAspectRatio && mCameraComponent->getViewport())
		{
			mCameraComponent->resizeViewport(w, h);
		}

		/*if ((haveMyGUI) && (GRender->haveMyGUI()))
		{
			int num = GRender->getOgrePlatform()->getRenderManagerPtr()->getActiveViewport();
			GRender->getOgrePlatform()->getRenderManagerPtr()->setActiveViewport(num);
			
		}*/
	}
}

//----------------------------------------------------------
Vector2 RenderView::getDimensions()
{
    unsigned int w, h;
    w = mOgreWindow->getWidth();
    h = mOgreWindow->getHeight();

    return Vector2(w,h);
}

//---------------------------------------------------------
Vector2 RenderView::getAbsolutePosition()
{
	QPoint p;
	QWidget *parent = this->parentWidget();

	if (parent && parent->inherits("QMainWindow"))
	{
		p = this->mapToParent(QPoint(0,0));
	}
	else
	{
		p = this->mapToGlobal(QPoint(0,0));
	}


    return Vector2(p.x(),p.y());
}

//---------------------------------------------------------
RenderStats *RenderView::getRenderStats()
{
    const Ogre::RenderTarget::FrameStats &stats = mOgreWindow->getStatistics();
    mRenderStats->avgFPS = stats.avgFPS;
    mRenderStats->batchCount = stats.batchCount;
    mRenderStats->bestFPS = stats.bestFPS;
    mRenderStats->bestFrameTime = stats.bestFrameTime;
    mRenderStats->lastFPS = stats.lastFPS;
    mRenderStats->triangleCount = stats.triangleCount;
    mRenderStats->worstFPS = stats.worstFPS;
    mRenderStats->worstFrameTime = stats.worstFrameTime;
    return mRenderStats;
}
//---------------------------------------------------------
void RenderView::enableOverlays(bool enable)
{
    if(!enable && mOverlayEnabled)
    {
        //Ogre::RenderTarget *target = mViewport->getTarget();
        mOgreWindow->addListener(this);
    }
    else if(enable && !mOverlayEnabled)
    {
        //Ogre::RenderTarget *target = mViewport->getTarget();
        mOgreWindow->removeListener(this);
    }
}
//---------------------------------------------------------
void RenderView::preRenderTargetUpdate (const Ogre::RenderTargetEvent &evt)
{
	if (mIsInitialised)
	{
		// hide all visible overlays
		Ogre::OverlayManager *man = Ogre::OverlayManager::getSingletonPtr();
		Ogre::OverlayManager::OverlayMapIterator iter = man->getOverlayIterator();

		while (iter.hasMoreElements())
		{
			Ogre::Overlay *ov = iter.getNext();
			if (ov->isVisible())
			{
				mOvList.push_back(ov);
				ov->hide();
			}
		}
	}
}
//---------------------------------------------------------
void RenderView::postRenderTargetUpdate (const Ogre::RenderTargetEvent &evt)
{
	if (mIsInitialised)
	{
		// show overlays previously hidden
		OverlayList::const_iterator iter = mOvList.begin();

		for (; iter != mOvList.end(); ++iter)
		{
			(*iter)->show();
		}

		mOvList.clear();
	}
}

//--------------------------------------------------------
bool RenderView::isOnRender() const
{
    return onRender;
}

//--------------------------------------------------------
/*bool RenderView::isActived() const
{
    return actived;
}
*/
///////////////////////////////////////////////////////////
/*void OgreWidget::moveEvent(QMoveEvent *e)
{
    QWidget::moveEvent(e);

    if(e->isAccepted() && ogreRenderWindow)
    {
        ogreRenderWindow->windowMovedOrResized();
        update();
    }
}*/
//----------------------------------------------------------
void RenderView::resizeEvent(QResizeEvent *evt)
{
	if (mIsInitialised)
	{
		/*const QSize &newSize = evt->size();
		resizeWindow(newSize.width(), newSize.height());*/

		//passando a largura e altura considerando windows frame (em fase de teste)
		resizeWindow(frameGeometry().width(), frameGeometry().height());

		QWidget::resizeEvent(evt);
		
		emit resizeEventOcurred();
	}
}

//----------------------------------------------------------
void RenderView::enterEvent(QEvent *evt)
{
    //Q_UNUSED(evt);
    //setFocus(Qt::MouseFocusReason);
    onRender = true;
    evt->setAccepted(true);
}

//----------------------------------------------------------
void RenderView::leaveEvent(QEvent *evt)
{
    //Q_UNUSED(evt);
    //setFocus(Qt::MouseFocusReason);
    onRender = false;
    evt->setAccepted(true);
}

////----------------------------------------------------------
/*void RenderView::focusInEvent(QFocusEvent *evt)
{
//	Q_UNUSED(evt);
    //actived = true;
    if(!hasFocus()){
        setFocus();
    }
    evt->setAccepted(true);
}
void RenderView::focusOutEvent(QFocusEvent *evt)
{
	//Q_UNUSED(evt);
	//actived = false;
	if(!hasFocus()){
        setFocus();
	}
	evt->setAccepted(true);
	//printf("\nEntrei no focusOutEvent");
}*/
//
////----------------------------------------------------------
void RenderView::paintEvent(QPaintEvent *evt)
{
	if (mIsInitialised && isVisible() && mOgreWindow)
	{
		mOgreWindow->update();
		evt->accept();
	}
}

QPaintEngine* RenderView::paintEngine() const
{
    // We don't want another paint engine to get in the way for our Ogre based paint engine.
    // So we return nothing.
    return NULL;
}

void RenderView::setHaveMyGUI(const bool enable)
{
    haveMyGUI = enable;
}

bool RenderView::getHaveMyGUI()
{
	return haveMyGUI;
}

void RenderView::setAutoAspectRatio(bool active)
{
	autoAspectRatio = active;
}

bool RenderView::getAutoAspectRatio()
{
	return autoAspectRatio;
}