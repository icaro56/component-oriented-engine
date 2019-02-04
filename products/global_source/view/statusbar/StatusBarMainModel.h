///////////////////////////////////////////////////////////
//  StatusBarModel.h
//  Implementation of the Class StatusBarModel
///////////////////////////////////////////////////////////

#if !defined(STATUS_BAR_MAIN_MODEL_H)
#define STATUS_BAR_MAIN_MODEL_H

#include <QStringList>

class StatusBarMainModel
{	

public:

	 //! Construtor
    StatusBarMainModel();

    //! Destrutor
    ~StatusBarMainModel();

	/*!
     * \brief 
	 * \param 
     */
	QStringList calculateCoordinated(const QString& type,const double& x, 
											const double& y, const double& z);		
	
};


#endif //STATUS_BAR_MAIN_MODEL_H