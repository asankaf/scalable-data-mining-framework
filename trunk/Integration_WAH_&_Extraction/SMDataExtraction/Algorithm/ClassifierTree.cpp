#include "StdAfx.h"
#include "ClassifierTree.h"


ClassifierTree::ClassifierTree(void)
{
	init();
}

void ClassifierTree::init()
{
	m_toSelectModel = NULL;
	m_test = NULL;
	m_sons = NULL;
	m_exitec = NULL;
}

ClassifierTree::ClassifierTree(ModelSelection * toSelectLocModel)
{
	init();
	m_toSelectModel = toSelectLocModel;
}

ClassifierTree::~ClassifierTree(void)
{
	m_toSelectModel = NULL;
	m_test = NULL;
	if(m_sons != NULL){
	for (size_t i = 0 ;  i < m_sonsLength ; i++)
	{
		if (m_sons[i] != NULL)
		{
			delete m_sons[i];
			m_sons[i] = NULL;
		}
		
	}
	}
	if (m_exitec != NULL)
	{
		delete m_exitec;
	}
// 	delete m_localModel;
// 	m_localModel = NULL;
	
}

int ClassifierTree::numNodes()
{
	int no = 1;
	int i;

	if (!m_isLeaf){
		for (i=0;i<m_sonsLength;i++)
		{
			no = no+m_sons[i]->numNodes();
		}
	}
	return no;
}

string ClassifierTree::toString(){

		string text;

		if (m_isLeaf) {
			text.append(": ");
			text.append(m_localModel->dumpLabel(0,m_train));
		}else
		dumpTree(0,text);
		text.append("\n\nNumber of Leaves  : \t"+ Utils::toStringVal(numLeaves()) +"\n");
		text.append("\nSize of the tree : \t"+ Utils::toStringVal(numNodes()) +"\n");

		return text;
	 
}

string ClassifierTree::toGraph()
{
	string text;

	if (m_isLeaf) {
		text.append(": ");
		text.append(m_localModel->dumpLabel(0,m_train));
	}else
		dumpTree(0,text);
	//text.append("\n\nNumber of Leaves  : \t"+ Utils::toStringVal(numLeaves()) +"\n");
	//text.append("\nSize of the tree : \t"+ Utils::toStringVal(numNodes()) +"\n");
	text.erase(text.begin());
	return text;
}

void ClassifierTree::dumpTree(int depth, string & text) 
{
	int i,j;

	for (i=0;i<m_sonsLength;i++)
	{
		text.append("\n");
		for (j=0;j<depth;j++)
		{
			text.append("|   ");
		}
		text.append(m_localModel->leftSide(m_train));
		text.append(m_localModel->rightSide(i, m_train));
		if (m_sons[i]->m_isLeaf)
		{
			text.append(": ");
			text.append(m_localModel->dumpLabel(i,m_train));
		}else
		{
			m_sons[i]->dumpTree(depth+1,text);
		}
	}
}

int ClassifierTree::numLeaves()
{
	int num = 0;
	int i;

	if (m_isLeaf)
		return 1;
	else
		for (i=0;i<m_sonsLength;i++)
		{
			num = num+m_sons[i]->numLeaves();
		}

	return num;
}

void ClassifierTree::buildClassifier( DataSource * data,BitStreamInfo * _existence_bitmap )
{
	//buildTree(data, false);
}

ClassifierTree * ClassifierTree::getNewTree(DataSource * _data, BitStreamInfo * _existence_map)
{
	ClassifierTree * newTree = new ClassifierTree(m_toSelectModel);
	newTree->buildTree(_data,_existence_map,false);

	return newTree;
}

void ClassifierTree::buildTree(DataSource * data, BitStreamInfo * _existence_map,boolean keepData )
{

	vector<BitStreamInfo *> localInstances;

	if (keepData) {
		m_train = data;
	}
	
	m_exitec = _existence_map;

	m_test = NULL;
	m_isLeaf = false;
	m_isEmpty = false;
	m_sons = NULL;
	m_localModel = m_toSelectModel->selectModel(data,_existence_map);
	//m_localModel->Print();
 	if (m_localModel->NumSubsets() > 1) {
		localInstances = m_localModel->Existence_maps();
		m_sons = new ClassifierTree * [m_localModel->NumSubsets()];
		m_sonsLength = m_localModel->NumSubsets();
		for (int i = 0; i < m_sonsLength; i++) 
		{
			m_sons[i] = getNewTree(data,localInstances[i]);
			localInstances[i] = NULL;
		}
		m_sons_vect = vector<ClassifierTree *>(m_sons,m_sons+m_sonsLength);
	}
	else
	{
		m_isLeaf = true;
		if (Utils::eq((double)_existence_map->Count(), 0))
		{
			m_isEmpty = true;
		}
		//data = NULL;
	}
}
