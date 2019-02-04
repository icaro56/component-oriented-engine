///////////////////////////////////////////////////////////
//  UiBasic.h
//  Implementation of the Class UiBasic
//  Created on:      
///////////////////////////////////////////////////////////

#if !defined(UI_BASIC_H) 
#define UI_BASIC_H

class QAction;
class QMenu;
class QToolBar;
class QMainWindow;

enum TypeUi
{
	Ui_About,
	Ui_AdvancedSettings,
	Ui_Alert,
	Ui_Anotation,
	Ui_CamAutomatic,
	Ui_CamAvatar,
	Ui_CamFree,
	Ui_CamOrbital,
	Ui_CamGE,
	Ui_Clock,
	Ui_Close,
	Ui_Compass,
	Ui_ConfigTerrain,
	Ui_ControlPanel,
	Ui_Deposit,
	Ui_Draw,
	Ui_EffectivenessMining,
	Ui_GeographicLandmark,
	Ui_GeologicalBlocks,
	Ui_Groundwater,
	Ui_Help,
	Ui_IndicatorAll,
	Ui_IndicatorPerimeter,
	Ui_InformationPanel,
	Ui_LegendStatusEquipment,
	Ui_LogAplication,
	Ui_Material,
	Ui_Mensuration,
	Ui_Minimap,
	Ui_ModificationTerrain,
	Ui_MonitoringRoads,
	Ui_MoveMine,
	Ui_ObjectInspector,
	Ui_PitMine,
	Ui_Planning,
	Ui_Plant,
	Ui_Pool,
	Ui_Preferences,
	Ui_RateProduction,
	Ui_RegisterEquipment,
	Ui_Reports,
	Ui_Route,
	Ui_ScreenCapture,
	Ui_Signalcoverage,
	Ui_StatusBar,
	Ui_Terrain,
	Ui_ToolBar,
	Ui_TopographicAnalysis,
	Ui_TopographicProfile,
	Ui_UserLogin,
	Ui_Wireframe,
	Ui_Splash
};

class UiBasic
{

public: 

	//! Destrutor
	virtual ~UiBasic();	

	/*!
     * \brief 
	 * \param 
     */
	virtual void setEnabledUi(bool) = 0;

	/*!
     * \brief 
	 * \param 
     */
	virtual void setCheckedUi(bool) = 0;

	/*!
     * \brief 
	 * \param 
     */
	virtual void setVisibleUi(bool) = 0;

	/*!
     * \brief 
	 * \param 
     */
	virtual void setFunctionalityEnabledUi(unsigned int,bool);

	/*!
     * \brief 
	 * \param 
     */
	virtual void setFunctionalityCheckedUi(unsigned int,bool);

	
	/*!
     * \brief 
	 * \param 
     */
	virtual void setFunctionalityVisibleUi(unsigned int,bool);

	/*!
     * \brief 
	 * \param 
     */
	virtual void addActionsInMenu(QMenu*);

	/*!
     * \brief 
	 * \param 
     */
	virtual void addActionsInToolbar(QToolBar*);

	/*!
     * \brief 
	 * \param 
     */
	virtual void addDockWidget(QMainWindow*);

	/*!
     * \brief 
	 * \param 
     */
	unsigned int getType() const;

protected:

	//! Construtor
	UiBasic(unsigned int);	

private:

	//! Construtor de copia
    UiBasic(const UiBasic&);

	//! operator de atribuição
    UiBasic& operator = (const UiBasic&);

private:

	unsigned int type;

};

#endif