///////////////////////////////////////////////////////////
//  Functions.h
//  
///////////////////////////////////////////////////////////

#if !defined(FUNCTIONS_H)
#define FUNCTIONS_H

class QString;

namespace Collision2
{
	/*!
     * \brief 
	 * \return 
     */
	bool hasCollision(int mx,int my,QString & r);
};

namespace Position
{
	/*!
     * \brief 
	 * \return 
     */
	void mouseRelativePosition(int mx,int my, double& x, double& y);

};


#endif // ABOUT_H
