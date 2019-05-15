#ifndef OptimisationRun_H
#define OptimisationRun_H

#include <vector>
#include <list>
#include <utility>
#include <QRectF>
#include <memory>

#include "Enumerations.h"
#include "Profile.h"
#include "Mesh.h"
#include "ProcessManager.h"
#include "BoundaryPoint.h"
#include <QString>

#include <clusterManager.h>


class OptimisationModel;

/**
 * @brief The Optimisation class
 * This class stores project data
 */
class Optimisation
{
public:
	/**
     * @brief OptimisationRun
     * Constructor and destructor for
     * the OptimisationRun class.
	 */
    Optimisation();
    ~Optimisation();

    //Getters and Setters for the class variables
    /**
	 * @brief setFunction
	 * @param function Sets the function.
	 */
	void setFunction(bool function);
	/**
	 * @brief boundary
	 * @return True if boundary is set.
	 */
	bool boundary() const;
	/**
	 * @brief setBoundary
	 * @param boundary Sets the boundary.
	 */
	void setBoundary(bool boundary);
	/**
	 * @brief optimiser
	 * @return True if optimiser is set.
	 */
    void setOptimisationMethod(Enum::OptMethod method);
	/**
	 * @brief runTime
	 * @return True if runtime is set.
	 */
	bool runTime() const;
	/**
	 * @brief setRunTime
	 * @param runTime
	 */
	void setRunTime(bool runTime);
	/**
	 * @brief renderProfile
	 * @return True if render profile is set.
	 */
	bool renderProfile() const;
	/**
	 * @brief setRenderProfile
	 * @param renderProfile Sets render profile.
	 */
	void setRenderProfile(bool renderProfile);
	/**
	 * @brief objFunc
	 * @return The current objective function.
	 */
	Enum::ObjFunc objFunc() const;
	/**
	 * @brief setObjFunc
	 * @param objFunc sets the current objective function.
	 */
	void setObjFunc(const Enum::ObjFunc& objFunc);
	/**
	 * @brief machNo
	 * @return Gets the current mach number.
	 */
	float machNo() const;
	/**
	 * @brief setMachNo
	 * @param machNo Sets the current mach number.
	 */
	void setMachNo(float machNo);
	/**
	 * @brief reNo
	 * @return Gets the current Reynalds number.
	 */
	float reNo() const;
	/**
	 * @brief setReNo
	 * @param reNo Sets the current reynalds number.
	 */
	void setReNo(float reNo);
	/**
	 * @brief freeAlpha
	 * @return Gets the current angle of attack.
	 */
	float freeAlpha() const;
	/**
	 * @brief setFreeAlpha
	 * @param freeAlpha Sets the current angle of attack.
	 */
	void setFreeAlpha(float freeAlpha);
	/**
	 * @brief freePress
	 * @return Gets the current free pressure.
	 */
	float freePress() const;
	/**
	 * @brief setFreePress
	 * @param freePress Sets the current pree pressure.
	 */
	void setFreePress(float freePress);
	/**
	 * @brief freeTemp
	 * @return Gets the current free temperature.
	 */
	float freeTemp() const;
	/**
	 * @brief setFreeTemp
	 * @param freeTemp Sets the current free temperature.
	 */
	void setFreeTemp(float freeTemp);
	/**
	 * @brief noAgents
	 * @return Gets the number of agents.
	 */
	int noAgents() const;
	/**
	 * @brief setNoAgents
	 * @param noAgents Sets the number of agents.
	 */
	void setNoAgents(int noAgents);
	/**
	 * @brief noGens
	 * @return Gets the number of generations.
	 */
	int noGens() const;
	/**
	 * @brief setNoGens
	 * @param noGens Sets the number of generations.
	 */
	void setNoGens(int noGens);
    /**
     * @brief getOptimisationMethod
     * @param returns the method index as defined by ordering in OptimiserDialog.ui.
     */
    Enum::OptMethod getOptimisationMethod() const;
    /**
     * @brief readFitness
     * @param read the fitness for this optimisation
     */
    bool readFitness();
    /**
     * @brief mesh
     * @param getter method for meshes
     */
    Mesh *mesh(int genIndex, int agentIndex);
    /**
     * @brief load
     * @param load the optimisation from disk (based on label)
     */
    bool load(QString aerOptInputFilePath);

