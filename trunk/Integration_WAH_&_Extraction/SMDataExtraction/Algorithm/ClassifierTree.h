#pragma once
#include "modelselection.h"
#include "WrapDataSource.h"

class ClassifierTree
{
public:
	_declspec(dllexport) ClassifierTree(void);
	_declspec(dllexport) virtual ~ClassifierTree(void);
	_declspec(dllexport) ClassifierTree(ModelSelection * toSelectLocModel);
	/**
	* Method for building a classifier tree.
	*
	* @param data the data to build the tree from	
	*/
	_declspec(dllexport) void buildTree(WrapDataSource * data, boolean keepData);
	_declspec(dllexport) void buildClassifier(WrapDataSource * data);
	
private:
	/** The model selection method. */  
	ModelSelection * m_toSelectModel;     

	/** Local model at node. */
	//ClassifierSplitModel m_localModel;  

	/** References to sons. */
	ClassifierTree * m_sons;           

	/** True if node is leaf. */
	bool m_isLeaf;                   

	/** True if node is empty. */
	bool m_isEmpty;                  

	/** The training instances. */
	WrapDataSource * m_train;                  

	/** The pruning instances. */
	//Distribution m_test;     

	/** The id for the node. */
	int m_id;

};