    int getNoTop() const;
    void setNoTop(int noTop);

    QString label() const;
    QString simulationDirectoryName();
    QString simulationDirectoryPath();
    void setLabel(QString label);
    Mesh *initMesh();

    // control nodes
    std::vector<BoundaryPoint*> controlPoints();
    void setControlPoints(std::vector<BoundaryPoint*> controlPoints);
    int controlPointCount();

    bool run();
    void setModel(OptimisationModel* model);
    std::vector<std::vector<double>> allfitness();
    double fitness(int generationIndex, int agentIndex);
    QString outputText();
    std::pair<double,double> fitnessRange();

    ProfilePoints initProfilePoints();

    bool runOnCluster = false;
    QString mClusterPassword = "";

private:
    void optimiserFinished(int exitCode, QProcess::ExitStatus exitStatus);
    bool createAerOptInFile(const QString &filePath);
    bool createAerOptNodeFile(const QString &filePath);
    bool saveCurrentProfile(const QString& path);
    QString outputDataDirectory();
    bool readAerOptSettings(QString filePath);
    void addToOutputLog(const QString line);

    void writeProfilePointsToSimulationDir();
    bool readProfilePointsFromSimulationDir();
    void setInitProfilePoints(ProfilePoints profilePoints);

    bool readLogFromFile();
    QString logCacheFileName();

    // copy files
    void copyFileToSimulationDir(QString source);
    QString aerOptNodeFileCopyPath();
    QString aerOptInputFileCopyPath();

    /**
     * @brief mLabel Label for referring to this optimisation. It is used in naming.
     */
    QString mLabel = "";

	//Objective function attributes
    /**
     * @brief mObjFunc Optimisation Algorithm e.g. Modified Cuckoo Search
     */
    Enum::ObjFunc mObjFunc;

	//Boundary condition attributes
    /**
     * @brief mMachNo Mach Number
     */
	float mMachNo;

    /**
     * @brief mReNo Reynolds Number
     */
	float mReNo;

    /**
     * @brief mFreeAlpha Angle of attack
     */
	float mFreeAlpha;

    /**
     * @brief mFreePress Pressure Absolute
     */
	float mFreePress;

    /**
     * @brief mFreeTemp Temperature Absolute
     */
	float mFreeTemp;

	//Optimiser parameters
    /**
     * @brief mOptimisationMethod Optimisation Algorithm.
     */
    Enum::OptMethod mOptimisationMethod;

    /**
     * @brief mNoAgents Number of agents to use in optimisation algorithm.
     */
	int mNoAgents;

    /**
     * @brief mNoGens Number of generations to run optimisation algorithm for.
     */
	int mNoGens;

    /**
     * @brief mNoTop Number of fit agents to carry over to the next generation.
     */
    int mNoTop;

    Mesh* mInitMesh;
    /**
     * @brief mControlPoints List of optimisation boundary points that are also control points for optimisation.
     */
    std::vector<BoundaryPoint*> mControlPoints;

    /**
     * @brief mFitness Fitness values for each agent.
     */
    std::vector<std::vector<double>> mFitness;

    ProcessManager* mProcess = nullptr;

    /**
     * @brief mOptimisationModel Model that this optimisation is loaded in.
     */
    OptimisationModel* mOptimisationModel;

    /**
     * @brief mOutputLog Log of all optimisation messages.
     */
    QString mOutputLog = "";

    /**
     * @brief mProfilePoints List of points for loaded profile to optimise.
     */
    ProfilePoints mProfilePoints;

    clusterManager* clusterChecker = nullptr;
};



#endif // OptimisationRun_H
